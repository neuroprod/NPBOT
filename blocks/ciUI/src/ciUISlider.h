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

#ifndef CIUI_SLIDER
#define CIUI_SLIDER

#include "cinder/Utilities.h"
#include "cinder/CinderMath.h"
#include "cinder/app/KeyEvent.h"
#include "ciUIWidgetWithLabel.h"

class ciUISlider : public ciUIWidgetWithLabel
{
public:
    ciUISlider() {}

    ciUISlider(float x, float y, float w, float h, float _min, float _max, float _value, string _name)
    {
        useReference = false;         
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _min, _max, &_value, _name); 		
    }
    
    ciUISlider(float w, float h, float _min, float _max, float _value, string _name)
    {
        useReference = false;         
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _min, _max, &_value, _name); 
    }    
    
    ciUISlider(float x, float y, float w, float h, float _min, float _max, float *_value, string _name)
    {
        useReference = true; 
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _min, _max, _value, _name); 		
    }
    
    ciUISlider(float w, float h, float _min, float _max, float *_value, string _name)
    {
        useReference = true; 
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _min, _max, _value, _name); 
    }        
        
    ~ciUISlider()
    {
        if(!useReference)
        {
            delete valueRef; 
        }
    }
    
    virtual void init(float w, float h, float _min, float _max, float *_value, string _name)
    {
        name = _name; 				
		if(w > h)
		{
			kind = CI_UI_WIDGET_SLIDER_H;  			
		}
		else 
		{
			kind = CI_UI_WIDGET_SLIDER_V;  			
		}
        
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
        
		if(kind == CI_UI_WIDGET_SLIDER_H)
		{
			label = new ciUILabel(0,h+padding,(name+" LABEL"), (name + ": "), CI_UI_FONT_SMALL); 
		}
		else 
		{
			label = new ciUILabel(0,h+padding,(name+" LABEL"), name, CI_UI_FONT_SMALL); 	
		}
        
		label->setParent(label); 
		label->setRectParent(rect); 
        label->setEmbedded(true);
        increment = 0.1f;         
    }

    virtual void update()
    {
        if(useReference)
            value = cinder::lmap<float>(*valueRef, min, max, 0.0, 1.0);
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
    
    virtual void drawBack() 
    {
        if(draw_back)
        {
            cinder::gl::color(color_back); 
            rect->draw(); 
        }
    }
    
    virtual void drawOutline() 
    {
        if(draw_outline)
        {
            cinder::gl::color(color_outline); 
            rect->drawOutline(); 
        }
    }
    
    virtual void drawOutlineHighlight() 
    {
        if(draw_outline_highlight)
        {
            cinder::gl::color(color_outline_highlight);              
            rect->drawOutline();          
        }
    }    
    
    virtual void drawFill() 
    {
        if(draw_fill)
        {			
            cinder::gl::color(color_fill);              
			if(kind == CI_UI_WIDGET_SLIDER_H)
			{			   
                cinder::gl::drawSolidRect(Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth()*value, rect->getY()+rect->getHeight()));
			}
			else 
			{
				cinder::gl::drawSolidRect(Rectf(rect->getX(), rect->getY()+rect->getHeight()*(1.0f-value), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight())); 
			}
        }
    }
    
    virtual void drawFillHighlight() 
    {
        if(draw_fill_highlight)
        {
            cinder::gl::color(color_fill_highlight);              
			if(kind == CI_UI_WIDGET_SLIDER_H)
			{			   
                cinder::gl::drawSolidRect(Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth()*value, rect->getY()+rect->getHeight()));
			}
			else 
			{
				cinder::gl::drawSolidRect(Rectf(rect->getX(), rect->getY()+rect->getHeight()*(1.0f-value), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight())); 
				label->drawString(rect->getX()+rect->getWidth()+padding, rect->getY()+rect->getHeight()*(1.0f-value)-label->getRect()->getHalfHeight(), numToString(getScaledValue(),labelPrecision)); 
			}
        }        
    }
        
    void mouseMove(int x, int y ) 
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
    
    void mouseDrag(int x, int y, int button) 
    {
        if(hit)
        {
            state = CI_UI_STATE_DOWN;     
			input((float) x, (float) y); 
			triggerEvent(this); 			
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
            state = CI_UI_STATE_DOWN;     
			input((float) x, (float) y); 
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
			input((float) x, (float) y); 
			triggerEvent(this); 			
        }    
        else
        {
            state = CI_UI_STATE_NORMAL;         
        }
        stateChange();    
        hit = false; 
    }
	
    void keyDown( KeyEvent &event )
    {		
		if(state == CI_UI_STATE_OVER)
		{
			switch (event.getCode()) 
			{
				case ci::app::KeyEvent::KEY_RIGHT:
					setValue(getScaledValue()+increment); 
					triggerEvent(this); 
					break;
                    
				case ci::app::KeyEvent::KEY_UP:
					setValue(getScaledValue()+increment); 
					triggerEvent(this); 
					break;
					
				case ci::app::KeyEvent::KEY_LEFT:
					setValue(getScaledValue()-increment); 					
					triggerEvent(this); 
					break;
                    
				case ci::app::KeyEvent::KEY_DOWN:
					setValue(getScaledValue()-increment); 					
					triggerEvent(this); 
					break;					
                    
				default:
					break;
			}
		}
    }
    
    void keyUp( KeyEvent &event )
    {
		
    }
	
    void windowResized(int w, int h) 
    {
		
    }
    
    float getIncrement()
    {
        return increment; 
    }
    
    void setIncrement(float _increment)
	{
		increment = _increment; 
	}
    
	virtual void input(float x, float y)
	{
		if(kind == CI_UI_WIDGET_SLIDER_H)
		{
			value = rect->percentInside(x, y).x; 
		}
		else 
		{
			value = 1.0-rect->percentInside(x, y).y; 
		}	
        
        if(value > 1.0)
        {
            value = 1.0;
        }
        else if(value < 0.0)
        {
            value = 0.0;
        }        
        updateValueRef();
		updateLabel(); 
	}

    void updateValueRef()
    {
        (*valueRef) = getScaledValue();  
    }
    
	virtual void updateLabel()
	{
		if(kind == CI_UI_WIDGET_SLIDER_H)
		{
            label->setLabel(name + ": " + numToString(*valueRef, labelPrecision));
		}		
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
				label->unfocus(); 
            }
                break;
            case CI_UI_STATE_DOWN:
            {
                draw_fill_highlight = true;            
                draw_outline_highlight = true;             
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
	
	void setValue(float _value)
	{
		value = ci::lmap<float>(_value, min, max, 0.0, 1.0);
        if(value > 1.0f)
        {
            value = 1.0f; 
        }
        else if(value < 0.0f) 
        {
            value = 0.0f; 
        }
        updateValueRef();        
		updateLabel(); 		
	}
		
	float getValue()
	{
		return value; 
	}
	
	float getPercentValue()
	{
		return value; 
	}
	
	float getScaledValue()
	{
		return ci::lmap<float>(value, 0.0, 1.0, min, max);
	}
    
	ciUILabel *getLabel()
	{
		return label; 
	}
    
    void setLabelVisible(bool _labelVisible)
    {
        label->setVisible(_labelVisible);
    }
    
    void setVisible(bool _visible)
    {
        visible = _visible; 
        label->setVisible(visible); 
    }
	
	virtual void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
        rect->setParent(parent->getRect());
		paddedRect->setHeight(paddedRect->getHeight() + label->getPaddingRect()->getHeight());
        if(label->getPaddingRect()->getWidth() > paddedRect->getWidth())
        {
            paddedRect->setWidth(label->getPaddingRect()->getWidth());				
        }
        updateValueRef();        
        updateLabel(); 
	}
	
    void setLabelPrecision(int _precision) 
    {
        labelPrecision = _precision;
        updateValueRef();
        updateLabel();
    }
    
    void setMax(float _max)
    {
        setMaxAndMin(_max, min); 
    }

    float getMax()
    {
        return max; 
    }
    
    void setMin(float _min)
    {
        setMaxAndMin(max, _min); 
    }
    
    float getMin()
    {
        return min; 
    }
    
    Vec2f getMaxAndMind()
    {
        return Vec2f(max, min); 
    }
    
    void setMaxAndMin(float _max, float _min)
    {
        max = _max; 
        min = _min; 
		
		value = ci::lmap<float>(value, 0, 1.0, min, max);
		value = ci::lmap<float>(value, min, max, 0.0, 1.0);
        updateValueRef();        
        updateLabel(); 
    }

    bool isDraggable()
    {
        return true; 
    }
    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
	float value, increment; 
    float *valueRef; 
    bool useReference; 
	float max, min;  
    int labelPrecision;
}; 

#endif
