#pragma once

#include "ofMain.h"

#include "ofxRP2A03.h"

struct MousePoint {
    int note_number;
    int program_index;
    float x;
    float y;
    float size;
    ofColor color;
};

class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void audioOut(float* output, int buffer_size, int n_channels);
    
private:
    
    static const int kToneSize = 4;
    static const int kPresetTable[kToneSize][6];
    static const int kKeyRange = 48;
    static const int kAudioSampleRate = 44100;
    static const int kAudioBufferSize = 1024;
    
    ofxRP2A03* tone_;
    struct MousePoint mouse_point_;

};
