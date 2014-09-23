//
//  HandControler.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 23/09/14.
//
//

#include "HandControler.h"
void HandControler::setup( int x,int y)
{
    pos1 =90;
    pos2 =90;
    gui = new ciUICanvas(x,y,200,200);
    gui->addWidgetDown(new ciUILabelButton(100, false, "send", CI_UI_FONT_MEDIUM,"send"));
    gui->addWidgetDown(new ciUISlider(172,20, 45, 125, pos1, "hand1"));
    gui->addWidgetDown(new ciUISlider(172,20, 45, 125, pos2, "hand2"));
    gui->registerUIEvents(this, &HandControler::guiEvent);

}
void HandControler::update()
{
  
    
}
void HandControler::draw()
{
    gui->draw();
}
void HandControler::guiEvent(ciUIEvent *event)
{
    string name = event->widget->getName();
    
    if(name == "send")
    {
        serialHandler->sendHandPos(pos1,pos2);
    }else if(name =="hand2")
    {
        
        ciUISlider *slider = (ciUISlider *) event->widget;
        pos2= slider->getScaledValue();
        
    }else if(name=="hand1")
    {
        ciUISlider *slider = (ciUISlider *) event->widget;
        pos1= slider->getScaledValue();
    }

   
}