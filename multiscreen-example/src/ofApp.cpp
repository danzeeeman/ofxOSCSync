#include "ofApp.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::setup()
{
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
    
	ofSeedRandom();
	int uniqueID = ofRandom( 999999999 ); // Yeah this is bogus I know. Good enough for our purposes.
	client = NULL;
    
    ofxXmlSettings XML;
    bool loadedFile = XML.loadFile( "Settings/sync-settings.xml" );
    if( loadedFile )
    {
        uuid = XML.getValue("settings:client:uuid", uniqueID);
        screenIndex = XML.getValue("settings:client:index", 0);
        displayWidth = XML.getValue("settings:client:displayWidth", 1920);
        displayHeight = XML.getValue("settings:client:displayHeight", 1080);
        viewWidth = XML.getValue("settings:client:viewWidth", ofGetWidth());
        viewHeight = XML.getValue("settings:client:viewHeight", ofGetHeight());
        port = XML.getValue("settings:client:receive", 7778);
        screenOffsetX = viewWidth*screenIndex;
    }
    
    client = new ofxClientOSCManager();
    client->init( uuid, port);
    
    commonTimeOsc = client->getCommonTimeOscObj();
    commonTimeOsc->setEaseOffset( true );
    
    ofAddListener( client->newDataEvent, this, &ofApp::newData );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::update()
{
    currTime = 0.0f;
    currTime = commonTimeOsc->getTimeSecs();
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::draw()
{
    
    float currTime = 0.0f;
    
    currTime = commonTimeOsc->getTimeSecs();
    
    
    
	// Set a color that pulsates based on the time we get
	ofColor bgColor;
	bgColor.setHsb( ((cosf(currTime/10.0f)+1.0f)/2.0f) * 255, 180, ((cosf(currTime*1.4f)+1.0f)/2.0f) * 255 );
	ofSetColor(bgColor);
	ofRect(0,0,ofGetWidth(), ofGetHeight() );
    
	// Rotate a circle
	ofColor circleColor = bgColor.getInverted();
	ofSetColor(circleColor);
	ofPushMatrix();
    ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f );
    ofRotate( currTime * 50.0f );
    ofTranslate( ofGetHeight() * 0.45f, 0 );
    ofCircle( 0, 0, 40 );
	ofPopMatrix();
	
	ofSetColor(255);
	
    
    fontSmall.drawString( "Offset: " + ofToString(commonTimeOsc->offsetMillis) + " OffsetTarget: " + ofToString(commonTimeOsc->offsetMillisTarget), 80, 80 );

    
    fontSmall.drawString( "Offset: " + ofToString(commonTimeOsc->offsetMillis) + " OffsetTarget: " + ofToString(commonTimeOsc->offsetMillisTarget), 80, 80 );
    
    fontSmall.drawString( "Screen: " + ofToString(screenIndex) + "  Time: " + ofToString( currTime, 2), 80, 45 );
    
    fontSmall.drawString( "uuid: " + ofToString(uuid) + " port: " + ofToString(port), 80, 120 );
    
    if(client->isReady()){
        fontSmall.drawString( "client: " + client->getServerIP(), 80, 160);
    }
	
}

void ofApp::newData( DataPacket& _packet  )
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



// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::keyPressed(int key)
{
    
    DataPacket data;
    int clientID;
    string command;
    float value;
    if(key == '1'){
        clientID = 0;
        command = "play";
        value = 100.00;
    }
    if(key == '2'){
        clientID = 1;
        command = "play";
        value = 50.00;
        
    }
    if(key == '3'){
        clientID = 2;
        command = "play";
        value = 25.00;
        
    }
    if(key == '4'){
        clientID = 3;
        command = "play";
        value = 15.00;
        
    }
    if(key == '5'){
        clientID = 4;
        command = "play";
        value = 5.00;
        
    }
    if(key == '6'){
        clientID = 5;
        command = "play";
        value = 2.50;
    }
    
    data.valuesFloat.push_back(value);
    data.valuesInt.push_back(clientID);
    data.valuesString.push_back(command);
    
    client->sendData(data.valuesString, data.valuesInt, data.valuesFloat);
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::keyReleased(int key)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::mouseMoved(int x, int y )
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::mouseDragged(int x, int y, int button)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::mousePressed(int x, int y, int button)
{
    DataPacket packet;
    packet.valuesString.push_back("mouseClicked");
    client->sendData(packet);
    
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::mouseReleased(int x, int y, int button)
{
}
