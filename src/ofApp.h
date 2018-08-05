#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"
#include "ofxSoundPlayerObject.h"
#include "faceDraw.h"

#define WIDTH 1920
#define HEIGHT 1080

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioIn(float * input, int bufferSize, int nChannels);

    ofxVideoRecorder    vidRecorder;
    ofSoundStream       soundStream;
    bool bRecording;
    int sampleRate;
    int channels;
    string fileName;
    string fileExt;

    void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);

    ofFbo recordFbo;
    ofPixels recordPixels;

    ofSoundStream stream;
    ofxSoundMixer mixer;
    ofxSoundPlayerObject benVoice;
    ofxSoundPlayerObject danVoice;
    ofxSoundPlayerObject mattVoice;
    faceDraw benFace;
    faceDraw danFace;
    faceDraw mattFace;
};
