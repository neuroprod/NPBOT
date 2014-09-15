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

#ifndef CIUI_TOGGLE_MATRIX
#define CIUI_TOGGLE_MATRIX

#include "ciUIWidgetWithLabel.h"
#include "ciUIToggle.h"

class ciUIToggleMatrix : public ciUIWidgetWithLabel
{
public:
    ciUIToggleMatrix(float x, float y, float w, float h, int _rows, int _cols, string _name)
    {
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _rows, _cols, _name); 
    }
    
    ciUIToggleMatrix(float w, float h, int _rows, int _cols, string _name)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _rows, _cols, _name); 
    }    
    
    void init(float w, float h, int _rows, int _cols, string _name)
    {
        name = _name; 		
		kind = CI_UI_WIDGET_TOGGLEMATRIX; 		
		rows = _rows; 
        cols = _cols; 
        draw_back = false;  
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding*2.0f);
		paddedRect->setParent(rect); 
        
		label = new ciUILabel(0,0,(name+" LABEL"), name, CI_UI_FONT_SMALL); 
		label->setParent(label); 
		label->setRectParent(rect); 
        label->setEmbedded(true);
        
        Vec2f pos = Vec2f(0,0); 
		for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {                             
                ciUIToggle *toggle = new ciUIToggle(pos.x,pos.y, w, h, false, (name+"("+numToString(i,0)+","+numToString(j,0)+")")); 
                toggle->setLabelVisible(false); 
                toggles.push_back(toggle);  
                pos.x += w+padding; 
            }
            pos.y += h+padding; 
            pos.x = 0;
        }
        allowMultiple = true;  
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
            t->getLabelWidget()->setVisible(false);
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
		ciUIRectangle *labelrect = label->getRect(); 

        float tWidth = cols*(toggles[0]->getRect()->getWidth())+cols*padding; 
		float tHeight = rows*(toggles[0]->getRect()->getHeight())+rows*padding; 

        for(unsigned int i = 0; i < toggles.size(); i++)
		{
			ciUIToggle *t = toggles[i]; 			
			t->setParent(this); 
			t->getRect()->setParent(this->getRect()); 
        }
        
        labelrect->setX(0); 
        labelrect->setY(tHeight+padding); 

        tWidth+=padding; 
        if(label->getPaddingRect()->getWidth() > tWidth)
        {
            tWidth = label->getPaddingRect()->getWidth(); 
        }
        
		paddedRect->setWidth(tWidth); 	
		paddedRect->setHeight(tHeight+padding+label->getPaddingRect()->getHeight()); 			
	}	
    
    void setAllToggles(bool _value)
    {
		for(unsigned int i = 0; i < toggles.size(); i++)
		{
			ciUIToggle *t = toggles[i]; 			
            t->setValue(_value);
        }        
    }
    
    void setAllTogglesAndTrigger(bool _value)
    {
		for(unsigned int i = 0; i < toggles.size(); i++)
		{
			ciUIToggle *t = toggles[i]; 			
            t->setValue(_value);
        }        
        
        for(unsigned int i = 0; i < toggles.size(); i++)
		{
            triggerEvent(toggles[i]); 
        }        
    }
    
    void setToggle(int x, int y, bool _value)
    {
        if(x*y < toggles.size())
        {
            toggles[x+y*rows]->setValue(_value);
        }
    }
	
    void setToggleAndTrigger(int x, int y, bool _value)
    {
        setToggle(x,y,_value); 
        triggerEvent(toggles[x+y*rows]); 
        
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
    
    void setAllowMultiple(bool _allowMultiple)
    {
        allowMultiple = _allowMultiple; 
    }

protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
	vector<ciUIToggle *> toggles; 		   
    int rows, cols; 
    bool allowMultiple;     
}; 

#endif