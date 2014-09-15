#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ciUI.h"
#include "AxisData.h"
#include "AxisUI.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class NPBOTApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    vector <AxisUI *> UIAxxisses;
    
    AxisData axis1;
    AxisUI axisUI1;
    
    
   // float bgColorR;
   // ciUICanvas *gui;
    
};

void NPBOTApp::setup()
{
    
    axis1.setup(0, "z axis", 1, 0, 400000, "mm");
  
    
    axisUI1.setup(&axis1,0,0);
    UIAxxisses.push_back(&axisUI1);
    
    
    
    
   /* gui = new ciUICanvas(0,0,200,400);
    bgColorR =10;
    gui->addWidgetDown(new ciUILabel("manual", CI_UI_FONT_LARGE));
    gui->addWidgetDown(new ciUISpacer(182, 2));
    gui->addWidgetDown(new ciUILabel("z axis", CI_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ciUISlider(182,20, 0, 250, bgColorR, "dist (mm)"));
    gui->addWidgetDown(new ciUILabelButton(85, false, "+", CI_UI_FONT_MEDIUM ,"w"));
    gui->addWidgetRight(new ciUILabelButton(85, false, "-", CI_UI_FONT_MEDIUM,"h"));
  
    */
}

void NPBOTApp::mouseDown( MouseEvent event )
{
}

void NPBOTApp::update()
{
    for (int i=0;i<UIAxxisses.size();i++)
    {
        UIAxxisses[i]->update();
    }
}

void NPBOTApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.3, 0.3, 0.3 ) );
     //gui->draw();
    
    
    for (int i=0;i<UIAxxisses.size();i++)
    {
        UIAxxisses[i]->draw();
    }
}

CINDER_APP_NATIVE( NPBOTApp, RendererGl )
