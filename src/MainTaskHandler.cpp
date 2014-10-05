//
//  MainTaskHandler.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 05/10/14.
//
//

#include "MainTaskHandler.h"
#include "HomingTask.h"
void MainTaskHandler::setup()
{

    
    float windowScale =1500;
    float asp =(float)getWindowWidth()/ getWindowHeight();
    center.set(500, 200, 1000);
    camera.setOrtho(windowScale, -windowScale, -windowScale/asp, windowScale/asp, 100, 7000);
    camera.lookAt( Vec3f(2000,1000,1000)+center, center, Vec3f(0,1,0) );

	for( auto device = Capture::getDevices().begin(); device != Capture::getDevices().end(); ++device ) {
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
        
	}
    
	

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
    if( mCapture && mCapture->checkNewFrame() ) {
        
        mSurface =mCapture->getSurface();
		mTexture = gl::Texture::create( mSurface );
	}

    
    
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
    
    
    if( mTexture ) {
		glPushMatrix();
        
		gl::draw( mTexture );
        
		glPopMatrix();
	}
   
    gl::pushMatrices();
    gl::setMatrices(camera );
    
    gl::drawCoordinateFrame (300,0,0);
    gl::color(ColorA(1,1,1,0.5));
    gl::drawLine(Vec3f(130,0,0), Vec3f(130,0,2000));
    gl::drawLine(Vec3f(-130,0,0), Vec3f(-130,0,2000));
    
   
    for(int i=0;i<floorCaptures.size();i++)
    {
        floorCaptures[i]->draw();
    }
    
    
    root->drawTarget();
    
    root->drawCurrent();
    
    position->drawTarget();
    position->drawCurrent();
    gl::popMatrices();
    
  



};
