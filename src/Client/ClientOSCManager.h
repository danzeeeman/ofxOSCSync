#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "osc/CommonTime/CommonTimeOsc.h"
#include "osc/Client/DataPacket.h"
// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class ClientOSCManager
{
public:
    
    ClientOSCManager();
    ~ClientOSCManager();
    
    void	init( int _uniqueComputerID, int _port = 7778);
	
    void	_update(ofEventArgs &e);
	
    void    sendData( vector<string> _valuesStrings, vector<int> _valuesInt, vector<float> _valuesFloat );
    
    void    sendData(DataPacket _packet);
    
    inline ofxOscSender*	getOSCSender()
    {
        return &sender;
    }
    
    inline ofxOscReceiver* getOSCReceiver()
    {
        return &receiver;
    }
    
    inline CommonTimeOSC* getCommonTimeOscObj()
    {
        return &commonTimeOsc;
    }
	
    ofEvent<DataPacket>  newDataEvent;
	
private:
    
    int					uniqueComputerID;
    
    CommonTimeOSC		commonTimeOsc;
	
    bool				haveSetupSender;
    ofxOscSender		sender;
	
    ofxOscReceiver		receiver;
    
};
