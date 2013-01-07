#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "ofxRP2A03.h"

struct TouchPoint {
    int note_number;
    int program_index;
    float x;
    float y;
    float size;
    ofColor color;
};

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

private:

    static const int kPolyphonyVoices = 4;
    static const int kPresetTable[kPolyphonyVoices][6];
    static const int kKeyRange = 48;
    static const int kAudioSampleRate = 44100;
    static const int kAudioBufferSize = 1024;
    
    ofxRP2A03* tones_[kPolyphonyVoices];
    struct TouchPoint touch_points_[kPolyphonyVoices];
    
};


