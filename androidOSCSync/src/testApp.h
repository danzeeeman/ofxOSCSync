#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxAccelerometer.h"
#include "ofxOSCSync.h"

class testApp : public ofxAndroidApp{
	
	public:
		
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

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
