#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	// initialize the accelerometer
	ofSeedRandom();
	int uniqueID = ofRandom(999999999); // Yeah this is bogus I know. Good enough for our purposes.
	client = NULL;

	ofxXmlSettings XML;
	bool loadedFile = XML.loadFile("Settings/sync-settings.xml");
	if (loadedFile) {
		uuid = XML.getValue("settings:client:uuid", uniqueID);
		screenIndex = XML.getValue("settings:client:index", 0);
		displayWidth = XML.getValue("settings:client:displayWidth", 1920);
		displayHeight = XML.getValue("settings:client:displayHeight", 1080);
		viewWidth = XML.getValue("settings:client:viewWidth", ofGetWidth());
		viewHeight = XML.getValue("settings:client:viewHeight", ofGetHeight());
		port = XML.getValue("settings:client:receive", 7776);
		screenOffsetX = viewWidth * screenIndex;
	}else{
		port = 7776;
		uuid = uniqueID;
	}

	client = new ofxClientOSCManager();
	client->init(uuid, port);

	commonTimeOsc = client->getCommonTimeOscObj();
	commonTimeOsc->setEaseOffset(true);

	ofAddListener(client->newDataEvent, this, &testApp::newData);
}

//--------------------------------------------------------------
void testApp::update() {

}

//--------------------------------------------------------------
void testApp::draw() {
	float currTime = 0.0f;

	currTime = commonTimeOsc->getTimeSecs();

	// Set a color that pulsates based on the time we get
	ofColor bgColor;
	bgColor.setHsb(((cosf(currTime / 10.0f) + 1.0f) / 2.0f) * 255, 180,
			((cosf(currTime * 1.4f) + 1.0f) / 2.0f) * 255);
	ofSetColor(bgColor);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());

	// Rotate a circle
	ofColor circleColor = bgColor.getInverted();
	ofSetColor(circleColor);
	ofPushMatrix();
	ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
	ofRotate(currTime * 50.0f);
	ofTranslate(ofGetHeight() * 0.45f, 0);
	ofCircle(0, 0, 40);
	ofPopMatrix();

	ofSetColor(255);

	ofDrawBitmapString(ofToString(currTime), 100, 100);

}

void testApp::newData( DataPacket& _packet  )
{

    ofLog()<<"new data"<<endl;

    for(int i = 0; i < _packet.valuesFloat.size(); i++){
        ofLog()<<_packet.valuesFloat[i]<<endl;
    }
    for(int i = 0; i < _packet.valuesInt.size(); i++){
        ofLog()<<_packet.valuesInt[i]<<endl;
    }
    for(int i = 0; i < _packet.valuesString.size(); i++){
        ofLog()<<_packet.valuesString[i]<<endl;
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void testApp::touchDown(int x, int y, int id) {

}

//--------------------------------------------------------------
void testApp::touchMoved(int x, int y, int id) {

}

//--------------------------------------------------------------
void testApp::touchUp(int x, int y, int id) {

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(int x, int y, int id) {

}

//--------------------------------------------------------------
void testApp::touchCancelled(int x, int y, int id) {

}

//--------------------------------------------------------------
void testApp::swipe(ofxAndroidSwipeDir swipeDir, int id) {

}

//--------------------------------------------------------------
void testApp::pause() {

}

//--------------------------------------------------------------
void testApp::stop() {

}

//--------------------------------------------------------------
void testApp::resume() {
	ofBackground(0, 0, 0);
}

//--------------------------------------------------------------
void testApp::reloadTextures() {
	ofBackground(0, 0, 0);
}

//--------------------------------------------------------------
bool testApp::backPressed() {
	return false;
}

//--------------------------------------------------------------
void testApp::okPressed() {

}

//--------------------------------------------------------------
void testApp::cancelPressed() {

}
