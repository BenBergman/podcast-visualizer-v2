#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    int bufferSize = 512;
    sampleRate = 44100;
    channels = 2;

    recordFbo.allocate(1920, 1080, GL_RGB);

    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);

    fileName = "testMovie";
    fileExt = ".mov"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats

    // override the default codecs if you like
    // run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("800k");
    vidRecorder.setAudioCodec("mp3");
    vidRecorder.setAudioBitrate("192k");

    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);

    ofDirectory dir;
    dir.listDir("");

    ofRectangle r1(0, 0, ofGetWidth()/3, ofGetHeight());
    r1.scaleFromCenter(0.95);
    benFace.set(r1);

    benVoice.load(dir.getAbsolutePath() + "/ben.wav");
    benVoice.play();
    benVoice.connectTo(benFace).connectTo(mixer);

    ofRectangle r2(ofGetWidth()/3, 0, ofGetWidth()/3, ofGetHeight());
    r2.scaleFromCenter(0.95);
    danFace.set(r2);

    danVoice.load(dir.getAbsolutePath() + "/dan.wav");
    danVoice.play();
    danVoice.connectTo(danFace).connectTo(mixer);

    ofRectangle r3(ofGetWidth()/3*2, 0, ofGetWidth()/3, ofGetHeight());
    r3.scaleFromCenter(0.95);
    mattFace.set(r3);

    mattVoice.load(dir.getAbsolutePath() + "/matt.wav");
    mattVoice.play();
    mattVoice.connectTo(mattFace).connectTo(mixer);

//    soundStream.listDevices();
//    soundStream.setDeviceID(11);
    soundStream.setup(this, 2, channels, sampleRate, bufferSize, 4);
    soundStream.setOutput(mixer);

    bRecording = false;
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    {
        recordFbo.begin();
        ofClear(0, 0, 0, 255);
        ofSetColor(255);
        benFace.draw();
        danFace.draw();
        mattFace.draw();
        recordFbo.end();
        if(bRecording){
            recordFbo.readToPixels(recordPixels);

            bool success = vidRecorder.addFrame(recordPixels);
            if (!success) {
                ofLogWarning("This frame was not added!");
            }
        }

        // Check if the video recorder encountered any error while writing video frame or audio smaples.
        if (vidRecorder.hasVideoError()) {
            ofLogWarning("The video recorder failed to write some frames!");
        }

        if (vidRecorder.hasAudioError()) {
            ofLogWarning("The video recorder failed to write some audio samples!");
        }
    }


    ofSetColor(255, 255, 255);
    recordFbo.draw(0,0);

    stringstream ss;
    ss //<< "video queue size: " << vidRecorder.getVideoQueueSize() << endl
    //<< "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
    << "FPS: " << ofGetFrameRate() << endl
    << (bRecording?"pause":"start") << " recording: r" << endl
    << (bRecording?"close current video file: c":"") << endl;

    ofSetColor(0,0,0,100);
    ofDrawRectangle(0, 0, 260, 75);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ss.str(),15,15);

    if(bRecording){
    ofSetColor(255, 0, 0);
    ofDrawCircle(ofGetWidth() - 20, 20, 5);
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
    if(bRecording)
        vidRecorder.addAudioSamples(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if(key=='r'){
        bRecording = !bRecording;
        if(bRecording && !vidRecorder.isInitialized()) {
            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, recordFbo.getWidth(), recordFbo.getHeight(), 30, sampleRate, channels);

            // Start recording
            vidRecorder.start();
        }
        else if(!bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(true);
        }
        else if(bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(false);
        }
    }
    if(key=='c'){
        bRecording = false;
        vidRecorder.close();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
