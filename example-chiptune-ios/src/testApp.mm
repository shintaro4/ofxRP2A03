#include "testApp.h"

//--------------------------------------------------------------
// static variables
//--------------------------------------------------------------

// MIDI note numbers
const int testApp::kNoteNumbers[kPolyphonyVoices][kSongLength] = {
  // NOTE: -1 indicates a rest.
  
  { // melody
    74,74,-1,72, -1,71,-1,-1, -1,-1,-1,62, -1,64,65,67,
    69,-1,-1,62, -1,-1,69,-1, 67,72,-1,-1, -1,-1,-1,-1,
    74,74,-1,72, -1,71,-1,-1, -1,-1,-1,62, -1,64,65,67,
    69,-1,-1,62, -1,-1,69,-1, 67,60,-1,-1, -1,-1,-1,-1,
    -1,-1,74,-1, -1,81,-1,-1, 80,81,80,77, -1,-1,-1,-1,
    81,-1,81,80, -1,80,79,-1, 81,-1,81,80, -1,80,79,-1,
    -1,-1,74,-1, -1,81,-1,-1, 80,81,80,77, -1,-1,-1,-1,
    81,-1,81,80, -1,80,79,-1, 81,-1,81,80, -1,80,79,-1,
    73,-1,-1,76, -1,-1,82,-1, 81,-1,-1,77, -1,-1,74,-1,
    73,-1,-1,76, -1,-1,82,-1, 81,-1,-1,74, -1,-1,-1,-1,
    73,-1,-1,76, -1,-1,82,-1, 81,-1,-1,77, -1,-1,74,-1,
    76,-1,-1,79, -1,-1,82,-1, 81,-1,-1,83, -1,-1,85,-1,
    86,86,74,74, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    58,-1,58,-1, 62,65,-1,-1, 60,-1,60,-1, 64,67,-1,-1,
    86,86,74,74, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    58,-1,58,-1, 62,65,-1,-1, 60,-1,-1,-1, -1,-1,-1,-1
  },
  
  { // bass
    38,-1,38,38, 38,-1,-1,38, 38,-1,38,38, 38,38,38,38,
    34,-1,34,34, 46,-1,-1,-1, 36,36,48,36, 36,36,36,36,
    38,-1,38,38, 38,-1,-1,38, 38,-1,38,38, 38,38,38,38,
    34,-1,34,34, 46,-1,-1,-1, 36,36,48,36, 36,36,36,36,
    38,-1,38,38, 38,-1,-1,38, 38,-1,38,38, 38,38,38,38,
    34,-1,34,34, 46,34,-1,34, -1,34,46,-1, 46,-1,46,-1,
    38,-1,38,38, 38,-1,-1,38, 38,-1,38,38, 38,38,38,38,
    34,-1,34,34, 46,34,-1,34, -1,34,46,-1, 46,-1,46,-1,
    37,-1,-1,-1, -1,-1,-1,-1, 38,-1,-1,-1, -1,-1,-1,-1,
    40,-1,-1,-1, -1,-1,-1,-1, 38,-1,-1,-1, -1,-1,-1,-1,
    37,-1,-1,-1, -1,-1,-1,-1, 38,-1,-1,-1, -1,-1,-1,-1,
    40,-1,-1,-1, -1,-1,-1,-1, 45,-1,-1,-1, -1,-1,-1,-1,
    38,-1,38,-1, -1,38,36,-1, 38,-1,-1,-1, -1,-1,-1,-1,
    34,-1,34,-1, -1,-1,-1,-1, 36,-1,36,-1, -1,-1,-1,-1,
    38,-1,38,-1, -1,38,36,-1, 38,-1,-1,-1, -1,-1,-1,-1,
    34,-1,34,-1, -1,-1,-1,-1, 36,-1,36,-1, 33,36,-1,-1
  }
};

// Preset Program Table:
//   {"WaveForm", "PitchEnvType", "PitchEnvRate",
//    "PitchEnvDepth", "VolumeEnvType, "VolumeEnvRate"}
const int testApp::kPresetTable[kProgramSize][6] = {
  {1, 0, 0, 0, 1, 24},
  {2, 4, 100, 1, 2, 24},
  {0, 1, 100, 2, 1, 24}
};

const ofColor testApp::kColorList[kProgramSize] = {
  ofColor(255, 0, 0),
  ofColor(0, 160, 64),
  ofColor(255, 128, 48)
};

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(60);
	ofBackground(191);

  counter_ = 0;
  touch_idx_ = 0;

  for (int i = 0; i < kPolyphonyVoices; ++i) tones_[i] = new ofxRP2A03();
  setProgram();
  
  ofSoundStreamSetup(2, 0, this, kAudioSampleRate, kAudioBufferSize, 4);
  TimerHandler::getInstance().init((Poco::TimerCallback<testApp>
                                    (*this, &testApp::onTimer)));

}

//--------------------------------------------------------------
void testApp::update() {}

//--------------------------------------------------------------
void testApp::draw() {
	int rect_height = ofGetHeight() / kProgramSize;
  int y = 0;
  for (int i = 0; i < kProgramSize; ++i) {
    if (i == touch_idx_) {
      ofFill();
      ofSetColor(kColorList[touch_idx_]);
    } else {
      ofNoFill();
      ofSetColor(63);
    }
    ofRect(0, y, ofGetWidth(), rect_height);
    y += rect_height;
  }
}

//--------------------------------------------------------------
void testApp::exit(){
  TimerHandler::getInstance().deallocate();
  for (int i = 0; i < kPolyphonyVoices; ++i) delete tones_[i];
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch) {
  int rect_height = ofGetHeight() / kProgramSize;
  int touch_idx = touch.y / rect_height;
  if (touch_idx == kProgramSize) touch_idx = kProgramSize - 1;
  touch_idx_ = touch_idx;
  
  setProgram();
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch) {}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch) {}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch) {}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch) {}

//--------------------------------------------------------------
void testApp::lostFocus() {}

//--------------------------------------------------------------
void testApp::gotFocus() {}

//--------------------------------------------------------------
void testApp::gotMemoryWarning() {}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation) {}

//--------------------------------------------------------------
void testApp::audioOut(float *output, int buffer_size, int n_channels) {
  // clear buffer
  for (int i = 0; i < buffer_size; ++i) {
    output[i * n_channels] = 0;
    output[i * n_channels + 1] = 0;
  }
  
  for (int i = 0; i < kPolyphonyVoices; ++i) {
    tones_[i]->audioOut(output, buffer_size, n_channels);
  }
}

//--------------------------------------------------------------
void testApp::onTimer(Poco::Timer &timer) {
  TimerHandler& timer_handler = TimerHandler::getInstance();
  if (timer_handler.isNextStepTime()) {
    timer_handler.incrementNextStepTime();
    
    for (int i = 0; i < kPolyphonyVoices; ++i) {
      int note_number = kNoteNumbers[i][counter_];
      if (note_number > 0) {
        tones_[i]->play(note_number);
      }
    }
    
    counter_ = (counter_ + 1) % kSongLength;
  }
}

//--------------------------------------------------------------
void testApp::setProgram() {
  for (int i = 0; i < kPolyphonyVoices; ++i) {
    tones_[i]->setProgram(kPresetTable[touch_idx_]);
  }
}
