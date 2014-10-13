//
//  ArmNode.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#include "ArmNode.h"

void ArmNode::setup(int pos,int _type, float x,float y,float z,float rotX,float rotY,float rotZ)
{
    offsetMatrix.rotate(Vec3f(1,0,0), rotX/180*3.1415);
    offsetMatrix.rotate(Vec3f(0,1,0), rotY/180*3.1415);
    offsetMatrix.rotate(Vec3f(0,0,1), rotZ/180*3.1415);
    offsetMatrix.translate(Vec3f(x,y,z));
    type =_type;
    if(pos==1) customDraw =std::bind(&ArmNode::drawNode1, this);
    if(pos==2) customDraw =std::bind(&ArmNode::drawNode2   , this);
    if(pos==3) customDraw =std::bind(&ArmNode::drawNode3  , this);
    if(pos==4) customDraw =std::bind(&ArmNode::drawNode4 , this);
    if(pos==5) customDraw =std::bind(&ArmNode::drawNode5  , this);
    if(pos==6) customDraw =std::bind(&ArmNode::drawNode6  , this);
    
    nodeColor =ColorA(1,1,1,0.6);
    ambientColor =ColorA(1,1,1,0.3);
    nodeColor2 =ColorA(1,1,1,1);
    ambientColor2 =ColorA(1,1,1,0.3);
    
};

void ArmNode::update()
{
    lockalMatrix1.setToIdentity();
    
	lockalMatrix2.setToIdentity();
	
    
    if(type ==0)
    {
        lockalMatrix1.rotate(Vec3f(0,0,1),data->getTargetInUnits()/180*3.1415);
        lockalMatrix2.rotate(Vec3f(0,0,1),data->getCurrentInUnits()/180*3.1415);

    }else
    {
        lockalMatrix1.translate(Vec3f(0,0,data->getTargetInUnits()));
        lockalMatrix2.translate(Vec3f(0,0,data->getCurrentInUnits()));
    }
    
    
    Matrix44f l1 =offsetMatrix *lockalMatrix1;
    Matrix44f l2 =offsetMatrix *lockalMatrix2;
    if(!parent)
	{
		globalMatrix1.set(&l1.m[0]);
		globalMatrix2.set(&l2.m[0]);
	}
	else
	{
		
		globalMatrix1=  parent->globalMatrix1 *l1;
        globalMatrix2=  parent->globalMatrix2 *l2;
	}
    
    if(child) child->update();


};


void ArmNode::drawTarget()
{
    GLfloat light_position[] = {1000,600 , 800,1 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glEnable( GL_LIGHT0 );    gl::pushMatrices();
 
    gl::multModelView(globalMatrix1 );
    glEnable( GL_LIGHT0 );
    //gl::enableWireframe();
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
   // glEnable(GL_DEPTH_TEST);
    glEnable( GL_LIGHTING );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,	nodeColor );
    glMaterialfv( GL_FRONT, GL_AMBIENT,	ambientColor );
    customDraw();
    
    
    glDisable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
    glDisable (GL_DEPTH_TEST);
   // gl::disableWireframe();
    
    
    gl::drawCoordinateFrame(80,0,0);
    gl::popMatrices();
     glDisable(GL_CULL_FACE);

if(child) child->drawTarget();
};
void ArmNode::drawCurrent()
{
    GLfloat light_position[] = {1000,600 , 800,1 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glEnable( GL_LIGHT0 );    gl::pushMatrices();
    gl::pushMatrices();
    gl::multModelView(globalMatrix2 );
    glEnable( GL_LIGHT0 );
    
    
    
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_LIGHTING );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,	nodeColor2 );
    glMaterialfv( GL_FRONT, GL_AMBIENT,	ambientColor2 );
    customDraw();
    glDisable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
    glDisable (GL_DEPTH_TEST);
    gl::drawCoordinateFrame();
    gl::popMatrices();
if(child) child->drawCurrent();

};




///////////////////
//
////////////////////

void ArmNode::drawNode1()
{
  

    
    gl::drawCube(Vec3f(0,125,0), Vec3f(30,250,30));
    
   

};
void ArmNode::drawNode2()
{
  
 gl::drawCube(Vec3f(150,0,0), Vec3f(300,30,30));
};
void ArmNode::drawNode3()
{
    gl::drawCube(Vec3f((384-80)/2,40,0), Vec3f(384-80,30,30));
};
void ArmNode::drawNode4()
{

gl::drawCube(Vec3f(-40,0,50), Vec3f(10,70,150));

    gl::drawCube(Vec3f(40,0,50), Vec3f(10,70,150));
     gl::drawCube(Vec3f(70,0,50), Vec3f(40,40,40));

};
void ArmNode::drawNode5()
{
 gl::drawCube(Vec3f(100,0,0), Vec3f(100,30,30));

};
void ArmNode::drawNode6()
{
    float grabHeight = 100;
    gl::drawCube(Vec3f(0,0,-grabHeight), Vec3f(30,100,30));
    
    
    gl::drawCube(Vec3f(0,50,50-grabHeight), Vec3f(30,30,100));
    gl::drawCube(Vec3f(0,-50,50-grabHeight), Vec3f(30,30,100));
    
    
};