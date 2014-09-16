//
//  ArmNode.h
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#ifndef __NPBOT__ArmNode__
#define __NPBOT__ArmNode__
#include "cinder/app/AppNative.h"
#include "AxisData.h"
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;


class ArmNode
{
public:
    ArmNode(){child =nullptr; parent=nullptr;}
    
    void setup(int pos,int type,float x =0,float y=0,float z=0,float rotX=0,float rotY=0,float rotZ=0);
    
    void update();
    void customDrawE(){};
    
    void drawTarget();
    void drawCurrent();
    int type;
    
    AxisData *data;
    ArmNode *child;
    ArmNode *parent;
    
    cinder::Matrix44f offsetMatrix;
    
    cinder::Matrix44f lockalMatrix1;
    cinder::Matrix44f lockalMatrix2;
    
    cinder::Matrix44f globalMatrix1;
    cinder::Matrix44f globalMatrix2;
    
    std::function<void( )> customDraw;
    
    void drawNode1();
    void drawNode2();
    void drawNode3();
    void drawNode4();
    void drawNode5();
    void drawNode6();
};

#endif /* defined(__NPBOT__ArmNode__) */
