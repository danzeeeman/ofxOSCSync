#include "ofxClientOSCManager.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
ofxClientOSCManager::ofxClientOSCManager()
{
    haveSetupSender = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
ofxClientOSCManager::~ofxClientOSCManager()
{
    ofRemoveListener(ofEvents().update, this, &ofxClientOSCManager::_update);
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
//

void ofxClientOSCManager::init(string _settingsfile)
{
    if(ofFile::doesFileExist(ofToDataPath(_settingsfile))){
        ofxXmlSettings settings;
        settings.load(ofToDataPath(_settingsfile));
        int id = settings.getValue("osc:client:id", 0);
        int port = settings.getValue("osc:client:port", 7776);
        init(uniqueComputerID, port);
    }else{
        init(ofRandom(0, 255), 7777);
    }
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxClientOSCManager::init( int _uniqueComputerID , int port)
{
    uniqueComputerID = _uniqueComputerID;

    receiver.setup( port );

    commonTimeOsc.init( &sender, uniqueComputerID );

    ofAddListener(ofEvents().update, this, &ofxClientOSCManager::_update );
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
//

void ofxClientOSCManager::sendData( vector<string> _valuesStrings, vector<int> _valuesInt, vector<float> _valuesFloat )
{
    if( !haveSetupSender ) return;

    ofxOscMessage m;
    m.setAddress("/data");

    for(auto value :_valuesStrings);
    {
        m.addStringArg( value );
    }
    for(auto value :_valuesInt)
    {
        m.addIntArg( value );
    }
    for(auto value:_valuesFloat)
    {
        m.addFloatArg(value );
    }

    sender.sendMessage(m);

}

void ofxClientOSCManager::sendData( DataPacket _packet)
{
    if( !haveSetupSender ) return;

    ofxOscMessage m;
    m.setAddress("/data");

    for(auto value : _packet.valueString);
    {
        m.addStringArg( value );
    }
    for(auto value : _packet.valuesInt)
    {
        m.addIntArg( value );
    }
    for(auto value: _packet.valuesFloat)
    {
        m.addFloatArg( value );
    }

    sender.sendMessage(m);

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//


void ofxClientOSCManager::_update(ofEventArgs &e)
{
    // check for waiting messages
    while( receiver.hasWaitingMessages() )
    {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);

        //cout << "Got message: " << m.getAddress() << endl;

        if( m.getAddress() == "/hello" )
        {
            // if we get a hello message and we haven't set up our sender, get the Ip and port from the hello message
            if( !haveSetupSender )
            {
                serverIP = m.getRemoteIp();
                int sendPort = m.getArgAsInt32(0);
                sender.setup( m.getRemoteIp(), sendPort );
                commonTimeOsc.senderIsSetup( true );
                haveSetupSender = true;
            }
        }
        else if( m.getAddress() == "/pong" )
        {
            // pong messages come in as: /pong, receiving computer ID (int), server timestamp (int), timestamp when remote computer sent (int)

            if( m.getArgAsInt32(0) == uniqueComputerID )  	// if the message is for us
            {
                int serverTime = m.getArgAsInt32(1);
                int originalTimeStamp = m.getArgAsInt32(2);

                commonTimeOsc.newReading( serverTime, commonTimeOsc.getInternalTimeMillis() - originalTimeStamp );
            }
        }
        else if ( m.getAddress() == "/data" )
        {
            DataPacket packet;

            for( int i = 0; i < m.getNumArgs(); i++ )
            {
                ofxOscArgType argType = m.getArgType(i);
                if( argType == OFXOSC_TYPE_INT32 || argType ==  OFXOSC_TYPE_INT64 )
                {
                    packet.valuesInt.push_back( m.getArgAsInt32(i) );
                }
                else if ( argType == OFXOSC_TYPE_FLOAT )
                {
                    packet.valuesFloat.push_back( m.getArgAsFloat(i) );
                }
                else if( argType ==OFXOSC_TYPE_STRING)
                {
                    packet.valuesString.push_back( m.getArgAsString(i) );
                }
            }

            ofNotifyEvent( newDataEvent, packet, this );
        }
    }

}
