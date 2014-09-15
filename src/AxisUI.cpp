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
    axisData = _axisData;
    
    gui = new ciUICanvas(x,y,200,400);
    gui->addWidgetDown(new ciUILabel(axisData->name, CI_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ciUISpacer(182, 2));
    /*gui->addWidgetDown(new ciUILabel("z axis", CI_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ciUISlider(182,20, 0, 250, bgColorR, "dist (mm)"));
    gui->addWidgetDown(new ciUILabelButton(85, false, "+", CI_UI_FONT_MEDIUM ,"w"));
    gui->addWidgetRight(new ciUILabelButton(85, false, "-", CI_UI_FONT_MEDIUM,"h"));*/

}
void AxisUI::update()
{



}
void AxisUI::draw()
{
    gui->draw();

}