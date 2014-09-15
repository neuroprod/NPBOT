#ifndef CIUI_LINE_SPACER
#define CIUI_LINE_SPACER

#include "ciUIWidget.h"

class ciUISpacer : public ciUIWidget
{
public:
    ciUISpacer(float x, float y, float w, float h)
    {
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h); 
    }
    
    ciUISpacer(float x, float y, float w, float h, string _name)
    {
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h); 
        name = _name; 
    }
    
    ciUISpacer(float w, float h)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h); 
    }    
    
    ciUISpacer(float w, float h, string _name)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h); 
        name = _name; 
    }        
    
    void init(float w, float h)
    {
        name = "CI_UI_SPACER"; 				
        kind = CI_UI_WIDGET_SPACER; 
        
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding*2.0f);
		paddedRect->setParent(rect);     
        
        draw_fill = true; 
        draw_back = false; 
    }
		
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
	}	
    
    void toggleColor()
    {
        draw_fill = !draw_fill; 
        draw_back = !draw_back; 
    }
    
    void setToggleColor(bool _value)
    {
        if(_value)
        {
            draw_fill = true; 
            draw_back = false; 
        }
        else
        {
            draw_fill = false; 
            draw_back = true;             
        }
    }
protected: 
}; 

#endif
