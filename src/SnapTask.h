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
        mainTaskHandler->axisDatas[0]->setUnits(snapPos);
        mainTaskHandler->axisDatas[1]->setUnits(71);
        mainTaskHandler->axisDatas[2]->setUnits(-75);
        mainTaskHandler->axisDatas[3]->setUnits(0);
        mainTaskHandler->axisDatas[4]->setUnits(85);
        mainTaskHandler->axisDatas[5]->setUnits(90);
        if(! mainTaskHandler->serialHandler->sendPositions())
        {
            
            serialIsOnline =false;
            wait =20;
            
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
            int pos =snapPos+300;
            if(pos>1500) pos =1500;
            nextTask = std::make_shared<SnapTask>(pos);
            nextTask->id =id+1;
        }
        
        Task::taskComplete();
        
    }
    
    void snapPicture()
    {
        if(mainTaskHandler->mCapture)
        {
        
            
        }else
        {
             string url =ci::app::getAssetPath("image" +toString(id)+".png").string();
            ImageSourceRef img = loadImage(url);
            mainTaskHandler->mSurface =Surface8u(img);
            mainTaskHandler->mTexture->create(  mainTaskHandler->mSurface );
         ;
        
        
        }
        
        
        std::shared_ptr<CaptureImage> capImage=  std::make_shared<CaptureImage>();
        capImage->mTexture =mainTaskHandler->mTexture;
          capImage->mSurface=mainTaskHandler->mSurface;
        capImage->map1 =&mainTaskHandler->map1;
         capImage->map2 =&mainTaskHandler->map2;
        capImage->posZ=snapPos;
        capImage->id =id;
        capImage->calculate();
        mainTaskHandler->floorCaptures.push_back(capImage);
    
    }
    
    float snapPos;
    bool serialIsOnline;
    int wait;
};

#endif
