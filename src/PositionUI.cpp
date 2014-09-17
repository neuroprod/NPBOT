//
//  PositionUI.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#include "PositionUI.h"
void PositionUI::setup(float * _value,float *_targetValue, int x,int y, string _name, int max,int min)
{
    name =_name;
    value =_value;
    targetValue =_targetValue;
    gui = new ciUICanvas(x,y,590,80);
    labelMain =new ciUILabel(name , CI_UI_FONT_LARGE);
    gui->addWidgetDown(labelMain);
    
    
    sliderMain =new ciUISlider(400,20, min, max, *targetValue, "target" );
    gui->addWidgetDown(sliderMain );
    gui->addWidgetRight(new ciUILabelButton(30, false, "+", CI_UI_FONT_MEDIUM ,"+"));
    gui->addWidgetRight(new ciUILabelButton(30, false, "-", CI_UI_FONT_MEDIUM,"-"));
    gui->addWidgetRight(new ciUISlider(90,20, 1, 45, 1, "value"));
}
void PositionUI::update(bool axisIsDirty)
{
    std::ostringstream out;
    out <<std::fixed << std::setprecision(4) <<*value;
    if( axisIsDirty){
    
    labelMain->setLabel(name +" " + out.str());
    sliderMain->setValue(*targetValue);
    }

}
void PositionUI::draw()
{
    gui->draw();
}
void PositionUI::guiEvent(ciUIEvent *event)
{

}