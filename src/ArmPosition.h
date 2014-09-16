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

class ArmPosition
{



public:
    ArmPosition(){};
    void setup()
    {
        x =y= z=rotX= rotY=rotZ=0;
        
        
        targetX = targetY= targetZ=targetRotX= targetRotY=targetRotZ =0;
    
    
    }
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
    
    
    void draw()
    {
    
    };
    void drawTarget()
    {
    
    
    };
    

};
#endif /* defined(__NPBOT__ArmTarget__) */
