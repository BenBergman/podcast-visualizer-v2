#pragma once
#include "ofMain.h"
#include "ofxSoundObject.h"
class faceDraw: public ofRectangle, public ofxSoundObject{
   public:
      faceDraw(){}
      faceDraw(const faceDraw& a):ofRectangle(a), ofxSoundObject(a){
      }
      ~faceDraw(){}
      //--------------------------------------------------------------
      void setup(float x, float y, float w, float h){
         this->set(x, y, w, h);
      }
      //--------------------------------------------------------------
      void setFace(string facePath, string faceGrayPath){
         face.load(facePath);
         faceGray.load(faceGrayPath);
      }
      //--------------------------------------------------------------
      void process(ofSoundBuffer &input, ofSoundBuffer &output) {
         if(input.size()!=buffer.size()) {
            ofLogVerbose("faceDraw") << "working buffer size != output buffer size.";
            buffer.resize(input.size());
            buffer.setNumChannels(input.getNumChannels());
            buffer.setSampleRate(input.getSampleRate());
         }

         mutex1.lock();
         input.copyTo(buffer);
         mutex1.unlock();
         input.copyTo(output);
      }
      //--------------------------------------------------------------
      void update(){
         if (buffer.size() >0) {
            vector<ofMesh>meshes;
            chans = buffer.getNumChannels();
            for (int i = 0; i < chans; i++) {
               meshes.push_back(ofMesh());
               meshes.back().setMode(OF_PRIMITIVE_LINE_STRIP);
            }

            float h = this->height / float(chans);
            //float h2 = h * 0.5f;
            //            float factor= this->width / buffer.getNumFrames();

            float xInc = this->getWidth()/(float)(buffer.getNumFrames() -1);
            ofVec3f v;
            v.x = x;
            for(int i=0; i<buffer.getNumFrames(); i++){

               //v.x = ofMap(i, 0, buffer.getNumFrames() -1 , x, getMaxX());
               for (int j = 0; j < chans; j++) {
                  v.y = ofMap(buffer[i*chans + j], -1, 1, h*(j+1) + y, h*j + y );
                  //     meshes[j].addColor(ofFloatColor::pink);
                  meshes[j].addVertex(v);
               }
               v.x += xInc;
            }
            this->meshes = meshes;


            curVol = 0;
            curVol2 = 0;
            for (int i = 0; i < buffer.getNumFrames(); i++) {
               curVol += buffer[i * chans] * buffer[i * chans];
               curVol2 += ofMap(buffer[i * chans] * buffer[i * chans], 0.005, 1, 0, 1, true);
            }


            { // raw volumes, no suppressing bleed over
               curVol /= buffer.getNumFrames();
               curVol = sqrt(curVol);

               smoothedVol *= 0.90;
               smoothedVol += 0.10 * curVol;
            }


            { // supress quiet noises, likely bleed over from neighbours' mics
               curVol2 /= buffer.getNumFrames();
               curVol2 = sqrt(curVol2);

               smoothedVol2 *= 0.95;
               smoothedVol2 += 0.05 * curVol2;
            }
         }
      }
      //--------------------------------------------------------------
      void draw(){
         float scaling = ofMap(smoothedVol2, 0, 1, 0.75, 3);
         float faceWidth = face.getWidth() * scaling;
         float faceHeight = face.getHeight() * scaling;
         ofSetColor(255);
         faceGray.draw(x + this->width/2 - faceWidth/2, y + this->height/2 - faceHeight/2, faceWidth, faceHeight);
         ofSetColor(255, 255, 255, ofMap(smoothedVol2, 0, 0.2, 0, 255, true));
         face.draw(x + this->width/2 - faceWidth/2, y + this->height/2 - faceHeight/2, faceWidth, faceHeight);
         ofSetColor(255);
      }
      //--------------------------------------------------------------
      void drawDebug(){
         for (int i = 0; i < chans; i++) {
            meshes[i].draw();
         }


         { // raw volumes, no suppressing bleed over
            ofSetColor(255, 0, 0);
            ofDrawRectangle(x, this->height, 10, -curVol * this->height);
            ofDrawRectangle(x + 15, this->height, 10, -smoothedVol * this->height);
            ofSetColor(255);
         }


         { // supress quiet noises, likely bleed over from neighbours' mics
            ofSetColor(0, 0, 255);
            ofDrawRectangle(x + 30, this->height, 10, -curVol2 * this->height);
            ofDrawRectangle(x + 30 + 15, this->height, 10, -smoothedVol2 * this->height);
            ofSetColor(255);
         }
      }
      //--------------------------------------------------------------

      ofSoundBuffer buffer;
      mutable ofMutex mutex1;
      vector<ofMesh>meshes;
      int chans;
      float curVol;
      float curVol2;
      float smoothedVol;
      float smoothedVol2;
      ofImage face;
      ofImage faceGray;
};
