#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ciUI.h"
#include "AxisData.h"
#include "AxisUI.h"
#include "ArmViewport.h"
#include "PositionUI.h"
#include "ArmPosition.h"
#include "SerialHandler.h"
#include "HandControler.h"
#include "ArmNode.h"
#include "MainTaskHandler.h"
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
    vector <AxisData *> axisDatas;
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
    
    HandControler handControler;
    
    MainTaskHandler mainTaskHandler;
    bool isMainTask;
};
void NPBOTApp::guiEvent(ciUIEvent *event)
{
    string name = event->widget->getName();
    cout<<" -->"<<name<<endl;;
    if(name == "SEND")
    {
        serialHandler.sendPositions();
    }else if(name =="HOME")
    {
        
        serialHandler.sendHoming();
        
        
    }else if(name =="Neutral")
    {
        
        
        axis1.setUnits(1800);
         axis2.setUnits(116);
        axis3.setUnits(-115);
        axis4.setUnits(0);
         axis5.setUnits(0);
         axis6.setUnits(0);
    }else if(name =="mainTask")
    {
        
        isMainTask =true;
        mainTaskHandler.start();
      /*
        axis1.setUnits(1400);
        axis2.setUnits(36);
        axis3.setUnits(-105);
        axis4.setUnits(-65);
        axis5.setUnits(80);
        axis6.setUnits(0);*/
    }

}



void NPBOTApp::setup()
{
   isMainTask =false;
    setWindowSize(1920, 1160);
    setWindowPos(0, 0);
    gui = new ciUICanvas(10,10,300-20,200-40);
    gui->addWidgetRight(new ciUILabelButton(100, false, "HOME", CI_UI_FONT_MEDIUM,"HOME"));
    gui->addWidgetRight(new ciUILabelButton(100, false, "SEND", CI_UI_FONT_MEDIUM ,"SEND"));
    
    gui->addWidgetDown(new ciUILabelButton(100, false, "Neutral", CI_UI_FONT_MEDIUM,"neutral"));
    gui->addWidgetDown (new ciUILabelButton(100, false, "mainTask", CI_UI_FONT_MEDIUM,"mainTask"));
     // gui->addWidgetRight(new ciUILabelButton(100, false, "prestep2", CI_UI_FONT_MEDIUM,"prestep2"));
    gui->registerUIEvents(this, &NPBOTApp::guiEvent);
    
    
    
    handControler.setup(300,10);
    
    int posX=10;
    int posY =220-40;
    int stepY=210;
    int stepX=200;
    axis1.setup(0, "A1 z-axis",1900, 1600.0f/75.5f, 0, 1900, "mm",0);
    axisUI1.setup(&axis1,posX,posY);
    UIAxxisses.push_back(&axisUI1);
    
    
    
  
    posX+=stepX;
    axis2.setup(0, "A2 arm 1",161.5 ,400.0f* 40 /360,0, 161.5, "deg",0);
    axisUI2.setup(&axis2,posX,posY);
    UIAxxisses.push_back(&axisUI2);
    
    
    posX+=stepX;
    axis3.setup(0, "A3 arm2",-145, 400.0f* 50 /360, -145, 0, "deg",180);
    axisUI3.setup(&axis3,posX,posY);
    UIAxxisses.push_back(&axisUI3);
    
    posX=10;
    posY+=stepY;
    axis4.setup(0, "A4 pols rot",-100,  400.0f* 10 /360, -100, 100, "deg",100);
    axisUI4.setup(&axis4,posX,posY);
    UIAxxisses.push_back(&axisUI4);
    
    posX+=stepX;
    axis5.setup(0, "A5 posl bend",-105, 400.0f* 10 /360, -105, 100, "deg",105);
    axisUI5.setup(&axis5,posX,posY);
    UIAxxisses.push_back(&axisUI5);
    
    posX+=stepX;
    axis6.setup(0, "A6 hand rot",0 ,1600.0f/360.0f, 0, 360, "deg",0);
    axisUI6.setup(&axis6,posX,posY);
    UIAxxisses.push_back(&axisUI6);
    
    
    
    axisDatas.push_back(&axis1);
    axisDatas.push_back(&axis2);
    axisDatas.push_back(&axis3 );
    axisDatas.push_back(&axis4);
    axisDatas.push_back(&axis5);
    axisDatas.push_back(&axis6);
    
    
    axis6Node =new ArmNode();
    axis6Node->data =&axis6;
    axis6Node->setup(6,0,0,0,270,0,90,0);
    
    
    axis5Node =new ArmNode();
    axis5Node->data =&axis5;
    axis5Node->setup(5,0,80,0,0,0,-90,0);
    axis5Node->child = axis6Node;
    axis6Node->parent =axis5Node;
    
    axis4Node =new ArmNode();
    axis4Node->data =&axis4;
    //384
    axis4Node->setup(4,0,0,39,384-80,0,90,0);
    axis4Node->child = axis5Node;
    axis5Node->parent =axis4Node;
    
    axis3Node =new ArmNode();
    axis3Node->data =&axis3;
    axis3Node->setup(3,0,350,0,0);
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
    view1.setup(posX,10,1300,610);
    int vpY =610+20;
    view2.setup(posX,vpY,500,500);
    view2.setView(3);
    view3.setup(posX+510,vpY,1300-500-10,500);
    view3.setView(2);
    view1.root =root;
    view2.root =root;
    view3.root =root;
    
    
    
    armPosition.setup();
    view1.position =&armPosition;
    view2.position =&armPosition;
    view3.position =&armPosition;
    
    
    posY+=stepY;
    
    stepY =90;
    
    xposUI.setup(&armPosition.x, &armPosition.targetX,10, posY, "X", 1000, -1000);
    UIpositions.push_back(&xposUI);
   
    posY+=stepY;
    yposUI.setup(&armPosition.y, &armPosition.targetY,10, posY, "Y", 1000, -1000);
    UIpositions.push_back(&yposUI);
    
    posY+=stepY;
    zposUI.setup(&armPosition.z, &armPosition.targetZ,10, posY, "Z",2000, 0);
    UIpositions.push_back(&zposUI);
    
    posY+=stepY;
    xRotposUI.setup(&armPosition.rotX, &armPosition.targetRotX,10, posY, "rot X", 90, -90);
    UIpositions.push_back(&xRotposUI);
    
    posY+=stepY;
    yRotposUI.setup(&armPosition.rotY, &armPosition.targetRotY,10, posY, "rot Y", 90, -90);
    UIpositions.push_back(&yRotposUI);
    
    posY+=stepY;
    zRotposUI.setup(&armPosition.rotZ, &armPosition.targetRotZ,10, posY, "rot Z", 360, -360);
    UIpositions.push_back(&zRotposUI);
    
    
    serialHandler.setup();
    serialHandler.axisData.push_back(&axis1);
    serialHandler.axisData.push_back(&axis2);
    serialHandler.axisData.push_back(&axis3);
    serialHandler.axisData.push_back(&axis4);
    serialHandler.axisData.push_back(&axis5);
    serialHandler.axisData.push_back(&axis6);
    
    //// start pos
    root->update();
    
    handControler.serialHandler =&serialHandler;

    
    
     armPosition.axisDatas.push_back(&axis1);
     armPosition.axisDatas.push_back(&axis2);
     armPosition.axisDatas.push_back(&axis3 );
     armPosition.axisDatas.push_back(&axis4);
     armPosition.axisDatas.push_back(&axis5);
     armPosition.axisDatas.push_back(&axis6);
    armPosition.rootNode =root;
    armPosition.endNode =axis6Node;
    armPosition.setPositionsFromRotations();
    
    
    
    
    
    ///////////////
    
    mainTaskHandler.root =root;
    mainTaskHandler.axisDatas = axisDatas;
    mainTaskHandler.position =&armPosition;
    mainTaskHandler.serialHandler =&serialHandler;
    mainTaskHandler.setup();
}

void NPBOTApp::mouseDown( MouseEvent event )
{
}

void NPBOTApp::update()
{
    serialHandler.update();
    serialHandler.updateHand();
    bool axisIsDirty =false;
    
    for (int i=0;i<UIAxxisses.size();i++)
    {
        if(UIAxxisses[i]->axisData->isDirty){
           axisIsDirty =true;
            UIAxxisses[i]->axisData->isDirty =false;
        }
    }
    
    if(axisIsDirty)
    {
        root->update();
        armPosition.setPositionsFromRotations();
    
    }
    
 
    bool positionIsDirty =false;
    
    for (int i=0;i< UIpositions.size();i++)
    {
        if( UIpositions[i]->isDirty){
            positionIsDirty =true;
            UIpositions[i]->isDirty =false;
        }
    }
    
    
    
    if(positionIsDirty )
    {
    
    
        armPosition.setRotationsFromPositions();
    }
    
    
    
    
    
    for (int i=0;i<UIAxxisses.size();i++)
    {
        UIAxxisses[i]->update();
    }
    for (int i=0;i<UIpositions.size();i++)
    {
        UIpositions[i]->update(axisIsDirty);
    }
    root->update();
    
    
    
    
    if(isMainTask){
        
        mainTaskHandler.update();

    }
    else {
    view1.update();
    view2.update();
        view3.update();
    }
}

void NPBOTApp::draw()
{
    
     glViewport (0  , 0 , getWindowWidth(), getWindowHeight());
    gl::setMatricesWindow(getWindowWidth(), getWindowHeight());
	// clear out the window with black
	gl::clear( Color( 0.2, 0.2, 0.2 ) );
    
    if(isMainTask)
    {
        gl::clear( Color( 0.0, 0.0, 0.0 ) );
      mainTaskHandler.draw();
    
    
    }
    else
    {
    
    
     gui->draw();
    
    handControler.draw();
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
}

CINDER_APP_NATIVE( NPBOTApp, RendererGl )
