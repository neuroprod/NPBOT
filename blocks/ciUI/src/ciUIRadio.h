/********************************************************************************** 
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#ifndef CIUI_RADIO
#define CIUI_RADIO

#include "ciUIWidgetWithLabel.h"
#include "ciUIToggle.h"

class ciUIRadio : public ciUIWidgetWithLabel
{
public:
    ciUIRadio(float x, float y, float w, float h, string _name, vector<string> names, int _orientation)
    {
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _name, names, _orientation);         
    }

    ciUIRadio(float w, float h, string _name, vector<string> names, int _orientation)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _name, names, _orientation);         
    }    
    
    void init(float w, float h, string _name, vector<string> names, int _orientation)
    {
		name = _name; 		
		kind = CI_UI_WIDGET_RADIO; 		
        
        draw_back = false; 
        orientation = _orientation; 
		
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding*2.0f);
		paddedRect->setParent(rect); 
		
		label = new ciUILabel(0,0,(name+" LABEL"), name, CI_UI_FONT_MEDIUM); 
		label->setParent(label); 
		label->setRectParent(rect); 
        label->setEmbedded(true);
        
		for(unsigned int i = 0; i < names.size(); i++)
		{
			string tname = names[i]; 
			ciUIToggle *toggle = new ciUIToggle(0,0, w, h, false, tname); 
			toggles.push_back(toggle); 
		}
        allowMultiple = false; 
    }
        
    virtual void setDrawPadding(bool _draw_padded_rect)
	{
		draw_padded_rect = _draw_padded_rect; 
        label->setDrawPadding(false);
		for(unsigned int i = 0; i < toggles.size(); i++)
		{
			ciUIToggle *t = toggles[i]; 			
            t->setDrawPadding(false);             
        }           
	}
    
    virtual void setDrawPaddingOutline(bool _draw_padded_rect_outline)
	{
		draw_padded_rect_outline = _draw_padded_rect_outline; 
        label->setDrawPaddingOutline(false);
		for(unsigned int i = 0; i < toggles.size(); i++)
		{
			ciUIToggle *t = toggles[i]; 			
            t->setDrawPaddingOutline(false);             
        }        
	}  
    
	ciUILabel *getLabel()
	{
		return label; 
	}
	    
    void setVisible(bool _visible)
    {
        visible = _visible; 
        label->setVisible(visible); 
		for(unsigned int i = 0; i < toggles.size(); i++)
		{
			ciUIToggle *t = toggles[i]; 			
            t->setVisible(visible);             
        }
    }
	void activateToggle(string _name)
	{
		for(unsigned int i = 0; i < toggles.size(); i++)
		{
			ciUIToggle *t = toggles[i]; 			
			if(!(t->getName().compare(_name.c_str())))
			{
				t->setValue(true); 					
			}
			else 
			{
				t->setValue(false); 
			}			
		}
	}
	
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent;         
		rect->setParent(parent->getRect());
        
		float tWidth = label->getPaddingRect()->getWidth(); 
		float tHeight = label->getPaddingRect()->getHeight(); 

		float xt = 0; 
		float yt = label->getPaddingRect()->getHeight();
		
		for(unsigned int i = 0; i < toggles.size(); i++)
		{
			ciUIToggle *t = toggles[i]; 			
			t->setParent(this); 			
             t->getRect()->setParent(rect);
			
			if(orientation == CI_UI_ORIENTATION_HORIZONTAL)
			{
				t->getRect()->setX(xt); 
				t->getRect()->setY(yt); 				
				xt+=t->getPaddingRect()->getWidth(); 
				tHeight = label->getPaddingRect()->getHeight()+t->getPaddingRect()->getHeight(); 								
			}			
			else 
			{
				xt+=t->getPaddingRect()->getWidth(); 				
				t->getRect()->setY(yt); 			
				if(t->getPaddingRect()->getWidth() > tWidth)
				{
					tWidth = t->getPaddingRect()->getWidth(); 
				}
				tHeight+=t->getPaddingRect()->getHeight(); 				
				yt +=t->getPaddingRect()->getHeight(); 
			}			
		}
        
        if(orientation == CI_UI_ORIENTATION_HORIZONTAL)
        {            
            if(xt > tWidth)
            {
                tWidth = xt; 
            }
        }
        rect->setHeight(tHeight);
		tHeight += padding; 
		paddedRect->setWidth(tWidth); 	
		paddedRect->setHeight(tHeight); 			
	}	
	
	vector<ciUIToggle *> getToggles()
	{
		return toggles; 
	}
	
	void triggerEvent(ciUIWidget *child)
	{
        if(!allowMultiple)
        {
            activateToggle(child->getName().c_str()); 
        }
		if(parent != NULL)
		{
			parent->triggerEvent(child); 
		}
	}	
    
    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
	int orientation; 
	vector<ciUIToggle *> toggles; 		   
    bool allowMultiple;
}; 

#endif