//
//  GrabTask.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 23/10/14.
//
//

#include "GrabTask.h"
 void GrabTask::start(){
    
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
void GrabTask::startNextState()
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
        mainTaskHandler->position->targetY=30;//currentCube->center.y-200;
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
        wait =60;
        float       pos =0.3;
        int pos1= (pos *80)+45;
        int pos2 =125-(pos *80);
        mainTaskHandler->serialHandler->sendHandPos(pos1,pos2);
        ///grab
    }else if(state==4)
    {
        mainTaskHandler->position->targetX=currentCube->center.x;
        mainTaskHandler->position->targetY=saveY;
        mainTaskHandler->position->targetZ=currentCube->center.z;
        mainTaskHandler->position->targetRotX =90;
        mainTaskHandler->position->targetRotY =-90;
        mainTaskHandler->position->setRotationsFromPositions();
        mainTaskHandler-> axisDatas[5]->setUnits(90);//invers coordiantes
        if(! mainTaskHandler->serialHandler->sendPositions())
        {
            serialIsOnline =false;
            wait =30;
            
        }else
        {
            serialIsOnline =true ;
        }
        
    }else if(state==5)
    {
        targetPos = mainTaskHandler->getNextPos();
        
        mainTaskHandler->position->targetX=targetPos.x;
        mainTaskHandler->position->targetY=saveY;
        mainTaskHandler->position->targetZ=targetPos.z;
        mainTaskHandler->position->targetRotX =90;
        mainTaskHandler->position->targetRotY =-90;
        mainTaskHandler->position->setRotationsFromPositions();
        mainTaskHandler-> axisDatas[5]->setUnits(90);//invers coordiantes
        if(! mainTaskHandler->serialHandler->sendPositions())
        {
            serialIsOnline =false;
            wait =30;
            
        }else
        {
            serialIsOnline =true ;
        }
        
    }else if(state==6)
    {
        mainTaskHandler->position->targetX=targetPos.x;
        mainTaskHandler->position->targetY=targetPos.y;
        mainTaskHandler->position->targetZ=targetPos.z;
        mainTaskHandler->position->targetRotX =90;
        mainTaskHandler->position->targetRotY =-90;
        mainTaskHandler->position->setRotationsFromPositions();
        mainTaskHandler-> axisDatas[5]->setUnits(90);//invers coordiantes
        if(! mainTaskHandler->serialHandler->sendPositions())
        {
            serialIsOnline =false;
            wait =30;
            
        }else
        {
            serialIsOnline =true ;
        }
        
    }else if(state==8)
    {
        mainTaskHandler->position->targetX=targetPos.x;
        mainTaskHandler->position->targetY=saveY;
        mainTaskHandler->position->targetZ=targetPos.z;
        mainTaskHandler->position->targetRotX =90;
        mainTaskHandler->position->targetRotY =-90;
        mainTaskHandler->position->setRotationsFromPositions();
        mainTaskHandler-> axisDatas[5]->setUnits(90);//invers coordiantes
        if(! mainTaskHandler->serialHandler->sendPositions())
        {
            serialIsOnline =false;
            wait =30;
            
        }else
        {
            serialIsOnline =true ;
        }
        
    }else if(state==7)
    {
        serialIsOnline =false;
        wait =60;
        float       pos =1;
        int pos1= (pos *80)+45;
        int pos2 =125-(pos *80);
        mainTaskHandler->serialHandler->sendHandPos(pos1,pos2);
        ///grab
        mainTaskHandler->cubes.pop_back();
       
    }else if (state==9)taskComplete();
    
}
void GrabTask::update()
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
    
    
}
 void GrabTask::taskComplete()
{
    if(mainTaskHandler->cubes.size()>0)
    {
        nextTask = std::make_shared<GrabTask>();
        
    }else
    {
        nextTask  = std::make_shared<SnapTask>(200);
        
    }
    isDone =true;
}
