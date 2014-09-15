//
//  ArmViewport.h
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#ifndef __NPBOT__ArmViewport__
#define __NPBOT__ArmViewport__
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include <iostream>
#include "ciUI.h"
#include "cinder/Camera.h"
using namespace ci;
using namespace ci::app;
using namespace std;


class ArmViewport
{


    public :
    ArmViewport(){};
    
    void setup(int x,int y,int w,int h);
    void update();
    void draw();
    void guiEvent(ciUIEvent *event);
    ci::Area viewArea;
    ciUIDropDownList *ddl;
    int x;
    int y;
    int w;
    int h;
    
    CameraOrtho camera;
    ciUICanvas *gui;
    Vec3f center;


};
#endif /* defined(__NPBOT__ArmViewport__) */
