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
#include "CinderOpenCV.h"
#include "Cube.h"
#include "CameraHandler.h"
#include "cinder/gl/TextureFont.h"
using namespace ci;
using namespace ci::app;
using namespace std;
class Task;
class MainTaskHandler
{



    public :
    MainTaskHandler(){isRunning =false;}
    void setup();
    void start();
    void update();
    void draw();
    Vec3f getNextPos();
    gl::TextureFontRef font;
    bool isRunning;
    CameraOrtho camera;
    ciUICanvas *gui;
    Vec3f center;
    ArmNode * root;
    ArmPosition * position;
    SerialHandler *serialHandler;
    vector <AxisData *> axisDatas;
    std::shared_ptr<Task > currentTask;
   
    vector<std::shared_ptr<CaptureImage>> floorCaptures;
       
    cv::Mat  map1, map2;
    CameraHandler *cameraHandler;
    vector<Cube *> cubes;
    int count;
    int heightCount;
};
#endif /* defined(__NPBOT__MainTaskHandler__) */
