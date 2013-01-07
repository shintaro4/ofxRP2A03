/*
 *  ofxRP2A03
 *  A Famicom(NES) sound synthesizer for openFrameworks.
 *
 *  Created by Shintaro Matsushima on 29/12/2012.
 *	Thanks to g200kg.
 *	Special thanks to mohayonao.
 *
 *  This addon is based on WebBeeper 2A03.
 *
 *  WebBeeper 2A03 Polyphonic Beeper for WebMidiLink
 *  URL: http://www.g200kg.com/en/docs/webbeeper
 *  Author: g200kg
 *
 *  CommentedWebBeeper.js
 *  URL: https://gist.github.com/3094681
 *  Author: mohayonao
 *
 *
 *  Copyright (c) 2013 Shintaro Matsushima
 *
 *	Permission is hereby granted, free of charge, to any person
 *	obtaining a copy of this software and associated documentation
 *	files (the "Software"), to deal in the Software without
 *	restriction, including without limitation the rights to use,
 *	copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the
 *	Software is furnished to do so, subject to the following
 *	conditions:
 *	
 *	The above copyright notice and this permission notice shall be
 *	included in all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,	
 *	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *	OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#include <math.h>
#include "ofMain.h"

class ofxRP2A03 {
    
public:

    static void setAudioSampleRate(int audio_sample_rate);
    
    ofxRP2A03();
    ofxRP2A03(const int* program);

    void play(int note_number);
    void play(int note_number, float volume, float pan);
    void stop();
    void updateEnvelope();
    float processSignal();
    void audioOut(float* output, int buffer_size, int n_channels);
    
    int getNoteNumber() const;
    void setVolume(float volume);
    float getVolume() const;
    void setPan(float pan);
    float getPan() const;
    void setProgram(const int* program);
    const int* getProgram() const;
    bool isPlaying() const;
    
    void setWaveForm(int wave_form);
    int getWaveForm() const;
    void setPitchEnvType(int pitch_evn_type);
    int getPitchEnvType() const;
    void setPitchEnvRate(int pitch_evn_rate);
    int getPitchEnvRate() const;
    void setPitchEnvDepth(int pitch_evn_depth);
    int getPitchEnvDepth() const;
    void setVolumeEnvType(int volume_evn_type);
    int getVolumeEnvType() const;
    void setVolumeEnvRate(int volume_evn_rate);
    int getVolumeEnvRate() const;
    
    float getPhase() const;
    float getPhaseIncrement() const;
    float getCurrentLevel() const;
    float getTargetLevel() const;
    float getPitchModulation() const;
    float getPitchEnvIndex() const;
    float getVolumeEnvIndex() const;
    float getNoise() const;
    
private:
    
    static float kAudioSampleRatef;
    static const float kWaveTable[4][16];
    static const float kPitchEnvTable[5][18];
    static const float kVolumeEnvTable[3][18];
    
    int note_number_;
    float pan_;
    float volume_;
    bool playing_;
    
    // program_[6]:
    //   {"WaveForm", "PitchEnvType", "PitchEnvRate",
    //    "PitchEnvDepth", "VolumeEnvType, "VolumeEnvRate"}
    int program_[6];

    float phase_;
    float phase_increment_;
    float current_level_;
    float target_level_;
    float pitch_modulation_;
    float pitch_env_index_;
    float volume_env_index_;
    float noise_; // noise generator value
    
};
