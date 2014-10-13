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
#include "Cube.h"
class GrabTask:public Task
{
    
    public :
    GrabTask(): Task("GrabTask")
    {
        state =-1;
        saveY =200;
    }
    virtual void start(){
        
        Task::start();
        startNextState();
        //find cube
        //go above cube
        //takegrabPos
        //lower
        //grab
        //go_up safe
        //goto_target
        //godwn
        //gotosave
        //release
    }
    void startNextState()
    {
        state++;
        if(state==0)
        {
            if(mainTaskHandler->cubes.size()>0)
            {
                currentCube =  mainTaskHandler->cubes[mainTaskHandler->cubes.size()-1];
                startNextState();
            }else
            {
                taskComplete();
            }
        }else if(state==1)
        {
            
            mainTaskHandler->position->targetX=currentCube->center.x;
            mainTaskHandler->position->targetY=saveY;
             mainTaskHandler->position->targetZ=currentCube->center.z;
            mainTaskHandler->position->targetRotX =90;
            mainTaskHandler->position->targetRotY =-90;
            mainTaskHandler->position->setRotationsFromPositions();
            mainTaskHandler-> axisDatas[5]->setUnits(-currentCube->angle);//invers coordiantes
            if(! mainTaskHandler->serialHandler->sendPositions())
            {
                serialIsOnline =false;
                wait =30;
                
            }else
            {
                serialIsOnline =true ;
            }

        }else if(state==2)
        {
            
            mainTaskHandler->position->targetX=currentCube->center.x;
            mainTaskHandler->position->targetY=currentCube->center.y;
            mainTaskHandler->position->targetZ=currentCube->center.z;
            mainTaskHandler->position->targetRotX =90;
            mainTaskHandler->position->targetRotY =-90;
            mainTaskHandler->position->setRotationsFromPositions();
            mainTaskHandler-> axisDatas[5]->setUnits(-currentCube->angle);//invers coordiantes
            if(! mainTaskHandler->serialHandler->sendPositions())
            {
                serialIsOnline =false;
                wait =30;
                
            }else
            {
                serialIsOnline =true ;
            }
            
        }else if(state==3)
        {
            serialIsOnline =false;
            wait =30;
        ///grab
        }else if(state==4)
        {
            mainTaskHandler->position->targetX=currentCube->center.x;
            mainTaskHandler->position->targetY=saveY;
            mainTaskHandler->position->targetZ=currentCube->center.z;
            mainTaskHandler->position->targetRotX =90;
            mainTaskHandler->position->targetRotY =-90;
            mainTaskHandler->position->setRotationsFromPositions();
            mainTaskHandler-> axisDatas[5]->setUnits(0);//invers coordiantes
            if(! mainTaskHandler->serialHandler->sendPositions())
            {
                serialIsOnline =false;
                wait =30;
                
            }else
            {
                serialIsOnline =true ;
            }
        
        }
    
    }
    virtual void update()
    {
        if(serialIsOnline)
        {
            if( mainTaskHandler->serialHandler->isDone) startNextState();
            
        }else
        {
            wait--;
            if(wait==0)
            {
                
                startNextState();
            }
            
            
        }
        if(state==4)
        {
        
        
        }
        
    }
    virtual void taskComplete()
    {
        if(mainTaskHandler->cubes.size()>0)
        {
        
        
        }
        isDone =true;
    }
    Cube *currentCube;
    int state;
    float saveY;
    bool serialIsOnline;
    int wait;
};

#endif
