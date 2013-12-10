//
//  CommonTimeOsc.h
//  MultiScreenCommonTiming
//
//  Created by Andreas Müller on 26/12/2012.
//
//
#pragma once
#include "ofxCommonTimeBase.h"
#include "ofxOsc.h"

class ofxCommonTimeOSC : public ofxCommonTimeBase
{
public:

    ofxCommonTimeOSC();
    ~ofxCommonTimeOSC();

    void	init( ofxOscSender* _oscSender, int _uniqueComputerID );

    void	senderIsSetup( bool _isSetup );

private:

    void	sendPing();

    int		uniqueComputerID;

    bool	isSenderSetup;
    ofxOscSender* oscSender;

};
