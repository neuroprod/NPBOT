//
//  MainTaskHandler.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 05/10/14.
//
//

#include "MainTaskHandler.h"
#include "HomingTask.h"
Vec3f MainTaskHandler::getNextPos()
{
    Vec3f position;
    position.z =1400 - 150*count;
    position.y =heightCount;
    position.x = 400;
    
    count++;
    if (count>2)
    {
        heightCount+=46;
        count =0;
    }
    return position;

}
void MainTaskHandler::setup()
{
    
    count =0;
    heightCount=35;
/*
    [503.910252793615, 0, 319.5;
     0, 503.910252793615, 239.5;
     0, 0, 1]
    
    
disatance:
    [0.02562266583003429; -0.144444291145838; 0; 0; 0.08528916548364274]
    */
    
    Font fontMedium = Font( "NewMedia",15);
    font = gl::TextureFont::create(fontMedium);
    
 
  
    cv::Size imageSize;
    
    imageSize.width = 640;
    imageSize.height =480;
    
    //undistort( view, rview, cameraMatrix, distCoeffs, cameraMatrix );
    /*initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
                            getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0),
                            imageSize, CV_16SC2, map1, map2);
    
    */
    
    
    float windowScale =1000;
    float asp =(float)getWindowWidth()/ getWindowHeight();
    center.set(500, 200, 750);
    camera.setOrtho(windowScale, -windowScale, -windowScale/asp, windowScale/asp, 100, 7000);
    camera.lookAt( Vec3f(2000,1000,1000)+center, center, Vec3f(0,1,0) );

	/*for( auto device = Capture::getDevices().begin(); device != Capture::getDevices().end(); ++device ) {
		console() << "Device: " << (*device)->getName() << " "<< std::endl;
        if((*device)->getName().find("Logitech Camera") !=string::npos)
            
        

        {
            try {
                mCapture = Capture::create( 640, 480,(*device) );
                mCapture->start();
            }
            catch( ... ) {
                console() << "Failed to initialize capture" << std::endl;
            }
            
        }
        
	}*/
    
	

};
void MainTaskHandler::start()
{
    floorCaptures.clear();
    currentTask = std::make_shared<HomingTask>();
    
    currentTask->mainTaskHandler=this;
    
    
    currentTask->start();
  

};


void MainTaskHandler::update()
{
    /*if( mCapture && mCapture->checkNewFrame() ) {
        
        mSurface =mCapture->getSurface();
		mTexture = gl::Texture::create( mSurface );
	}*/

    
    
    if(!currentTask) return;
    currentTask->update();
    if(currentTask->isDone)
    {
    
        console()<< currentTask->name <<" is done"<<endl;
        if(currentTask->nextTask)
        {
        
            currentTask =currentTask->nextTask;
            currentTask->mainTaskHandler=this;
            currentTask->start();
        
        }else
        {
            currentTask.reset();
            console()<<" all tasks are done"<<endl;
        
        }
    
    
    }
    


};
void MainTaskHandler::draw(){

    glViewport (0  , 0 , getWindowWidth(), getWindowHeight());
    gl::setMatricesWindow(getWindowWidth(), getWindowHeight());
    gl::color(1.f,1.f,1.f,1.f);
    //  gui->draw();
    
    
    if( cameraHandler->mTexture ) {
		glPushMatrix();
        
		gl::draw( cameraHandler->mTexture );
        
		glPopMatrix();
	}
   
    gl::pushMatrices();
    gl::setMatrices(camera );
    
    gl::drawCoordinateFrame (300,0,0);
    gl::color(ColorA(1,1,1,0.5));
    gl::drawLine(Vec3f(130,0,0), Vec3f(130,0,1500));
    gl::drawLine(Vec3f(-130,0,0), Vec3f(-130,0,1500));
    
   gl::color(ColorA(1,1,1,0.1));
    for(int i=0;i<floorCaptures.size();i++)
    {
        floorCaptures[i]->draw();
    }
    gl::color(ColorA(1,1,1,1));
    
    
    GLfloat light_position[] = {1000,600 , 800,1 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glEnable( GL_LIGHT0 );    gl::pushMatrices();
    
    
    glEnable( GL_LIGHT0 );
    ColorA nodeColor =ColorA(1,0,0,1);
   ColorA ambientColor =ColorA(1,1,1,0.3);
    
    
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_LIGHTING );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,	nodeColor );
    glMaterialfv( GL_FRONT, GL_AMBIENT,	ambientColor );
   
    for(int i=0;i<cubes.size();i++)
    {
        float size =cubes[i]->size;
        gl::pushMatrices();
        gl::translate(cubes[i]->center);
        gl::rotate(Vec3f(0,cubes[i]->angle,0));
       // cout << cubes[i]->angle<<endl ;
        gl::drawCube(Vec3f(0,0,0), Vec3f(size,size,size));
      
        gl::popMatrices();
        
        
        
    }
   
    
    
    glDisable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );
    glDisable (GL_DEPTH_TEST);
    
    
    for(int i=0;i<cubes.size();i++)
    {
        
        gl::pushMatrices();
        gl::translate(cubes[i]->center);
         gl::rotate(Vec3f(0,70,180));
        gl::color(1.0f,1.0f,1.0f);
        font->drawString(toString(cubes[i]->center.x)+"-"+toString(cubes[i]->center.z),Vec2f(0,0));
        gl::popMatrices();
        
        
        
    }
    root->drawTarget();
    
    root->drawCurrent();
    
    position->drawTarget();
    position->drawCurrent();
    gl::popMatrices();
    
  



};
