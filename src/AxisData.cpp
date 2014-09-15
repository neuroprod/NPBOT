//
//  AxisData.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#include "AxisData.h"
#include "cinder/Utilities.h"
void AxisData::setup(int _id, string _name, float _stepsToUnit, int _minSteps, int _maxSteps, string _unit )
{
    id =_id;
    name =_name;
    stepsToUnit =_stepsToUnit;
    minSteps =_minSteps;
    maxSteps =_maxSteps;
    unit = _unit;
    
    targetStep =0;
    currentStep =0;

}

void AxisData::addUnits(float unit)
{
    targetStep += unit /stepsToUnit;

}

float AxisData::getTargetInUnits()
{
    return round ((float)targetStep*stepsToUnit*100)/100;

}
float AxisData::getCurrentInUnits(){
    return  round ((float)currentStep*stepsToUnit*100)/100;
    
}
string AxisData::getTargetForView()
{
    std::ostringstream out;
    out <<std::fixed << std::setprecision(2) <<getTargetInUnits();
    return out.str()+" "+unit;
   
}
string AxisData::getCurrentForView()
{

    std::ostringstream out;
    out<<std::fixed  << std::setprecision(2) <<getCurrentInUnits();
    return out.str()+" "+unit;

}