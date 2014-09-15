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

#ifndef CIUI_CIRCLE_SLIDER
#define CIUI_CIRCLE_SLIDER

#include "ciUISlider.h"

class ciUICircleSlider : public ciUISlider
{
public:    
    ciUICircleSlider(float x, float y, float w, float _min, float _max, float _value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = false;         
        rect = new ciUIRectangle(x,y,w,w); 
        init(w, w, _min, _max, &_value, _name, _size); 		
    }
    
    ciUICircleSlider(float w, float _min, float _max, float _value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = false;                 
        rect = new ciUIRectangle(0,0,w,w); 
        init(w, w, _min, _max, &_value, _name, _size); 
    }        

    ciUICircleSlider(float x, float y, float w, float _min, float _max, float *_value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = true;                 
        rect = new ciUIRectangle(x,y,w,w); 
        init(w, w, _min, _max, _value, _name, _size); 		
    }
    
    ciUICircleSlider(float w, float _min, float _max, float *_value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = true;                 
        rect = new ciUIRectangle(0,0,w,w); 
        init(w, w, _min, _max, _value, _name, _size); 
    }        
    
    void init(float w, float h, float _min, float _max, float *_value, string _name, int _size)
    {
        name = _name; 				
        kind = CI_UI_WIDGET_CIRCLESLIDER;
        
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding);
		paddedRect->setParent(rect);     
        
        draw_fill = true; 
        
        value = *_value;                                               //the widget's value
        if(useReference)
        {
            valueRef = _value; 
        }
        else
        {
            valueRef = new float(); 
            *valueRef = value; 
        }

		max = _max; 
		min = _min; 
        labelPrecision = 2;
        
		if(value > max)
		{
			value = max; 
		}
		if(value < min)
		{
			value = min; 
		}
		
		value = cinder::lmap<float>(value, min, max, 0.0, 1.0); 
                
        label = new ciUILabel(0,w+padding,(name+" LABEL"), name, _size); 	
        label->setDrawBack(true);        
		label->setParent(label); 
		label->setRectParent(rect); 	
        label->setEmbedded(true);        
        increment = 0.01;    
        inputDirection = CI_UI_DIRECTION_SOUTHNORTH;
    }
    
    void drawBack() 
    {
        if(draw_back)
        {
            cinder::gl::color(color_back); 
            cinder::gl::drawSolidCircle(Vec2f(rect->getX()+rect->getHalfWidth(), rect->getY()+rect->getHalfHeight()),rect->getHalfWidth());
        }
    }
    
    void drawOutline() 
    {
        if(draw_outline)
        {
            cinder::gl::color(color_outline); 
            cinder::gl::drawStrokedCircle(Vec2f(rect->getX()+rect->getHalfWidth(), rect->getY()+rect->getHalfHeight()),rect->getHalfWidth());
        } 
    }
    
    virtual void drawFill()
    {
        if(draw_fill)
        {			
            cinder::gl::color(color_fill); 
            cinder::gl::drawSolidCircle(Vec2f(rect->getX()+rect->getHalfWidth(), rect->getY()+rect->getHalfHeight()),value*rect->getHalfWidth());
        }
    }
    
    virtual void drawFillHighlight()
    {
        if(draw_fill_highlight)
        {
            cinder::gl::color(color_fill_highlight);         
            cinder::gl::drawSolidCircle(Vec2f(rect->getX()+rect->getHalfWidth(), rect->getY()+rect->getHalfHeight()),value*rect->getHalfWidth());
            cinder::gl::color(label->getColorFillHighlight());                         
            label->drawString(rect->getX()+rect->getWidth()+padding, rect->getHalfHeight()+rect->getY()-label->getRect()->getHalfHeight(), numToString(getScaledValue(), labelPrecision));
            
        }        
    }
    
    virtual void drawOutlineHighlight()
    {
        if(draw_outline_highlight)
        {
            cinder::gl::color(color_outline_highlight); 
            cinder::gl::drawStrokedCircle(Vec2f(rect->getX()+rect->getHalfWidth(), rect->getY()+rect->getHalfHeight()),rect->getHalfWidth());
            if(!draw_fill_highlight)
            {
                cinder::gl::color(label->getColorFill()); 
                label->drawString(rect->getX()+rect->getWidth()+padding, rect->getHalfHeight()+rect->getY()-label->getRect()->getHalfHeight(), numToString(getScaledValue(), labelPrecision));
            }
        }
    }
    
    void mouseDrag(int x, int y, int button) 
    {
        if(hit)
        {            
            switch(inputDirection)
            {
                case CI_UI_DIRECTION_NORTHSOUTH:
                    value -= increment*(hitPoint.y-y); 
                    valueClamp();                 
                    break;
                case CI_UI_DIRECTION_SOUTHNORTH:
                    value += increment*(hitPoint.y-y);
                    valueClamp(); 
                    break;                    
                case CI_UI_DIRECTION_EASTWEST:
                    value += increment*(hitPoint.x-x);
                    valueClamp(); 
                    break;
                case CI_UI_DIRECTION_WESTEAST:
                    value -= increment*(hitPoint.x-x); 
                    valueClamp();                     
                    break;
            }
            updateValueRef();
            hitPoint = Vec2f((float) x, (float) y);    
			triggerEvent(this);             
            state = CI_UI_STATE_DOWN;         
        }    
        else
        {
            state = CI_UI_STATE_NORMAL;               
        }
        stateChange();     
    }
    
    void mouseDown(int x, int y, int button) 
    {
        if(rect->inside((float) x, (float) y))
        {
            hit = true; 
            hitPoint = Vec2f((float) x, (float) y); 
            state = CI_UI_STATE_DOWN;     
			triggerEvent(this);            
        }    
        else
        {
            state = CI_UI_STATE_NORMAL;        
        }
        stateChange();         
    }
    
    void mouseUp(int x, int y, int button) 
    {
        if(hit)
        {
#if defined( CINDER_COCOA_TOUCH )
            state = CI_UI_STATE_NORMAL;        
#else            
            state = CI_UI_STATE_OVER; 
#endif 
			triggerEvent(this); 
        }    
        else
        {
            state = CI_UI_STATE_NORMAL;         
        }
        stateChange();   
        hit = false; 
    }
    
    void valueClamp()
    {
        if(value > 1.0)
        {
            value = 1.0;
        }
        else if(value < 0.0)
        {
            value = 0.0;
        }                        
    }
    
    void setInputDirection(ciUIWidgetInputDirection _inputDirection)
    {
        inputDirection = _inputDirection; 
    }
    
    void updateLabel()
	{
        
	}
    
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
		paddedRect->setHeight(paddedRect->getHeight() + label->getPaddingRect()->getHeight()); 

		ciUIRectangle *labelrect = label->getRect(); 
        
        while(labelrect->getWidth() > rect->getWidth())
        {
            string labelstring = label->getLabel();
            string::iterator it;
            it=labelstring.end();
            it--; 
            labelstring.erase (it); 
            label->setLabel(labelstring);
        }                    
        
        float w = labelrect->getWidth(); 
        float pw = rect->getWidth(); 
        
        labelrect->setX((int)(pw*.5f - w*.5f-padding*.5f)); 
    }	
    
protected:
    ciUIWidgetInputDirection inputDirection; 
    Vec2f hitPoint;     
}; 

#endif
