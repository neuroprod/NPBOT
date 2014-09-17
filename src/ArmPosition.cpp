//
//  ArmTarget.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#include "ArmPosition.h"

void ArmPosition::setPositionsFromRotations(ArmNode * endNode)
{
    Vec4f posTarget  = endNode->globalMatrix1 *Vec4f(0,0,0,1);
    targetX  =posTarget.x;
    targetY =posTarget.y;
    targetZ =posTarget.z;


    
    Vec4f posCurrent = endNode->globalMatrix2 *Vec4f(0,0,0,1);
    x =posCurrent.x;
    y=posCurrent.y;
    z=posCurrent.z;
}
void ArmPosition::setRotationsFromPositions(){}
void ArmPosition::drawCurrent()
{

    gl::color(ColorA(1,0,1,1));
    drawPosition(x,y,z,rotX,rotY,rotZ);
    
}

void ArmPosition::drawTarget()
{

    
    gl::color(ColorA(1,1,1,1));
    drawPosition(targetX,targetY,targetZ,targetRotX,targetRotY,targetRotZ);
   


}
void ArmPosition::drawPosition(float targetX, float targetY,float targetZ,float targetRotX,float targetRotY,float targetRot)
{
    float size =100;
    gl::drawLine(Vec3f(targetX+size ,targetY,targetZ), Vec3f(targetX-size,targetY,targetZ));
    gl::drawLine(Vec3f(targetX ,targetY+size,targetZ), Vec3f(targetX,targetY-size,targetZ));
    gl::drawLine(Vec3f(targetX ,targetY,targetZ+size), Vec3f(targetX,targetY,targetZ-size));

}