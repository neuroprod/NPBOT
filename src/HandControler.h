//
//  HandControler.h
//  NPBOT
//
//  Created by Kris Temmerman on 23/09/14.
//
//

#ifndef __NPBOT__HandControler__
#define __NPBOT__HandControler__

#include "cinder/app/AppNative.h"
#include "ciUI.h"
#include "SerialHandler.h"
#include <iostream>
#include "AxisData.h"

using namespace ci;
using namespace ci::app;
using namespace std;
class HandControler
{
    
public:
    HandControler(){}
    void setup( int x, int y);
    void update();
    void draw();
    void guiEvent(ciUIEvent *event);
    ciUICanvas *gui;
    SerialHandler *serialHandler;
    float pos1;
    float pos2;
    float pos;
    AxisData *data;
};
#endif /* defined(__NPBOT__HandControler__) */
