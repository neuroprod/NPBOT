//
//  ArmViewport.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 15/09/14.
//
//

#include "ArmViewport.h"
void ArmViewport::setup(int _x,int _y,int _w,int _h)
{

    x= _x;
    y=_y;
    w=_w;
    h=_h;
    gui = new ciUICanvas(x+2,y+2,w-10,200);
    gui->setColorBack(ColorA(1,0,0,0));
    gui->setColorFill(ColorA(1,0,0,1));
    gui->setColorFillHighlight(ColorA(1,0,0,1));
    
    vector<string> names;
    names.push_back("FRONT");    names.push_back("LEFT");    names.push_back(" TOP ");    names.push_back("F_T_L");
    
    
    ddl  = new ciUIDropDownList(100, "FRONT", names, CI_UI_FONT_SMALL);
    ddl->setAutoClose(true);
    ddl->setColorFillHighlight(ColorA(1,0,0,1));
    ddl->setColorFill(ColorA(1,1,0,0));
    gui->addWidgetDown(ddl);
    gui->addWidgetRight(new ciUIToggle(20,20,true, "target", CI_UI_FONT_SMALL));
       gui->addWidgetRight(new ciUIToggle(20,20,true, "current", CI_UI_FONT_SMALL));
       gui->addWidgetRight(new ciUIToggle(20,20,true, "path", CI_UI_FONT_SMALL));
    gui->registerUIEvents(this, &ArmViewport::guiEvent);

}
void ArmViewport::guiEvent(ciUIEvent *event)
{
    string name = event->widget->getName();
    
   // cout << "WIDGET NAME: " << name << endl;
    
    if(event->widget == ddl )
    {
        ciUIDropDownList *ddlist = (ciUIDropDownList *) event->widget;
        vector<ciUIWidget *> &selected = ddlist->getSelected();
        for(int i = 0; i < selected.size(); i++)
        {
           // cout << "SELECTED: " << selected[i]->getName() << endl;
            ddlist->getLabel() ->setLabel(selected[i]->getName());
            //(selected[i]->getName());
            //ddlist->s
            ddlist->getLabel()->unfocus();
        }
    }
}
void ArmViewport::update()
{

}
void ArmViewport::draw()
{
     viewArea.set(x,getWindowHeight()- y, x+w,getWindowHeight()-y-h);
    gl::setViewport(viewArea);
   // gl::clear( Color( 0.3, 0.0, 0.3 ) );
    gl::setMatricesWindow(w, h);

    gl::pushMatrices();
    gl::color(0.5, 0.5, 0.5);
    gl::drawSolidRect(Rectf( 0,0,w,h));
    gl::color(1, 1, 1);
    gl::popMatrices();
    
    
    gl::pushMatrices();
    //  gl::setMatrices(camera1 );
    // drawScene();
    
    
    gl::popMatrices();
    
    glViewport (0  , 0 , getWindowWidth(), getWindowHeight());
    gl::setMatricesWindow(getWindowWidth(), getWindowHeight());

    gui->draw();
    
    
   
    

}
