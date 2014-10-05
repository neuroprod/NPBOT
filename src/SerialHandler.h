//
//  SerialHandler.h
//  NPBOT
//
//  Created by Kris Temmerman on 16/09/14.
//
//

#ifndef __NPBOT__SerialHandler__
#define __NPBOT__SerialHandler__

#include <iostream>

#include "cinder/Serial.h"
#include "cinder/app/AppNative.h"
#include "AxisData.h"
using namespace ci;
using namespace ci::app;
using namespace std;


class SerialHandler
{
    public :
    SerialHandler(){}
    void setup();
    
    bool sendHandPos(int pos1, int pos2);
    bool sendHandRelease();
    bool sendHandGrab();
    bool sendHoming();
    bool sendPositions();
    bool setPositions();
    void update();
     void updateHand();
    void writeInt (int val);
    bool isDone;
    
    Serial serial;
    Serial serialHand;
    bool handisonline;
    bool isonline;
    vector<AxisData *> axisData;
};
#endif /* defined(__NPBOT__SerialHandler__) */
