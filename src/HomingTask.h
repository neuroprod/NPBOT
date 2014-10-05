//
//  HomingTask.h
//  NPBOT
//
//  Created by Kris Temmerman on 05/10/14.
//
//

#ifndef NPBOT_HomingTask_h
#define NPBOT_HomingTask_h
#include "Task.h"
#include "SnapTask.h"
class HomingTask:public Task
{

    public :
    HomingTask(): Task("HomingTask")
    {
        
    }


    virtual void start()
    {
    
        Task::start();
        
        if(! mainTaskHandler->serialHandler->sendHoming())
        {
           
            serialIsOnline =false;
            wait =100;
           
        }else
        {
        
        
            serialIsOnline =true ;
        }
      /*mainTaskHandler->axisDatas[0]->setUnits(1800);
         mainTaskHandler->axisDatas[1]->setUnits(116);
        mainTaskHandler->axisDatas[2]->setUnits(-115);
         mainTaskHandler->axisDatas[3]->setUnits(0);
         mainTaskHandler->axisDatas[4]->setUnits(0);
         mainTaskHandler->axisDatas[5]->setUnits(0);*/
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
       nextTask  = std::make_shared<SnapTask>(0);
        
        
        Task::taskComplete();
    
    }
    bool serialIsOnline;
    int wait;
};

#endif
