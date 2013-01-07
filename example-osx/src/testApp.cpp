#include "testApp.h"

//--------------------------------------------------------------
// static variables
//--------------------------------------------------------------

// Preset Program Table:
//   {"WaveForm", "PitchEnvType", "PitchEnvRate",
//    "PitchEnvDepth", "VolumeEnvType, "VolumeEnvRate"}
const int testApp::kPresetTable[kToneSize][6] = {
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

    tone_ = new ofxRP2A03();
    mouse_point_.size = 0.0;
}

//--------------------------------------------------------------
void testApp::update() {
    mouse_point_.size = tone_->getCurrentLevel() *
                        ofGetWidth() / 5;
}

//--------------------------------------------------------------
void testApp::draw() {
	int area_height = ofGetHeight() / kToneSize;
    int y = area_height;
    for (int i = 0; i < kToneSize - 1; ++i) {
        ofSetColor(32);
        ofLine(0, y, ofGetWidth(), y);
        y += area_height;
    }
    
    // draw shapes
    if (mouse_point_.size > 0.0) {
        ofSetColor(mouse_point_.color);
        ofPushMatrix();
        ofTranslate(mouse_point_.x, mouse_point_.y);
        float size = mouse_point_.size;
        if (mouse_point_.program_index == 0) {
            ofFill();
            ofCircle(0, 0, size);
        } else if (mouse_point_.program_index == 1) {
            ofFill();
            ofTriangle(-size * 0.5, size * 0.288675135,
                       0, -size * 0.577350269,
                       size * 0.5, size * 0.288675135);
        } else if (mouse_point_.program_index == 2) {
            ofFill();
            ofEllipse(0, 0, size * 2, size);
        } else if (mouse_point_.program_index == 3) {
            ofFill();
            float half = size / 2.0;
            ofRect(-half, -half, size, size);
        }
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void testApp::exit() {
    delete tone_;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
    int note_number = 36 + static_cast<float>(x) /
                      ofGetWidth() * kKeyRange;
    if (note_number != mouse_point_.note_number) {
        tone_->stop();
        
        int rect_height = ofGetHeight() / kToneSize;
        int program_index = y / rect_height;
        if (program_index == 4) program_index = 3;
        tone_->setProgram(kPresetTable[program_index]);
        
        int tone_y = static_cast<int>(y) % rect_height;
        int volume_env_rate = static_cast<float>(tone_y) / rect_height * 127;
        tone_->setVolumeEnvRate(volume_env_rate);
        
        tone_->play(note_number,
                    1.0,                  // volume
                    ofRandom(0.0, 1.0));  // pan
        
        // store touch point data
        mouse_point_.note_number = note_number;
        mouse_point_.program_index = program_index;
        mouse_point_.x = x;
        mouse_point_.y = y;
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    int note_number = 36 + static_cast<float>(x) /
                      ofGetWidth() * kKeyRange;

    int rect_height = ofGetHeight() / kToneSize;
    int program_index = y / rect_height;
    if (program_index == 4) program_index = 3;
    tone_->setProgram(kPresetTable[program_index]);
    
    int tone_y = static_cast<int>(y) % rect_height;
    int volume_env_rate = static_cast<float>(tone_y) / rect_height * 127;
    tone_->setVolumeEnvRate(volume_env_rate);
    
    tone_->play(note_number,
                1.0,                  // volume
                ofRandom(0.0, 1.0));  // pan
    
    // store touch point data
    mouse_point_.note_number = note_number;
    mouse_point_.program_index = program_index;
    mouse_point_.x = x;
    mouse_point_.y = y;
    mouse_point_.color = ofColor(ofRandom(255),
                                 ofRandom(255),
                                 ofRandom(255));
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
    tone_->stop();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo) {}

//--------------------------------------------------------------
void testApp::audioOut(float *output, int buffer_size, int n_channels) {
    // clear buffer
    for (int i = 0; i < buffer_size; ++i) {
        output[i * n_channels] = 0;
        output[i * n_channels + 1] = 0;
    }
    
    tone_->audioOut(output, buffer_size, n_channels);
}
