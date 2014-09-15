//
//  AxisData.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#include "AxisData.h"

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