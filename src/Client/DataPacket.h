//
//  DataPacket.h
//  ofxOSCSync
//
//  Created by Dan Moore on 10/13/13.
//
//
#pragma once
#include "ofMain.h"

class DataPacket
{
public:
    vector<int> valuesInt;
    vector<float> valuesFloat;
    vector<string> valuesString;
};
