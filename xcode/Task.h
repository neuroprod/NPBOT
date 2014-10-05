//
//  Task.h
//  NPBOT
//
//  Created by Kris Temmerman on 05/10/14.
//
//

#ifndef NPBOT_Task_h
#define NPBOT_Task_h
#include "cinder/app/AppNative.h"
#include "MainTaskHandler.h"

using namespace ci;
using namespace ci::app;
using namespace std;
class MainTaskHandler;
class Task
{

public:
    Task(string _name){
        name=_name;
        isDone =false;
    
    }
    string name;
    bool isDone;
    virtual void start(){
    
        console()<<"starting task "<<name<<endl;
    }
    virtual void update()
    {
    
    
    }
    virtual void taskComplete()
    {
    
        isDone =true;
    }
    std::shared_ptr<Task > nextTask;
    MainTaskHandler * mainTaskHandler;
    
};

#endif
