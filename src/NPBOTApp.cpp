#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ciUI.h"
#include "AxisData.h"
#include "AxisUI.h"
#include "ArmViewport.h"
#include "PositionUI.h"
#include "ArmPosition.h"
#include "SerialHandler.h"

#include "ArmNode.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class NPBOTApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
     void guiEvent(ciUIEvent *event);
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
    
 
    ciUICanvas *gui;
    ArmViewport view1;
      ArmViewport view2;
      ArmViewport view3;
    
    ArmPosition armPosition;
    
    PositionUI xposUI;
    PositionUI yposUI;
    PositionUI zposUI;
    
    PositionUI xRotposUI;
    PositionUI yRotposUI;
    PositionUI zRotposUI;
    
    
    vector <PositionUI *> UIpositions;
    
    SerialHandler serialHandler;
    ArmNode * axis6Node;
    ArmNode * axis5Node;
    ArmNode * axis4Node;
    ArmNode * axis3Node;
    ArmNode * axis2Node;
    ArmNode * root;
    
    
};
void NPBOTApp::guiEvent(ciUIEvent *event)
{
    string name = event->widget->getName();
    if(name == "SEND")
    {
        serialHandler.sendPositions();
    }else if(name =="HOME")
    {
        
        serialHandler.sendHoming();
        
        
    }

}
void NPBOTApp::setup()
{
    setWindowSize(1920, 1160);
    setWindowPos(0, 0);
    gui = new ciUICanvas(10,10,300,200-40);
    gui->addWidgetRight(new ciUILabelButton(100, false, "SEND", CI_UI_FONT_MEDIUM ,"SEND"));
    gui->addWidgetRight(new ciUILabelButton(100, false, "HOME", CI_UI_FONT_MEDIUM,"HOME"));
      gui->registerUIEvents(this, &NPBOTApp::guiEvent);
    int posX=10;
    int posY =220-40;
    int stepY=210;
    int stepX=200;
    axis1.setup(0, "A1 z-axis", 75.0f/1600.0f, 0, 400000, "mm");
    axisUI1.setup(&axis1,posX,posY);
    UIAxxisses.push_back(&axisUI1);
    
    
    
  
    posX+=stepX;
    axis2.setup(0, "A2 arm 1", 400.0f* 50 /360, 0, 400000, "deg");
    axisUI2.setup(&axis2,posX,posY);
    UIAxxisses.push_back(&axisUI2);
    
    
    posX+=stepX;
    axis3.setup(0, "A3 arm2", 400.0f* 40 /360, 0, 400000, "deg");
    axisUI3.setup(&axis3,posX,posY);
    UIAxxisses.push_back(&axisUI3);
    
    posX=10;
    posY+=stepY;
    axis4.setup(0, "A4 pols rot",  400.0f* 10 /360, 0, 400000, "°deg");
    axisUI4.setup(&axis4,posX,posY);
    UIAxxisses.push_back(&axisUI4);
    
    posX+=stepX;
    axis5.setup(0, "A5 posl bend", 400.0f* 10 /360, 0, 400000, "deg");
    axisUI5.setup(&axis5,posX,posY);
    UIAxxisses.push_back(&axisUI5);
    
    posX+=stepX;
    axis6.setup(0, "A6 hand rot", 75.0f/1600.0f, 0, 400000, "deg");
    axisUI6.setup(&axis6,posX,posY);
    UIAxxisses.push_back(&axisUI6);
    
    
    
    
    
    
    
    axis6Node =new ArmNode();
    axis6Node->data =&axis6;
    axis6Node->setup(6,0,0,0,100,0,90,0);
    
    
    axis5Node =new ArmNode();
    axis5Node->data =&axis5;
    axis5Node->setup(5,0,50,0,0,0,-90,0);
    axis5Node->child = axis6Node;
    axis6Node->parent =axis5Node;
    
    axis4Node =new ArmNode();
    axis4Node->data =&axis4;
    axis4Node->setup(4,0,0,50,250,0,90,0);
    axis4Node->child = axis5Node;
    axis5Node->parent =axis4Node;
    
    axis3Node =new ArmNode();
    axis3Node->data =&axis3;
    axis3Node->setup(3,0,300,0,0);
    axis3Node->child = axis4Node;
    axis4Node->parent =axis3Node;
    
     axis2Node =new ArmNode();
    axis2Node->data =&axis2;
    axis2Node->setup(2,0, 0,250,0);
    axis2Node->child = axis3Node;
    axis3Node->parent =axis2Node;
    
    
    
    
    root =new ArmNode();
    root->data =&axis1;
    root->setup(1,1);
    root->child = axis2Node;
    axis2Node->parent =root;
    root->parent =nullptr;
    
    
    posX+=stepX;
    view1.setup(posX,10,1300,710);
    int vpY =710+20;
    view2.setup(posX,vpY,500,400);
    view3.setup(posX+510,vpY,1300-500-10,400);
    
    view1.root =root;
    view2.root =root;
    view3.root =root;
    
    
    armPosition.setup();
     posY+=stepY;
    
    stepY =90;
    
    xposUI.setup(&armPosition.x, &armPosition.targetX,10, posY, "X", 100, 0);
    UIpositions.push_back(&xposUI);
   
    posY+=stepY;
    yposUI.setup(&armPosition.y, &armPosition.targetY,10, posY, "Y", 100, 0);
    UIpositions.push_back(&yposUI);
    
    posY+=stepY;
    zposUI.setup(&armPosition.z, &armPosition.targetZ,10, posY, "Z",2000, 0);
    UIpositions.push_back(&zposUI);
    
    posY+=stepY;
    xRotposUI.setup(&armPosition.rotX, &armPosition.targetRotX,10, posY, "rot X", 100, 0);
    UIpositions.push_back(&xRotposUI);
    
    posY+=stepY;
    yRotposUI.setup(&armPosition.rotY, &armPosition.targetRotY,10, posY, "rot Y", 100, 0);
    UIpositions.push_back(&yRotposUI);
    
    posY+=stepY;
    zRotposUI.setup(&armPosition.rotZ, &armPosition.targetRotZ,10, posY, "rot Z", 100, 0);
    UIpositions.push_back(&zRotposUI);
    
    
    serialHandler.setup();
    serialHandler.axisData.push_back(&axis1);
    serialHandler.axisData.push_back(&axis2);
    serialHandler.axisData.push_back(&axis3);
    serialHandler.axisData.push_back(&axis4);
    serialHandler.axisData.push_back(&axis5);
    serialHandler.axisData.push_back(&axis6);
    
}

void NPBOTApp::mouseDown( MouseEvent event )
{
}

void NPBOTApp::update()
{
    serialHandler.update();
    for (int i=0;i<UIAxxisses.size();i++)
    {
        UIAxxisses[i]->update();
    }
    for (int i=0;i<UIpositions.size();i++)
    {
        UIpositions[i]->update();
    }
    root->update();
    view1.update();
    view2.update();
    view3.update();
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
    for (int i=0;i<UIpositions.size();i++)
    {
        UIpositions[i]->draw ();
    }
    view1.draw();
    view2.draw();
    view3.draw();
}

CINDER_APP_NATIVE( NPBOTApp, RendererGl )
