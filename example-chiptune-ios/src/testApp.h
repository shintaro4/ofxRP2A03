#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "ofxRP2A03.h"

#include "Poco/Timer.h"
#include "TimerHandler.h"

class testApp : public ofxiPhoneApp {
	
public:
  void setup();
  void update();
  void draw();
  void exit();
	
  void touchDown(ofTouchEventArgs& touch);
  void touchMoved(ofTouchEventArgs& touch);
  void touchUp(ofTouchEventArgs& touch);
  void touchDoubleTap(ofTouchEventArgs& touch);
  void touchCancelled(ofTouchEventArgs& touch);
  
  void lostFocus();
  void gotFocus();
  void gotMemoryWarning();
  void deviceOrientationChanged(int newOrientation);
  
  void audioOut(float* output, int buffer_size, int n_channels);
  void onTimer(Poco::Timer& timer);
  
private:
  
  static const int kAudioSampleRate = 44100;
  static const int kAudioBufferSize = 1024;
  
  static const int kPolyphonyVoices = 2;
  static const int kSongLength = 256;
  static const int kNoteNumbers[kPolyphonyVoices][kSongLength];

  static const int kProgramSize = 3;
  static const int kPresetTable[kProgramSize][6];

  static const ofColor kColorList[kProgramSize];
  
  void setProgram();
  
  int counter_;
  ofxRP2A03* tones_[kPolyphonyVoices];
  int touch_idx_;
  
};


