//
//  AxisData.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#include "AxisData.h"
#include "cinder/Utilities.h"
void AxisData::setup(int _id, string _name,float _homePos, float _stepsToUnit, int _minSteps, int _maxSteps, string _unit ,float _offset)
{
    homePos =_homePos;
    id =_id;
    name =_name;
    stepsToUnit =_stepsToUnit;
    minSteps =_minSteps;
    maxSteps =_maxSteps;
    unit = _unit;
    offset=_offset;
    targetStep =0;
    currentStep =0;
    isHome =false;
    isDirty =true;
    targetStep = currentStep = (homePos+offset) *stepsToUnit;
    console()<< name<<" target: " << targetStep <<endl;
}
void AxisData::setHome()
{

    isDirty =true;
    
    targetStep = currentStep = (homePos+offset) *stepsToUnit;

}
void AxisData::setUnits(float _unit)
{
    isDirty =true;
 targetStep  = (_unit+offset) *stepsToUnit;

}
void AxisData::addUnits(float _unit)
{
    targetStep += _unit *stepsToUnit;
      console()<< name<<" targetcurrent: " << targetStep <<endl;

}

float AxisData::getTargetInUnits()
{
    return targetStep/stepsToUnit -offset;

}
float AxisData::getCurrentInUnits(){
    return  currentStep/stepsToUnit -offset;
    
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