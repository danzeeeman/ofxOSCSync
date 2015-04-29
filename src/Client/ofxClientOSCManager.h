#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxCommonTimeOsc.h"
#include "DataPacket.h"
#include "ofxXmlSettings.h"
// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class ofxClientOSCManager
{
public:
    
    ofxClientOSCManager();
    ~ofxClientOSCManager();
    
    void    init( int _uniqueComputerID, int _port = 7776);
    
    void    init( string _settingsfile = "settings/sync-settings.xml");
    
    void    _update(ofEventArgs &e);
    
    void    sendData( vector<string> _valuesStrings, vector<int> _valuesInt, vector<float> _valuesFloat );
    
    void    sendData(DataPacket _packet);
    
    
    
    inline ofxOscSender*  getOSCSender()
    {
        return &sender;
    }
    
    inline ofxOscReceiver*  getOSCReceiver()
    {
        return &receiver;
    }
    
    inline ofxCommonTimeOSC*  getCommonTimeOscObj()
    {
        return &commonTimeOsc;
    }
    
    inline bool isReady(){
        return haveSetupSender;
    }
    
    inline string getServerIP(){
        return serverIP;
    }
    
    ofEvent<DataPacket>  newDataEvent;
    
private:
    
    int				uniqueComputerID;
    
    ofxCommonTimeOSC		commonTimeOsc;
    
    bool			haveSetupSender;
    
    ofxOscSender		sender;
    ofxOscReceiver		receiver;
    
    string serverIP;
};
