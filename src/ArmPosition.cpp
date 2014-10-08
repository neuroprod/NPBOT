//
//  ArmTarget.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#include "ArmPosition.h"
#include "Constants.h"
void ArmPosition::setPositionsFromRotations()
{
    Vec4f posTarget  = endNode->globalMatrix1 *Vec4f(0,0,0,1);
    targetX  =posTarget.x;
    targetY =posTarget.y;
    targetZ =posTarget.z;


    
    Vec4f posCurrent = endNode->globalMatrix2 *Vec4f(0,0,0,1);
    x =posCurrent.x;
    y=posCurrent.y;
    z=posCurrent.z;
    fail=false;
}
void ArmPosition::setRotationsFromPositions()
{




    Matrix44f matrix;
    


    matrix.setToIdentity();
    matrix.translate(Vec3f(targetX,targetY,targetZ));
  matrix.rotate(Vec3f(1,0,0), targetRotX/180*pi);
    matrix.rotate(Vec3f(0,1,0),targetRotY/180*pi);
    
    
    targetMain= matrix*Vec4f(-HAND_LENGTH,0,0,1);



    vector<Vec2f > intersections;
    Vec2f  mainArmTarget;
    
    
    
    Vec3f axis2;
    Vec3f axis3; 
    //384 40
    
    float Axis2VirtLength = sqrt(ARM2_LENGTH*ARM2_LENGTH + ARM2_OFFSET*ARM2_OFFSET);
    
    float offAngle = atan2( ARM2_OFFSET,ARM2_LENGTH)*180/3.1415;
   
   getCircleIntersections(intersections,Vec2f(0,START_HEIGHT), ARM1_LENGTH,Vec2f(targetMain.x,targetMain.y), Axis2VirtLength);
    if(intersections.size()>1)
    {
        
        console()<<targetMain<<endl;
        if(intersections[0].y>intersections[1].y && targetMain.x>0   )
        {
            mainArmTarget  = intersections[0];
        }else
        {
            mainArmTarget = intersections[1];
        }
        
        axis2.x = mainArmTarget.x;
        axis2.y =mainArmTarget.y;
        axis2.z = targetMain.z;
        axis3.x = targetMain.x;
        axis3.y = targetMain.y;
        axis3.z = targetMain.z;
     
        
        
        
        float angle3 = atan2( axis3.y -axis2.y,axis3.x -axis2.x) *180/pi;
        float angle2 = atan2( axis2.y-START_HEIGHT ,axis2.x) *180/pi;
      
        axisDatas[1]->setUnits(angle2);
        axisDatas[2]->setUnits(angle3-angle2-offAngle);
       
        axisDatas[0]->setUnits(axis3.z);
        
        
        axisDatas[3]->setUnits(0 );
          axisDatas[4]->setUnits(0 );
        rootNode->update();
        Matrix44f matrixInv =  rootNode->child->child->child-> globalMatrix1.inverted();
        
        targetInvert =matrixInv * Vec4f(targetX,targetY,targetZ,1);
        
     
        
               float angle4  = atan2(targetInvert.y,targetInvert.x) *180/3.1415 ;
     
        int switchA =0;
        if(angle4<0)
        {
            switchA =1;
            angle4 =angle4+180;
       
        }
        if(angle4>180)
        {
         angle4 =angle4-180;
            switchA =2;
        }
        axisDatas[3]->setUnits(angle4 -90 );
        
      
        
        rootNode->update();
        matrixInv =  rootNode->child->child->child-> child->globalMatrix1.inverted();
        targetInvert2 =matrixInv * Vec4f(targetX,targetY,targetZ,1);
        float angle5  = atan2(targetInvert2.y,targetInvert2.x) *180/3.1415;
        
        axisDatas[4]->setUnits(angle5);
        rootNode->update();
        
        
        Vec4f posTarget  = endNode->globalMatrix1 *Vec4f(0,0,0,1);
        
        
        
        if(posTarget.x >targetX-1 && posTarget.x<targetX+1 && posTarget.y >targetY-1 && posTarget.y<targetY+1 && posTarget.z >targetZ-1 && posTarget.z<targetZ+1)
        {
        
            fail =false;
        
        }else
        {
            fail =true;
        
        }
        
        
        
        
    }else
    {
        fail=true;
        
       
    }







}
void  ArmPosition::getCircleIntersections(vector<Vec2f> &result, Vec2f center1, float radius1,Vec2f center2, float radius2)
{
    float  distance = center2.distance(center1);
    if(distance > radius1 + radius2) // circles are too far apart, no solution(s)
    {
        //std::cout << "Circles are too far apart\n";
        return ;
    }
    
    // one circle contains the other
    else if(distance + min(radius1, radius2) < max(radius1, radius2))
    {
        //std::cout << "One circle contains the other\n";
        return ;
    }else
    {
        double a = (radius1*radius1 - radius2*radius2 + distance*distance)/ (2.0*distance);
        double h = sqrt(radius1*radius1 - a*a);
        
        // find p2
        Vec2f p2( center1.x + (a * (center2.x - center1.x)) / distance,
                 center1.y + (a * (center2.y - center1.y)) / distance);
        
        // find intersection points p3
        Vec2f result1( p2.x + (h * (center2.y - center1.y)/ distance),
                      p2.y - (h * (center2.x - center1.x)/ distance)
                      );
        Vec2f result2( p2.x - (h * (center2.y - center1.y)/ distance),
                      p2.y + (h * (center2.x - center1.x)/ distance)
                      );
        
        result.push_back(result1);
        result.push_back(result2);
    }
    
    
    
    
}
void ArmPosition::drawCurrent()
{

    gl::color(ColorA(1,0,1,1));
    drawPosition(x,y,z,rotX,rotY,rotZ);
   
    //////
    
}

void ArmPosition::drawTarget()
{
    if(fail)
    {
     gl::color(ColorA(1,0,0,1));
    
    }else
    {
    
     gl::color(ColorA(1,1,1,1));
    }
    
   
    drawPosition(targetX,targetY,targetZ,targetRotX,targetRotY,targetRotZ);
   
    gl::color(ColorA(1,1,1,0.5));
    gl::drawLine(Vec3f(targetX ,targetY,targetZ),Vec3f( targetMain.x,targetMain.y,targetMain.z ));
    
  /* gl::color(ColorA(1,0,0,0.1));
    gl::drawSolidCircle(Vec2f(0,250), 350);
    gl::color(ColorA(1,0,0,0.1));
    gl::drawSolidCircle(Vec2f(targetMain.x,targetMain.y), 350.291);
    
     gl::color(ColorA(1,1,0,1));
    gl::drawLine(Vec3f(0 ,0,0),Vec3f( targetInvert.x,targetInvert.y,targetInvert.z ));
    
    gl::color(ColorA(0,1,1,1));
    gl::drawLine(Vec3f(0 ,0,0),Vec3f( targetInvert2.x,targetInvert2.y,targetInvert2.z ));
    */
    
    
    
    

}
void ArmPosition::drawPosition(float targetX, float targetY,float targetZ,float targetRotX,float targetRotY,float targetRot)
{
    float size =100;
    gl::drawLine(Vec3f(targetX+size ,targetY,targetZ), Vec3f(targetX-size,targetY,targetZ));
    gl::drawLine(Vec3f(targetX ,targetY+size,targetZ), Vec3f(targetX,targetY-size,targetZ));
    gl::drawLine(Vec3f(targetX ,targetY,targetZ+size), Vec3f(targetX,targetY,targetZ-size));

}