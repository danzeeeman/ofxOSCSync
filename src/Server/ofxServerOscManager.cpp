#include "ofxServerOscManager.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
ofxServerOscManager::ofxServerOscManager()
{
	initialised = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
ofxServerOscManager::~ofxServerOscManager()
{
    ofRemoveListener(ofEvents().update, this, &ofxServerOscManager::_update);
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxServerOscManager::init( string _xmlSettingsPath = "settings/oscsync-settings.xml")
{
	string _serverSendHost	= "127.0.0.1";
	int _serverSendPort	= 7776;
	int _serverReceivePort	= 7777;

	ofxXmlSettings XML;
	bool loadedFile = XML.loadFile( _xmlSettingsPath );
	if( loadedFile )
	{
	     _serverSendHost = XML.getValue("Settings:Server:Host", "172.16.1.255");
             _serverSendPort = XML.getValue("Settings:Server:SendPost",	7776);
             _serverReceivePort = XML.getValue("Settings:Server:ReceivePort",	7777);
	}
	
	init( _serverSendHost, _serverSendPort, _serverReceivePort ); // init with default
	
}



// ---------------------------------------------------------------------------------------------------------------------------------------------
//

void ofxServerOscManager::init(string _serverSendHost, int _serverSendPort, int _serverReceivePort )
{
	serverSendHost	= _serverSendHost;
	serverSendPort	= _serverSendPort;
	serverReceivePort = _serverReceivePort;

	multicastSender.setup( serverSendHost, serverSendPort);
	receiver.setup( serverReceivePort );

	lastSentHelloMessageMillis = 0;
	milliseBetweenHelloMessages = 3 * 1000;

	ofAddListener(ofEvents().update, this, &ofxServerOscManager::_update );

	initialised = true;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxServerOscManager::_update(ofEventArgs &e)
{

	// periodically send out a hello message on the multicast address, this way anyone joining us
	// can get the address to the server if they are on the same network and listening to the right port
	if( (getServerTime() - lastSentHelloMessageMillis) > milliseBetweenHelloMessages )
	{
		ofxOscMessage m;
		m.setAddress("/hello");
		m.addIntArg( serverReceivePort ); // add the port we would like to use to receive messages as an argument, seems more flexible than having a rule like remote port + 1
		// add the scene that we are in as well? Otherwise screens joining will not know what scene we are supposed to be in.

		multicastSender.sendMessage( m );

		lastSentHelloMessageMillis = getServerTime();
	}

	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		if( m.getAddress() == "/ping" )
		{
			string tmpStr = "Ping, remote time: " + ofToString(m.getArgAsInt32(1)) +
			" computer: " + ofToString(m.getArgAsInt32(0)) +
			"  IP: " + m.getRemoteIp() +
			" Port: " + ofToString(m.getRemotePort());
            ofxOscSender & _sender = clients[m.getArgAsInt32(0)].sender;
            _sender.setup(m.getRemoteIp(), serverSendPort);

            receivedMessageSubjects.push_back( tmpStr );

			// we need to send a "pong" message back, either we send this over the multicasting address,
			// or we create a multicastSender for each new address and port that send us a message, I'm going to
			// try the multicasting route first.

			// their message comes in as /ping, their ID (int), their timestamp (int)

			int remoteComputerID 	= m.getArgAsInt32(0);
			int remoteComputerTime 	= m.getArgAsInt32(1);

			ofxOscMessage m;
			m.setAddress("/pong");
			m.addIntArg( remoteComputerID );		// their ID
			m.addIntArg( getServerTime() );			// my time
			m.addIntArg( remoteComputerTime );		// their time

			_sender.sendMessage(m);
            multicastSender.sendMessage(m);
		}
        if(m.getAddress() == "/data"){
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

	/*
     * current limit is 20 machines
     */

	int maxSavedSubjects = 20;
	if( receivedMessageSubjects.size() > maxSavedSubjects )
	{
		receivedMessageSubjects.erase( receivedMessageSubjects.begin(),receivedMessageSubjects.begin()+1 );
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxServerOscManager::draw()
{
	if( !initialised ) return;

	string buf;
	buf = "Sending OSC messages to " + string(serverSendHost) + ":"+ ofToString(serverSendPort) + "    Listening for OSC messages on port: " + ofToString(serverReceivePort);

#ifdef HEADLESS

#ifdef TARGET_LINUX
	std::system( "clear" );
#endif

	cout << "*********************************************************************************" << endl;
	cout << buf << endl;
	for( unsigned int i = 0; i < receivedMessageSubjects.size(); i++ )
	{
        cout << "    " << receivedMessageSubjects.at(i) << endl;
	}
	cout << "*********************************************************************************" << endl;
#else

	ofDrawBitmapString(buf, 10, 20);

	for( unsigned int i = 0; i < receivedMessageSubjects.size(); i++ )
	{
		ofDrawBitmapString(receivedMessageSubjects.at(i), 10, 60 + (i * 20) );
	}

#endif
}


// ---------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxServerOscManager::sendData( vector<string> _valuesStrings, vector<int> _valuesInt, vector<float> _valuesFloat )
{
	if( !initialised ) return;

	ofxOscMessage m;
	m.setAddress("/data");

    for(unsigned int i = 0; i < _valuesStrings.size(); i++){
        m.addStringArg( _valuesStrings.at(i) );
    }
	for( unsigned int i = 0; i < _valuesInt.size(); i++ )
	{
		m.addIntArg( _valuesInt.at(i) );
	}

	for( unsigned int i = 0; i < _valuesFloat.size(); i++ )
	{
		m.addFloatArg( _valuesFloat.at(i) );
	}

	//sender.sendMessage( m );
    std::map<int, oscClient>::iterator iter;
    for(iter = clients.begin(); iter != clients.end(); iter++){
        iter->second.sender.sendMessage(m);
    }
}

void ofxServerOscManager::sendData(int clientID, vector<string> _valuesStrings, vector<int> _valuesInt, vector<float> _valuesFloat )
{
	if( !initialised ) return;

	ofxOscMessage m;
	m.setAddress("/data");

    for(unsigned int i = 0; i < _valuesStrings.size(); i++){
        m.addStringArg( _valuesStrings.at(i) );
    }
	for( unsigned int i = 0; i < _valuesInt.size(); i++ )
	{
		m.addIntArg( _valuesInt.at(i) );
	}

	for( unsigned int i = 0; i < _valuesFloat.size(); i++ )
	{
		m.addFloatArg( _valuesFloat.at(i) );
	}

    clients[clientID].sender.sendMessage(m);
}

void ofxServerOscManager::sendData( DataPacket _packet)
{
	if( !initialised ) return;

	ofxOscMessage m;
	m.setAddress("/data");

    for(unsigned int i = 0; i < _packet.valuesString.size(); i++){
        m.addStringArg( _packet.valuesString[i] );
        ofLog(OF_LOG_VERBOSE)<<_packet.valuesString[i]<<endl;
    }
	for( unsigned int i = 0; i < _packet.valuesInt.size(); i++ )
	{
		m.addIntArg( _packet.valuesInt[i] );
        ofLog(OF_LOG_VERBOSE)<<_packet.valuesInt[i]<<endl;
	}

	for( unsigned int i = 0; i < _packet.valuesFloat.size(); i++ )
	{
		m.addFloatArg( _packet.valuesFloat[i] );
        ofLog(OF_LOG_VERBOSE)<<_packet.valuesFloat[i]<<endl;
	}
    

    std::map<int, oscClient>::iterator iter;
    for(iter = clients.begin(); iter != clients.end(); iter++){
        ofLog(OF_LOG_VERBOSE)<<"send message"<<endl;
        iter->second.sender.sendMessage(m);
    }
}

void ofxServerOscManager::sendData( DataPacket _packet, int clientID)
{
	if( !initialised ) return;

	ofxOscMessage m;
	m.setAddress("/data");

    for(unsigned int i = 0; i < _packet.valuesString.size(); i++){
        m.addStringArg( _packet.valuesString[i] );
    }
	for( unsigned int i = 0; i < _packet.valuesInt.size(); i++ )
	{
		m.addIntArg( _packet.valuesInt[i] );
	}

	for( unsigned int i = 0; i < _packet.valuesFloat.size(); i++ )
	{
		m.addFloatArg( _packet.valuesFloat[i] );
	}

    std::map<int, oscClient>::iterator iter;
    for(iter = clients.begin(); iter != clients.end(); iter++){
        iter->second.sender.sendMessage(m);
    }
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
int ofxServerOscManager::getServerTime()
{
	return ofGetElapsedTimeMillis();
}
