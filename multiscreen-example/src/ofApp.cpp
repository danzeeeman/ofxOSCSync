#include "ofApp.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::setup()
{
	isServer = false;
	
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
    
	ofSeedRandom();
	int uniqueID = ofRandom( 999999999 ); // Yeah this is bogus I know. Good enough for our purposes.
    server = NULL;
	client = NULL;
	
    if( ofFile( ofToDataPath("Settings/IsServer.txt")).exists() )
	{
		server = new ofxServerOscManager();
		server->init( "Settings/ServerSettings.xml" );
        
		isServer = server->isInitialised();
        ofAddListener( server->newDataEvent, this, &ofApp::newData );
        cout<<"Starting Server"<<endl;
	}
	else
	{
        ofxXmlSettings XML;
        bool loadedFile = XML.loadFile( "Settings/ClientSettings.xml" );
        if( loadedFile )
        {
            screenIndex = XML.getValue("Settings:ScreenIndex", 0);
            displayWidth = XML.getValue("Settings:DisplayWidth", 5120);
            displayHeight = XML.getValue("Settings:DisplayHeight", 2880);
            viewWidth = XML.getValue("Settings:ViewWidth", ofGetWidth());
            viewHeight = XML.getValue("Settings:ViewHeight", ofGetHeight());
            screenOffsetX = viewWidth*screenIndex;
        }
        
		client = new ofxClientOSCManager();
		client->init( screenIndex );
		
		commonTimeOsc = client->getCommonTimeOscObj();
		commonTimeOsc->setEaseOffset( true );
		
		ofAddListener( client->newDataEvent, this, &ofApp::newData );
	}
    
    // Read the screen index from a file
    
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::update()
{
    currTime = 0.0f;
    if( isServer ) { currTime = ofGetElapsedTimef(); } else { currTime = commonTimeOsc->getTimeSecs(); }
    
	
    
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::draw()
{
    
    float currTime = 0.0f;
    
    if( isServer ) { currTime = ofGetElapsedTimef(); } else { currTime = commonTimeOsc->getTimeSecs(); }
    
    
    
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
	
    if( isServer )
    {
        fontSmall.drawString( "Server", 300, 85 );
    }
    else
    {
        fontSmall.drawString( "Offset: " + ofToString(commonTimeOsc->offsetMillis) + " OffsetTarget: " + ofToString(commonTimeOsc->offsetMillisTarget), 300, 80 );
    }
    
    ofSetColor(255, 255, 255);
    fontSmall.drawString( "Screen: " + ofToString(screenIndex) + "  Time: " + ofToString( currTime, 2), 300, 45 );
	
}

void ofApp::newData( DataPacket& _packet  )
{
    if(!isServer){
        cout<<"new data"<<endl;
        
        for(int i = 0; i < _packet.valuesFloat.size(); i++){
            cout<<_packet.valuesFloat[i]<<endl;
        }
        for(int i = 0; i < _packet.valuesInt.size(); i++){
            cout<<_packet.valuesInt[i]<<endl;
        }
        for(int i = 0; i < _packet.valuesString.size(); i++){
            cout<<_packet.valuesString[i]<<endl;
        }
    }
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::keyPressed(int key)
{
    if(isServer){
        DataPacket data;
        int client;
        string command;
        float value;
        if(key == '1'){
            client = 0;
            command = "play";
            value = 100.00;
        }
        if(key == '2'){
            client = 1;
            command = "play";
            value = 50.00;
            
        }
        if(key == '3'){
            client = 2;
            command = "play";
            value = 25.00;
            
        }
        if(key == '4'){
            client = 3;
            command = "play";
            value = 15.00;
            
        }
        if(key == '5'){
            client = 4;
            command = "play";
            value = 5.00;
            
        }
        if(key == '6'){
            client = 5;
            command = "play";
            value = 2.50;
        }
        data.valuesFloat.push_back(value);
        data.valuesInt.push_back(client);
        data.valuesString.push_back(command);
        
        server->sendData(data.valuesString, data.valuesInt, data.valuesFloat);
    }
    
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
    if(!isServer){
        DataPacket packet;
        packet.valuesString.push_back("mouseClicked");
        client->sendData(packet);
    }
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofApp::mouseReleased(int x, int y, int button)
{
}
