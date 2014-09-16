//
//  AxisUI.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#include "AxisUI.h"
void AxisUI::setup(AxisData * _axisData,int x, int y)
{
    step =1;
    axisData = _axisData;
    
    gui = new ciUICanvas(x,y,190,200);
    gui->addWidgetDown(new ciUILabel(axisData->name, CI_UI_FONT_LARGE));

//    gui->addWidgetDown(new ciUILabel("current", CI_UI_FONT_SMALL));
    
    labelCurrent =(ciUILabel *) gui->addWidgetDown(new ciUILabel("a", CI_UI_FONT_MEDIUM));
    
     gui->addWidgetDown(new ciUILabel("target", CI_UI_FONT_SMALL));
    labelTarget =(ciUILabel *) gui->addWidgetDown(new ciUILabel("a", CI_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ciUISlider(172,20, 0, 100, step, "step"));
    gui->addWidgetDown(new ciUILabelButton(80, false, "+", CI_UI_FONT_MEDIUM ,"+"));
    gui->addWidgetRight(new ciUILabelButton(80, false, "-", CI_UI_FONT_MEDIUM,"-"));
  
     gui->registerUIEvents(this, &AxisUI::guiEvent);

}
void AxisUI::guiEvent(ciUIEvent *event)
{
    string name = event->widget->getName();
    if(name == "step")
    {
        ciUISlider *slider = (ciUISlider *) event->widget;
        step= slider->getScaledValue();
    }else if(name =="+")
    {
        
        axisData->addUnits(step);
    
    
    }else if(name=="-")
    {
        axisData->addUnits(-step);
    
    }
}
void AxisUI::update()
{
    labelCurrent->setLabel( axisData->getCurrentForView());

    labelTarget->setLabel(axisData->getTargetForView());
}
void AxisUI::draw()
{
    gui->draw();

}

