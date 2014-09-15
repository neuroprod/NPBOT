#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ciUI.h"
#include "AxisData.h"
#include "AxisUI.h"
#include "ArmViewport.h"
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
    
    AxisData axis2;
    AxisUI axisUI2;
    
    AxisData axis3;
    AxisUI axisUI3;
    
    AxisData axis4;
    AxisUI axisUI4;
    
    AxisData axis5;
    AxisUI axisUI5;
    
    AxisData axis6;
    AxisUI axisUI6;
    
   // float bgColorR;
   ciUICanvas *gui;
    ArmViewport view1;
};

void NPBOTApp::setup()
{
    
    gui = new ciUICanvas(10,10,190,215);

    
    int posX=10;
    int posY =200;
    int stepY=225;
    int stepX=200;
    axis1.setup(0, "A1 z-axis", 75.0f/1600.0f, 0, 400000, "mm");
    axisUI1.setup(&axis1,posX,posY);
    UIAxxisses.push_back(&axisUI1);
    
    
    
  
    posX+=stepX;
    axis2.setup(0, "A2 arm 1", 75.0f/1600.0f, 0, 400000, "deg");
    axisUI2.setup(&axis2,posX,posY);
    UIAxxisses.push_back(&axisUI2);
    
    posX=10;
    posY+=stepY;
    
    axis3.setup(0, "A3 arm2", 75.0f/1600.0f, 0, 400000, "deg");
    axisUI3.setup(&axis3,posX,posY);
    UIAxxisses.push_back(&axisUI3);
    
    posX+=stepX;
    axis4.setup(0, "A4 pols rot", 75.0f/1600.0f, 0, 400000, "°deg");
    axisUI4.setup(&axis4,posX,posY);
    UIAxxisses.push_back(&axisUI4);
    
     posX=10;
    posY+=stepY;
    axis5.setup(0, "A5 posl bend", 75.0f/1600.0f, 0, 400000, "deg");
    axisUI5.setup(&axis5,posX,posY);
    UIAxxisses.push_back(&axisUI5);
    
    posX+=stepX;
    axis6.setup(0, "A6 hand rot", 75.0f/1600.0f, 0, 400000, "deg");
    axisUI6.setup(&axis6,posX,posY);
    UIAxxisses.push_back(&axisUI6);
    
    posX+=stepX;
    view1.setup(posX,10,500,500);
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
    
    view1.update();
}

void NPBOTApp::draw()
{
    
     glViewport (0  , 0 , getWindowWidth(), getWindowHeight());
    gl::setMatricesWindow(getWindowWidth(), getWindowHeight());
	// clear out the window with black
	gl::clear( Color( 0.2, 0.2, 0.2 ) );
     gui->draw();
    
    
    for (int i=0;i<UIAxxisses.size();i++)
    {
        UIAxxisses[i]->draw();
    }
    
    view1.draw();
}

CINDER_APP_NATIVE( NPBOTApp, RendererGl )
