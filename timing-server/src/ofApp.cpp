#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(4, 5, 6);
    ofSetVerticalSync(true);
    fontSmall.loadFont("Fonts/DIN.otf", 8 );

    server = new ofxServerOscManager();
    
    ofxXmlSettings XML;
    
    bool loadedFile = XML.loadFile( "Settings/sync-settings.xml" );
    if( loadedFile )
    {
        sendPort = XML.getValue("settings:server:send", 7778);
        receivePort = XML.getValue("settings:server:receive", 7777);
        host = XML.getValue("settings:server:host", "127.0.0.1");
    }
    
    server->init(host, sendPort, receivePort);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    float currTime = ofGetElapsedTimef();
    // Set a color that pulsates based on the time we get
	ofColor bgColor;
	bgColor.setHsb( ((cosf(currTime/10.0f)+1.0f)/2.0f) * 255, 180, ((cosf(currTime*1.4f)+1.0f)/2.0f) * 255 );
	ofSetColor(bgColor);
	ofRect(0,0,ofGetWidth(), ofGetHeight() );

    ofSetColor(255, 255, 255);
    
    fontSmall.drawString( "CurrentTime: " + ofToString(currTime), 80, 80 );
    
    fontSmall.drawString( "host: " + ofToString(host) + " sendPort: " + ofToString(sendPort), 80, 120 );
    
    fontSmall.drawString( "receivePort: " + ofToString(receivePort), 80, 160);
    
    
}
