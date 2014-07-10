#pragma once

#include <set>
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOSCSync.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class ofApp : public ofBaseApp
{
    
public:
    
    void				setup();
    void				update();
    void				draw();
    
    void				keyPressed(int key);
    void				keyReleased(int key);
    void				mouseMoved(int x, int y );
    void				mouseDragged(int x, int y, int button);
    void				mousePressed(int x, int y, int button);
    void				mouseReleased(int x, int y, int button);
    
    bool				isServer;
    void                newData( DataPacket& _packet  );
    
    ofxCommonTimeOSC*		commonTimeOsc;
    ofxClientOSCManager*   client;
    ofxServerOscManager*   server;
	
    ofTrueTypeFont		fontSmall;
    float               currTime;
    int                 port;
    int                 uuid;
    int                 screenIndex;
    int                 displayWidth;
    int                 displayHeight;
    int                 viewWidth;
    int                 viewHeight;
    int                 screenOffsetX;
    int                 screenOffsetY;
    
};
