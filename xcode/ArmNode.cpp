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
    if(pos==2) customDraw =std::bind(&ArmNode::drawNode1  , this);
    if(pos==3) customDraw =std::bind(&ArmNode::drawNode1  , this);
    if(pos==4) customDraw =std::bind(&ArmNode::drawNode1  , this);
    if(pos==5) customDraw =std::bind(&ArmNode::drawNode1  , this);
    if(pos==6) customDraw =std::bind(&ArmNode::drawNode1  , this);
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
    gl::pushMatrices();
    gl::multModelView(globalMatrix1 );
    glEnable( GL_LIGHT0 );
    // glEnable( GL_LIGHT1 );
    GLfloat light_position[] = {1000,600 , 800,1 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position );

    
    
    customDraw();
    gl::drawCoordinateFrame(40,0,0);
    gl::popMatrices();

if(child) child->drawTarget();
};
void ArmNode::drawCurrent()
{
    gl::pushMatrices();
    gl::multModelView(globalMatrix2 );
    customDraw();
    gl::drawCoordinateFrame();
    gl::popMatrices();
if(child) child->drawCurrent();

};




///////////////////
//
////////////////////

void ArmNode::drawNode1()
{
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_LIGHTING );
	   
   // GLfloat light_position2[] = {-1000,-1000 , -500,1};
	//glLightfv( GL_LIGHT1, GL_POSITION, light_position2 );
    
    ci::ColorA color(  1.0, 1.0, 1.0f, 1.0f );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,	color );

    
    gl::drawCube(Vec3f(0,100,0), Vec3f(30,200,30));
    
    glDisable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
    glDisable (GL_DEPTH_TEST);

};
void ArmNode::drawNode2()
{
  

};
void ArmNode::drawNode3()
{
   
};
void ArmNode::drawNode4()
{


};
void ArmNode::drawNode5()
{


};
void ArmNode::drawNode6()
{


};