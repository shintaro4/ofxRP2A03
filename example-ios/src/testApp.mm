#include "testApp.h"

//--------------------------------------------------------------
// static variables
//--------------------------------------------------------------

// Preset Program Table:
//   {"WaveForm", "PitchEnvType", "PitchEnvRate",
//    "PitchEnvDepth", "VolumeEnvType, "VolumeEnvRate"}
const int testApp::kPresetTable[kPolyphonyVoices][6] = {
    {0, 2, 25, 100, 1, 25},
    {3, 0, 50, 100, 0, 50},
    {1, 0, 25, 100, 2, 25},
    {2, 0, 50,   0, 0, 50}
};

//--------------------------------------------------------------
void testApp::setup() {	
	ofSetFrameRate(60);
	ofBackground(191);
    
    ofSoundStreamSetup(2, 0, this, kAudioSampleRate, kAudioBufferSize, 4);
    
    for (int i = 0; i < kPolyphonyVoices; ++i) {
        tones_[i] = new ofxRP2A03();
        touch_points_[i].size = 0.0;
    }
}

//--------------------------------------------------------------
void testApp::update() {
    for (int i = 0; i < kPolyphonyVoices; ++i) {
        touch_points_[i].size = tones_[i]->getCurrentLevel() *
                                ofGetWidth() / 3;
    }
}

//--------------------------------------------------------------
void testApp::draw() {
	int area_height = ofGetHeight() / kPolyphonyVoices;
    int y = area_height;
    for (int i = 0; i < kPolyphonyVoices - 1; ++i) {
        ofSetColor(32);
        ofLine(0, y, ofGetWidth(), y);
        y += area_height;
    }
    
    // draw shapes
    for (int i = 0; i < kPolyphonyVoices; ++i) {
        if (touch_points_[i].size > 0) {
            ofSetColor(touch_points_[i].color);
            ofPushMatrix();
            ofTranslate(touch_points_[i].x, touch_points_[i].y);
            float size = touch_points_[i].size;
            if (touch_points_[i].program_index == 0) {
                ofFill();
                ofCircle(0, 0, size);
            } else if (touch_points_[i].program_index == 1) {
                ofFill();
                ofTriangle(-size * 0.5, size * 0.288675135,
                           0, -size * 0.577350269,
                           size * 0.5, size * 0.288675135);
            } else if (touch_points_[i].program_index == 2) {
                ofFill();
                ofEllipse(0, 0, size * 2, size);
            } else if (touch_points_[i].program_index == 3) {
                ofFill();
                float half = touch_points_[i].size / 2.0;
                ofRect(-half, -half, size, size);
            }
            ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void testApp::exit(){
    for (int i = 0; i < kPolyphonyVoices; ++i) delete tones_[i];
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch) {
    if (touch.id >= kPolyphonyVoices) return;
    
    int note_number = 36 + touch.x / ofGetWidth() * kKeyRange;
    
    int rect_height = ofGetHeight() / kPolyphonyVoices;
    int program_index = touch.y / rect_height;
    if (program_index == 4) program_index = 3;
    tones_[touch.id]->setProgram(kPresetTable[program_index]);
    
    int tone_y = static_cast<int>(touch.y) % rect_height;
    int volume_env_rate = static_cast<float>(tone_y) / rect_height * 127;
    tones_[touch.id]->setVolumeEnvRate(volume_env_rate);
    
    tones_[touch.id]->play(note_number,
                           1.0,                  // volume
                           ofRandom(0.0, 1.0));  // pan
    
    // store touch point data
    touch_points_[touch.id].note_number = note_number;
    touch_points_[touch.id].program_index = program_index;
    touch_points_[touch.id].x = touch.x;
    touch_points_[touch.id].y = touch.y;
    touch_points_[touch.id].color = ofColor(ofRandom(255),
                                            ofRandom(255),
                                            ofRandom(255));
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch) {
    if (touch.id >= kPolyphonyVoices) return;
    
    int note_number = 36 + touch.x / ofGetWidth() * kKeyRange;
    if (note_number != touch_points_[touch.id].note_number) {
        tones_[touch.id]->stop();

        int rect_height = ofGetHeight() / kPolyphonyVoices;
        int program_index = touch.y / rect_height;
        if (program_index == 4) program_index = 3;
        tones_[touch.id]->setProgram(kPresetTable[program_index]);
        
        int tone_y = static_cast<int>(touch.y) % rect_height;
        int volume_env_rate = static_cast<float>(tone_y) / rect_height * 127;
        tones_[touch.id]->setVolumeEnvRate(volume_env_rate);
        
        tones_[touch.id]->play(note_number,
                               1.0,                  // volume
                               ofRandom(0.0, 1.0));  // pan
        
        // store touch point data
        touch_points_[touch.id].note_number = note_number;
        touch_points_[touch.id].program_index = program_index;
        touch_points_[touch.id].x = touch.x;
        touch_points_[touch.id].y = touch.y;
    }
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch) {
    if (touch.id >= kPolyphonyVoices) return;
    
    tones_[touch.id]->stop();
}

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

