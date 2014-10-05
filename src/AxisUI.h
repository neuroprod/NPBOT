//
//  AxisUI.h
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#ifndef __NPBOT__AxisUI__
#define __NPBOT__AxisUI__

#include "cinder/app/AppNative.h"
#include "ciUI.h"
#include "AxisData.h"
#include <iostream>
class AxisUI
{

public:
    AxisUI(){}
    void setup(AxisData * axisData, int x, int y);
    void update();
    void draw();
    void guiEvent(ciUIEvent *event);
    ciUICanvas *gui;
    AxisData * axisData;
    
    float step;
    ciUISlider *stepSlider;
    ciUILabel * labelCurrent;
    ciUILabel * labelTarget;
   
};
#endif /* defined(__NPBOT__AxisUI__) */
