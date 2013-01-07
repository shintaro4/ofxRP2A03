
#include "ofxRP2A03.h"

//--------------------------------------------------------------
// static methods
//--------------------------------------------------------------
void ofxRP2A03::setAudioSampleRate(int audio_sample_rate) {
    kAudioSampleRatef = static_cast<float>(audio_sample_rate);
}

//--------------------------------------------------------------
// static variables
//--------------------------------------------------------------
float ofxRP2A03::kAudioSampleRatef = 44100.0;

// Wave Table
// references:
//   波形メモリ音源
//   http://ja.wikipedia.org/wiki/%E6%B3%A2%E5%BD%A2%E3%83%A1%E3%83%A2%E3%83%AA%E9%9F%B3%E6%BA%90
//   Wavetable Synthesis
//   http://en.wikipedia.org/wiki/Wavetable_synthesis
const float ofxRP2A03::kWaveTable[4][16] = {
    // square(duty=12.5%)
    {0.25, 0.25, -0.25, -0.25, -0.25, -0.25, -0.25, -0.25,
        -0.25, -0.25, -0.25, -0.25, -0.25, -0.25, -0.25, -0.25},
    
    // square(duty=25.0%)
    {0.25, 0.25, 0.25, 0.25, -0.25, -0.25, -0.25, -0.25,
        -0.25, -0.25, -0.25, -0.25, -0.25, -0.25, -0.25, -0.25},
    
    // square(duty=50.0%)
    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25,
        -0.25, -0.25, -0.25, -0.25, -0.25, -0.25, -0.25, -0.25},
    
    // triangle
    {0.0, 0.125, 0.25, 0.375, 0.5, 0.375, 0.25, 0.125,
        0.0, -0.125, -0.25, -0.375, -0.5, -0.375, -0.25, -0.125}
};

// Pithch Envelope Table:
//   {"sample size", "repeat index",
//    "sample", "sample", ... , "sample"}
const float ofxRP2A03::kPitchEnvTable[5][18] = {
    // vibrato
    {16, 0,
        0, 0.25, 0.5, 0.75, 1, 0.75, 0.5, 0.25,
        0, -0.25, -0.5, -0.75, -1, -0.75, -0.5, -0.25},
    
    // increacing(low->target)
    {16, 15,
        -1.00, -0.93, -0.87, -0.80, -0.73, -0.67, -0.60, -0.53,
        -0.47, -0.40, -0.30, -0.27, -0.20, -0.13, -0.07, 0.00},

    // decreacing(high->target)
    {16, 15,
        1.00, 0.93, 0.87, 0.80, 0.73, 0.67, 0.60, 0.53,
        0.47, 0.40, 0.30, 0.27, 0.20, 0.13, 0.07, 0.00},

    // increacing(low->target) loop
    {16, 0,
        -1.00, -0.93, -0.87, -0.80, -0.73, -0.67, -0.60, -0.53,
        -0.47, -0.40, -0.30, -0.27, -0.20, -0.13, -0.07, 0.00},
    
    // decreacing(high->target) loop
    {16, 0,
        1.00, 0.93, 0.87, 0.80, 0.73, 0.67, 0.60, 0.53,
        0.47, 0.40, 0.30, 0.27, 0.20, 0.13, 0.07, 0.00}

};

// Volume Envelope Table:
//   {"sample size", "repeat index",
//    "sample", "sample", ... , "sample"}
const float ofxRP2A03::kVolumeEnvTable[3][18] = {
    // sustain
    {1, 0,
        1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0},
    
    // decay
    {16, 15,
        1.00, 0.93, 0.87, 0.80, 0.73, 0.67, 0.60, 0.53,
        0.47, 0.40, 0.30, 0.27, 0.20, 0.13, 0.07, 0.00},
    
    // reverse decay
    {16, 15,
        0.07, 0.13, 0.20, 0.27, 0.30, 0.40, 0.47, 0.53,
        0.60, 0.67, 0.73, 0.80, 0.87, 0.93, 1.00, 0.00}
};

//--------------------------------------------------------------
// constructor/destructor
//--------------------------------------------------------------
ofxRP2A03::ofxRP2A03()
    : note_number_(0), pan_(0.5), volume_(1.0), playing_(false),
      phase_(0.0), phase_increment_(0.0),
      current_level_(0.0), target_level_(0.0), pitch_modulation_(1.0),
      pitch_env_index_(0.0), volume_env_index_(0.0), noise_(-0.25) {
    for (int i = 0; i < 6; ++i) program_[i] = 0;
}

ofxRP2A03::ofxRP2A03(const int* program)
    : note_number_(0), pan_(0.5), volume_(1.0), playing_(false),
      phase_(0.0), phase_increment_(0.0),
      current_level_(0.0), target_level_(0.0), pitch_modulation_(1.0),
      pitch_env_index_(0.0), volume_env_index_(0.0), noise_(-0.25) {
    setProgram(program);
}

//--------------------------------------------------------------
// public methods
//--------------------------------------------------------------
void ofxRP2A03::play(int note_number) {
    note_number_ = note_number;
    pitch_env_index_ = volume_env_index_ = 0.0;
    current_level_ = 0.0;
    phase_ = 0.0;
    phase_increment_ = 440 * pow(2.0, (note_number_ - 69) / 12.0) /
                       kAudioSampleRatef;
    playing_ = true;
}

void ofxRP2A03::play(int note_number, float volume, float pan) {
    volume_ = volume;
    pan_ = pan;
    play(note_number);
}

void ofxRP2A03::stop() {
    playing_ = false;
    current_level_ = 0.0;
}

void ofxRP2A03::updateEnvelope() {
    pitch_env_index_ += program_[2] * 0.01;
    const float* pitch_env = kPitchEnvTable[program_[1]];
    // adjust pitch_env_index:
    //   if pitch_env[1] == 0, then repeat from the biginning.
    //   if pitch_env[1] == 15、then keep the last pitch.
    while (pitch_env_index_ >= pitch_env[0]) {
        pitch_env_index_ = pitch_env_index_ - pitch_env[0] + pitch_env[1];
    }
    
    // update the pitch moduration
    // (0.0078125 = 1.0 / 128)
    float exponent = pitch_env[static_cast<int>(pitch_env_index_ + 2)] *
                     program_[3] * 0.0078125;
    pitch_modulation_ = pow(2.0f, exponent);
  
    volume_env_index_ += program_[5] * 0.01;
    const float* volume_env = kVolumeEnvTable[program_[4]];
    // adjust volume_env_index:
    //   if volume_env[1] == 0, then repeat from the biginning.
    //   if volume_env[1] == 15、then keep the last volume.
    if (volume_env_index_ >= volume_env[0]) {
        volume_env_index_ = volume_env[1];
    }
    
    target_level_ = volume_env[static_cast<int>(volume_env_index_ + 2)];
}

float ofxRP2A03::processSignal() {
    // update the level
    if (current_level_ > target_level_) {
        current_level_ -= 0.01;
        if (current_level_ < 0.01) {
            playing_ = false;
            current_level_ = 0.0;
            return 0.0;
        }
    } else if (current_level_ < target_level_) {
        current_level_ += 0.01;
    }
    
    // update the phase(the index of wave table)
    phase_ += phase_increment_ * pitch_modulation_;
    if (program_[0] == 4) {
        // noise generator
        while (phase_ >= 0.25) {
            phase_ -= 0.25;
            noise_ = (ofRandomuf() >= 0.5) ? 0.25 : -0.25;
        }
        return noise_  * current_level_;
    } else {
        while (phase_ >= 1) phase_ -= 1;
        float wave = kWaveTable[program_[0]][static_cast<int>(phase_ * 16)];
        return wave * current_level_;
    }

    return 0.0;
}

void ofxRP2A03::audioOut(float* output, int buffer_size, int n_channels) {
    if (!playing_) return;

    updateEnvelope();
    
    float signal;
    float gain_left = 1.0 - pan_;
    float gain_right = pan_;
    for (int i = 0; i < buffer_size; ++i) {
        signal = processSignal();
        output[i * n_channels] += signal * volume_ * gain_left;
        output[i * n_channels + 1] += signal * volume_ * gain_right;
    }
}

//--------------------------------------------------------------
// getter/setter
//--------------------------------------------------------------
int ofxRP2A03::getNoteNumber() const {
    return note_number_;
}

void ofxRP2A03::setVolume(float volume) {
    volume_ = volume;
}

float ofxRP2A03::getVolume() const {
    return volume_;
}

void ofxRP2A03::setPan(float pan) {
    pan_ = pan;
}

float ofxRP2A03::getPan() const {
    return pan_;
}

void ofxRP2A03::setProgram(const int* program) {
    for (int i = 0; i < 6; ++i) program_[i] = program[i];
}

const int* ofxRP2A03::getProgram() const {
    return program_;
}

bool ofxRP2A03::isPlaying() const {
    return playing_;
}


//--------------------------------------------------------------
// program accessors
//--------------------------------------------------------------
void ofxRP2A03::setWaveForm(int wave_form) {
    if (wave_form < 0 || wave_form > 4) wave_form = 0;
    program_[0] = wave_form;
}

int ofxRP2A03::getWaveForm() const {
    return program_[0];
}

void ofxRP2A03::setPitchEnvType(int pitch_env_type) {
    if (pitch_env_type < 0 || pitch_env_type > 4) pitch_env_type = 0;
    program_[1] = pitch_env_type;
}

int ofxRP2A03::getPitchEnvType() const {
    return program_[1];
}

void ofxRP2A03::setPitchEnvRate(int pitch_env_rate) {
    if (pitch_env_rate < 0 || pitch_env_rate > 127) pitch_env_rate = 0;
    program_[2] = pitch_env_rate;
}

int ofxRP2A03::getPitchEnvRate() const {
    return program_[2];
}

void ofxRP2A03::setPitchEnvDepth(int pitch_env_depth) {
    if (pitch_env_depth < 0 || pitch_env_depth > 127) pitch_env_depth = 0;
    program_[3] = pitch_env_depth;
}

int ofxRP2A03::getPitchEnvDepth() const {
    return program_[3];
}

void ofxRP2A03::setVolumeEnvType(int volume_env_type) {
    if (volume_env_type < 0 || volume_env_type > 2) volume_env_type = 0;
    program_[4] = volume_env_type;
}

int ofxRP2A03::getVolumeEnvType() const {
    return program_[4];
}

void ofxRP2A03::setVolumeEnvRate(int volume_env_rate) {
    if (volume_env_rate < 0 || volume_env_rate > 127) volume_env_rate = 0;
    program_[5] = volume_env_rate;
}

int ofxRP2A03::getVolumeEnvRate() const {
    return program_[5];
}

//--------------------------------------------------------------
// signal processing variable accessors
//--------------------------------------------------------------
float ofxRP2A03::getPhase() const {
    return phase_;
}

float ofxRP2A03::getPhaseIncrement() const {
    return phase_increment_;
}

float ofxRP2A03::getCurrentLevel() const {
    return current_level_;
}

float ofxRP2A03::getTargetLevel() const {
    return target_level_;
}

float ofxRP2A03::getPitchModulation() const {
    return pitch_modulation_;
}

float ofxRP2A03::getPitchEnvIndex() const {
    return pitch_env_index_;
}

float ofxRP2A03::getVolumeEnvIndex() const {
    return volume_env_index_;
}

float ofxRP2A03::getNoise() const {
    return noise_;
}
