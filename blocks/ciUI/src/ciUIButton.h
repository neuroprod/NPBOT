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

#ifndef CIUI_BUTTON
#define CIUI_BUTTON

#include "ciUIWidgetWithLabel.h"

class ciUIButton : public ciUIWidgetWithLabel
{
public:    
    ciUIButton() {}
    
    ciUIButton(float x, float y, float w, float h, bool _value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = false; 
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, &_value, _name, _size);
    }
    
    ciUIButton(float w, float h, bool _value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = false;         
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, &_value, _name, _size);        
    }    

    ciUIButton(float x, float y, float w, float h, bool *_value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = true;         
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _value, _name, _size);
    }
    
    ciUIButton(float w, float h, bool *_value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = true;         
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _value, _name, _size);        
    }    

    ~ciUIButton()
    {
        if(!useReference)
        {
            delete value; 
        }
    }
    
    virtual void init(float w, float h, bool *_value, string _name, int _size)
    {
		name = _name; 		
		kind = CI_UI_WIDGET_BUTTON; 		
        		
        paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding*2.0f);
		paddedRect->setParent(rect); 
        
		label = new ciUILabel(w+padding*2.0f,0, (name+" LABEL"), name, _size); 
		label->setParent(label); 
		label->setRectParent(rect); 
        label->setEmbedded(true);		
        drawLabel = true;
        label->setVisible(drawLabel);      
        
        if(useReference)
        {
            value = _value; 
        }
        else
        {
            value = new bool(); 
            *value = *_value; 
        }
        
        setValue(*_value);         
    }
    
    virtual void update()
    {        
        draw_fill = *value;         
    }
        
    virtual void draw() 
    {        
        drawPadded();
        drawPaddedOutline();        
        
        drawBack();
        
        drawOutline();
        drawOutlineHighlight();
        
        drawFill();
        drawFillHighlight();        
    }
    
    virtual void setDrawPadding(bool _draw_padded_rect)
	{
		draw_padded_rect = _draw_padded_rect; 
        label->setDrawPadding(false);
	}
    
    virtual void setDrawPaddingOutline(bool _draw_padded_rect_outline)
	{
		draw_padded_rect_outline = _draw_padded_rect_outline; 
        label->setDrawPaddingOutline(false);
	}  

    virtual void mouseMove(int x, int y) 
    {
        if(rect->inside((float) x, (float) y))
        {
            state = CI_UI_STATE_OVER;         
        }    
        else
        {
            state = CI_UI_STATE_NORMAL;        
        }
        stateChange();         
    }
    
    virtual void mouseDrag(int x, int y, int button) 
    {
        if(hit)
        {
            if(rect->inside((float) x, (float) y))
            {                
                state = CI_UI_STATE_DOWN;         
            }    
            else                
            {
                hit = false;
                state = CI_UI_STATE_NORMAL;        
                setValue(false); 
                triggerEvent(this);
            }
            stateChange();     
        }
    }
    
    virtual void mouseDown(int x, int y, int button) 
    {
        if(rect->inside((float) x, (float) y))
        {
            hit = true;
            state = CI_UI_STATE_DOWN;         
            setValue(true); 
			triggerEvent(this); 			
        }    
        else
        {
            state = CI_UI_STATE_NORMAL;        
        }
        stateChange();         
    }
    
    virtual void mouseUp(int x, int y, int button) 
    {
        if(hit)
        {
#if defined( CINDER_COCOA_TOUCH )
            state = CI_UI_STATE_NORMAL;        
#else            
            if(rect->inside((float) x, (float) y))
            {
                state = CI_UI_STATE_OVER; 
            }
            else
            {
                state = CI_UI_STATE_NORMAL;                         
            }
#endif 
            setValue(false); 
			triggerEvent(this); 
        }    
        else
        {
            state = CI_UI_STATE_NORMAL;         
        }
        stateChange();         
        hit = false; 
    }
    
    virtual void stateChange()
    {                
        switch (state) {
            case CI_UI_STATE_NORMAL:
            {            
                draw_fill_highlight = false;             
                draw_outline_highlight = false;  
				label->unfocus(); 								
            }
                break;
            case CI_UI_STATE_OVER:
            {
                draw_fill_highlight = false;            
                draw_outline_highlight = true;  
				label->focus(); 								
            }
                break;
            case CI_UI_STATE_DOWN:
            {
                draw_fill_highlight = true;            
                draw_outline_highlight = false;             
				label->focus(); 					
            }
                break;
            case CI_UI_STATE_SUSTAINED:
            {
                draw_fill_highlight = false;            
                draw_outline_highlight = false;                         
				label->unfocus(); 								
            }
                break;            
                
            default:
                break;
        }        
    }
	
    virtual void setVisible(bool _visible)
    {
        visible = _visible; 
        label->setVisible(visible); 
    }
    
	ciUILabel *getLabel()
	{
		return label; 
	}
	
	virtual void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 

        rect->setParent(parent->getRect());
		ciUIRectangle *labelrect = label->getRect(); 
		float h = labelrect->getHeight(); 
		float ph = rect->getHeight(); 
		
		labelrect->setY(ph/2.0f - h/2.0f);
        
        if(!drawLabel)
        {
            paddedRect->setWidth(rect->getWidth()+padding*2.0f);
        }
        else
        {            
            paddedRect->setWidth(paddedRect->getWidth() + label->getPaddingRect()->getWidth()+padding);
        }
	}	
	
	bool getValue()
	{
		return *value; 
	}
    
    void setLabelVisible(bool _visible)
    {
        drawLabel = _visible; 
        label->setVisible(drawLabel); 
    }
	
    virtual void setValue(bool _value)
	{
		*value = _value;         
        draw_fill = *value; 
        label->setDrawBack(*value);         
	}
	
	void toggleValue() {
	  setValue(!(*value));
	}
    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
    bool *value; 
    bool useReference; 
    bool drawLabel; 
}; 

#endif
