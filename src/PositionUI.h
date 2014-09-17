//
//  PositionUI.h
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#ifndef __NPBOT__PositionUI__
#define __NPBOT__PositionUI__

#include "cinder/app/AppNative.h"
#include "ciUI.h"
#include "ArmPosition.h"
#include <iostream>

class PositionUI
{

public:
    PositionUI(){};
    void setup(float * value,float *targetValue, int x,int y, string name, int max,int min);
    void update(bool axisIsDirty);
    void draw();
    void guiEvent(ciUIEvent *event);
    ciUICanvas *gui;
    
    ciUILabel *labelMain;
    ciUISlider *sliderMain;
    
    float * value;
    float * targetValue;
    string name;
};

#endif /* defined(__NPBOT__PositionUI__) */
