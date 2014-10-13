//
//  SnapTask.h
//  NPBOT
//
//  Created by Kris Temmerman on 05/10/14.
//
//

#ifndef NPBOT_SnapTask_h
#define NPBOT_SnapTask_h
#include "Task.h"
#include "GrabTask.h"
#include "Constants.h"
class SnapTask:public Task
{


public:
    SnapTask(float _snapPos): Task("SnapTask")
    {
        id=0;
        snapPos =_snapPos;
    }

    virtual void start()
    {
        
        
        Task::start();
       /* mainTaskHandler->axisDatas[0]->setUnits(snapPos);
        mainTaskHandler->axisDatas[1]->setUnits(71);
        mainTaskHandler->axisDatas[2]->setUnits(-75);
        mainTaskHandler->axisDatas[3]->setUnits(0);
        mainTaskHandler->axisDatas[4]->setUnits(85);
        mainTaskHandler->axisDatas[5]->setUnits(90);
        */
        
        
        mainTaskHandler->position->targetX=SNAP_X;
        mainTaskHandler->position->targetY=SNAP_Y;
        mainTaskHandler->position->targetZ=snapPos;
        mainTaskHandler->position->targetRotX =90;
        mainTaskHandler->position->targetRotY =-90;
        mainTaskHandler->position->setRotationsFromPositions();
        mainTaskHandler-> axisDatas[5]->setUnits(85);//invers coordiantes
        
        
        if(! mainTaskHandler->serialHandler->sendPositions())
        {
            
            serialIsOnline =false;
            wait =3;
            
        }else
        {
            
            
            serialIsOnline =true ;
        }
    }
    
    virtual void update()
    {
        
        if(serialIsOnline)
        {
             if( mainTaskHandler->serialHandler->isDone) taskComplete();
            
        }else
        {
            wait--;
            if(wait==0)
            {
                
                taskComplete();
            }
            
            
        }
    }
    virtual void taskComplete()
    {
      
        
        
        snapPicture();
        
        
        if(snapPos<1500){
            int pos =snapPos+100;
            if(pos>1500) pos =1500;
            nextTask = std::make_shared<SnapTask>(pos);
            nextTask->id =id+1;
        }else
        {
            
             nextTask = std::make_shared<GrabTask>();
        }
        
        Task::taskComplete();
        
    }
    
    void snapPicture()
    {
        if(mainTaskHandler->cameraHandler->mCapture)
        {
        
            
        }else
        {
             string url =ci::app::getAssetPath("image" +toString(id)+".png").string();
            ImageSourceRef img = loadImage(url);
            mainTaskHandler->cameraHandler->mSurface =Surface8u(img);
            mainTaskHandler->cameraHandler->mTexture->create(  mainTaskHandler->cameraHandler->mSurface );
         ;
        
        
        }
        
        
        std::shared_ptr<CaptureImage> capImage=  std::make_shared<CaptureImage>();
        capImage->mTexture =mainTaskHandler->cameraHandler->mTexture;
          capImage->mSurface=mainTaskHandler->cameraHandler->mSurface;
        capImage->map1 =&mainTaskHandler->map1;
         capImage->map2 =&mainTaskHandler->map2;
        capImage->posZ=snapPos;
        capImage->id =id;
        

        capImage->calculate();
        
        for(int i=0;i<capImage->cubes.size();i++){
            Cube *newCube =capImage->cubes[i];
            bool replaced =false;
            for(int j=0;j< mainTaskHandler->cubes.size();j++)
            {
             Cube *oldCube =mainTaskHandler->cubes[j];
              //  cout << oldCube->center.distance(newCube->center)<<endl;
                if(oldCube->center.distance(newCube->center)<newCube->size*2)
                {
                    if(newCube ->distFactor<oldCube->distFactor)
                    {
                        oldCube->center = newCube->center;
                         oldCube->angle = newCube->angle;
                        oldCube->size = newCube->size;
                        
                        //cout <<"switch"<<endl;
                    }
                    replaced =true;
                
                }
            
            }
            
            
            if(!replaced){
            
                mainTaskHandler->cubes.push_back(capImage->cubes[i]);
            }
        }
               mainTaskHandler->floorCaptures.push_back(capImage);
    
    }
    
    float snapPos;
    bool serialIsOnline;
    int wait;
};

#endif
