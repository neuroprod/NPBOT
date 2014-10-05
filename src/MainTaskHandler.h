//
//  MainTaskHandler.h
//  NPBOT
//
//  Created by Kris Temmerman on 05/10/14.
//
//

#ifndef __NPBOT__MainTaskHandler__
#define __NPBOT__MainTaskHandler__

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include <iostream>
#include "ciUI.h"
#include "cinder/Camera.h"
#include "ArmNode.h"
#include "ArmPosition.h"
#include "SerialHandler.h"
#include "Task.h"
#include "cinder/Capture.h"
#include "CaptureImage.h"
using namespace ci;
using namespace ci::app;
using namespace std;
class Task;
class MainTaskHandler
{



    public :
    MainTaskHandler(){}
    void setup();
    void start();
    void update();
    void draw();
    
    
    CameraOrtho camera;
    ciUICanvas *gui;
    Vec3f center;
    ArmNode * root;
    ArmPosition * position;
    SerialHandler *serialHandler;
    vector <AxisData *> axisDatas;
    std::shared_ptr<Task > currentTask;
    CaptureRef			mCapture;
    Surface8u        mSurface;
    gl::TextureRef		mTexture;
    vector<std::shared_ptr<CaptureImage>> floorCaptures;
    
};
#endif /* defined(__NPBOT__MainTaskHandler__) */
