//
//  ArmTarget.h
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#ifndef __NPBOT__ArmTarget__
#define __NPBOT__ArmTarget__

#include <iostream>
#include "ArmNode.h"
#include "AxisData.h"
class ArmPosition
{



public:
    ArmPosition(){};
    void setup()
    {
        x =y= z=rotX= rotY=rotZ=0;
        
        
        targetX = targetY= targetZ=targetRotX= targetRotY=targetRotZ =0;
    
    
    }
    void getCircleIntersections(vector<Vec2f> &result, Vec2f center1, float radius1,Vec2f center2, float radius2);
    float x;
    float y;
    float z;
    float rotX;
    float rotY;
    float rotZ;

    
    float targetX;
    float targetY;
    float targetZ;
    float targetRotX;
    float targetRotY;
    float targetRotZ;
    
    void setPositionsFromRotations();
    void setRotationsFromPositions();
    vector<AxisData *> axisDatas;
    ArmNode *rootNode;
    ArmNode * endNode;
    void drawCurrent();
    void drawTarget();
    void drawPosition(float targetX, float targetY,float targetZ,float targetRotX,float targetRotY,float targetRotZ);
    
    bool fail;
    Matrix44f matrixEnd;
    Vec4f targetMain;
    
    Vec4f targetInvert;
     Vec4f targetInvert2;
};
#endif /* defined(__NPBOT__ArmTarget__) */
