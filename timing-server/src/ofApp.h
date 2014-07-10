#pragma once

#include "ofMain.h"
#include "ofxOSCSync.h"
class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    long long mLongTime;
    float mTime;
    ofxServerOscManager* server;
    ofxCommonTimeOSC* time;
    ofTrueTypeFont fontSmall;
    int sendPort;
    int receivePort;
    string host;
};
