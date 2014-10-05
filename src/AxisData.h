//
//  AxisData.h
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#ifndef __NPBOT__AxisData__
#define __NPBOT__AxisData__
#include "cinder/app/AppNative.h"

#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;


class AxisData
{
public:
    AxisData(){};
    
    void setup(int id, string name,float homePos, float _stepsToUnit, float _minUnits, float _maxUnits, string _unit, float offset );
    void setHome();
    void setUnits(float _unit);
    float targetStep;
    float currentStep;
    float homePos;
    float offset;
    float stepsToUnit;
    string unit;
    string name;
    int id;
    
    
    float minUnits;
    float maxUnits;
    
    float getTargetInUnits();
    float getCurrentInUnits();
    string getTargetForView();
    string getCurrentForView();
    bool isDirty;
    bool isHome;
    void addUnits(float unit);
};
#endif /* defined(__NPBOT__AxisData__) */
