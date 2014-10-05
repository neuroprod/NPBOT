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
        snapPos =_snapPos;
    }

    virtual void start()
    {
        
        Task::start();
        mainTaskHandler->axisDatas[0]->setUnits(snapPos);
        mainTaskHandler->axisDatas[1]->setUnits(116);
        mainTaskHandler->axisDatas[2]->setUnits(-115);
        mainTaskHandler->axisDatas[3]->setUnits(0);
        mainTaskHandler->axisDatas[4]->setUnits(0);
        mainTaskHandler->axisDatas[5]->setUnits(0);
        if(! mainTaskHandler->serialHandler->sendPositions())
        {
            
            serialIsOnline =false;
            wait =100;
            
        }else
        {
            
            
            serialIsOnline =true ;
        }
    }
    
    virtual void update()
    {
        
        if(serialIsOnline)
        {
            
            
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
        
        
        if(snapPos<1800){
            
            nextTask = std::make_shared<SnapTask>(snapPos+300);
        }
        
        Task::taskComplete();
        
    }
    
    void snapPicture()
    {
            std::shared_ptr<CaptureImage> capImage=  std::make_shared<CaptureImage>();
        capImage->mTexture =mainTaskHandler->mTexture;
          capImage->mSurface=mainTaskHandler->mSurface;
        capImage->posZ=snapPos;
        capImage->calculate();
        mainTaskHandler->floorCaptures.push_back(capImage);
    
    }
    
    float snapPos;
    bool serialIsOnline;
    int wait;
};

#endif
