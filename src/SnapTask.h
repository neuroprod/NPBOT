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

    virtual void start();
  
    
    virtual void update();
  
    virtual void taskComplete();
 
    
    void snapPicture();
    
    float snapPos;
    bool serialIsOnline;
    int wait;
};

#endif
