//
//  GrabTask.h
//  NPBOT
//
//  Created by Kris Temmerman on 10/10/14.
//
//

#ifndef NPBOT_GrabTask_h
#define NPBOT_GrabTask_h
#include "Task.h"
#include "SnapTask.h"
#include "Cube.h"

class GrabTask:public Task
{
    
    public :
    GrabTask(): Task("GrabTask")
    {
        state =-1;
        saveY =300;
    }
    virtual void start();
    void startNextState();
   
    virtual void update();
 
    virtual void taskComplete();
 
    Cube *currentCube;
    int state;
    float saveY;
    bool serialIsOnline;
    int wait;
    Vec3f targetPos;
};

#endif
