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

#ifndef CIUI_RANGE_SLIDER
#define CIUI_RANGE_SLIDER

#include "cinder/Utilities.h"
#include "cinder/CinderMath.h"
#include "ciUIWidgetWithLabel.h"

class ciUIRangeSlider : public ciUIWidgetWithLabel
{
public:
    ciUIRangeSlider(float x, float y, float w, float h, float _min, float _max, float _valuelow, float _valuehigh, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = false;                                 
        rect = new ciUIRectangle(x,y,w,h); 	        
        init(w, h, _min, _max, &_valuelow, &_valuehigh, _name, _size); 
    }
    
    ciUIRangeSlider(float w, float h, float _min, float _max, float _valuelow, float _valuehigh, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = false;                                 
        rect = new ciUIRectangle(0,0,w,h); 	        
        init(w, h, _min, _max, &_valuelow, &_valuehigh, _name, _size); 
    }

    ciUIRangeSlider(float x, float y, float w, float h, float _min, float _max, float *_valuelow, float *_valuehigh, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = true;                         
        rect = new ciUIRectangle(x,y,w,h); 	        
        init(w, h, _min, _max, _valuelow, _valuehigh, _name, _size); 
    }
    
    ciUIRangeSlider(float w, float h, float _min, float _max, float *_valuelow, float *_valuehigh, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = true;                                 
        rect = new ciUIRectangle(0,0,w,h); 	        
        init(w, h, _min, _max, _valuelow, _valuehigh, _name, _size); 
    }
    
    ~ciUIRangeSlider()    
    {
        if(!useReference)
        {
            delete valuelowRef; 
            delete valuehighRef;             
        }        
    }
    
    void init(float w, float h, float _min, float _max, float *_valuelow, float *_valuehigh, string _name, int _size)
    {
        name = _name; 				
		if(w > h)
		{
			kind = CI_UI_WIDGET_RSLIDER_H;  			
		}
		else 
		{
			kind = CI_UI_WIDGET_RSLIDER_V;  			
		}
		
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding);
		paddedRect->setParent(rect); 
		
        draw_fill = true; 
        
        valuelow = *_valuelow;                                                  //the widget's value
        valuehigh = *_valuehigh;                                                //the widget's value		
        
        if(useReference)
        {
            valuelowRef = _valuelow;                                               
            valuehighRef = _valuehigh;                                             
        }
        else
        {
            valuelowRef = new float(); 
            valuehighRef = new float();             
            *valuelowRef = valuelow;                                               
            *valuehighRef = valuehigh;                                             
        }
		
		max = _max; 
		min = _min; 
		hitHigh = false; 
		hitLow = false;
		hitCenter = false; 
		
        labelPrecision = 2;
        
		if(valuelow > max)
		{
			valuelow = max; 
		}
		if(valuelow < min)
		{
			valuelow = min; 
		}
		
		if(valuehigh > max)
		{
			valuehigh = max; 
		}
		if(valuehigh < min)
		{
			valuehigh = min; 
		}

		valuelow = ci::lmap<float>(valuelow, min, max, 0.0, 1.0); 
		valuehigh = ci::lmap<float>(valuehigh, min, max, 0.0, 1.0); 
		
		if(kind == CI_UI_WIDGET_RSLIDER_H)
		{            
			label = new ciUILabel(0,h+padding,(name+" LABEL"), (name + ": " + numToString(getScaledValueLow(), labelPrecision) + " " + numToString(getScaledValueHigh(), labelPrecision)), _size); 
		}
		else 
		{
			label = new ciUILabel(0,h+padding,(name+" LABEL"), name, _size); 	
		}
		
		label->setParent(label); 
		label->setRectParent(rect); 		
        label->setEmbedded(true);
        
        increment = 0.1f; 	
    }
    
    virtual void update()
    {
        if(useReference)
        {
            valuelow = cinder::lmap<float>(*valuelowRef, min, max, 0.0, 1.0); 
            valuehigh = cinder::lmap<float>(*valuehighRef, min, max, 0.0, 1.0);         
        }
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
    
    virtual void drawFill()
    {
        if(draw_fill)
        {			
            ci::gl::color(color_fill); 
			if(kind == CI_UI_WIDGET_RSLIDER_H)
			{			   
                cinder::gl::drawSolidRect(Rectf(rect->getX()+rect->getWidth()*valuelow, rect->getY(), rect->getX()+rect->getWidth()*valuehigh, rect->getY()+rect->getHeight()));
			}
			else 
			{
				cinder::gl::drawSolidRect(Rectf(rect->getX(), rect->getY()+(1.0f-valuehigh)*rect->getHeight(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()*(1.0f-valuelow))); 
			}
        }
    }
    
    virtual void drawFillHighlight()
    {
        if(draw_fill_highlight)
        {
            ci::gl::color(color_fill_highlight); 
			if(kind == CI_UI_WIDGET_RSLIDER_H)
			{			   
                cinder::gl::drawSolidRect(Rectf(rect->getX()+rect->getWidth()*valuelow, rect->getY(), rect->getX()+rect->getWidth()*valuehigh, rect->getY()+rect->getHeight()));
			}
			else 
			{
				cinder::gl::drawSolidRect(Rectf(rect->getX(), rect->getY()+(1.0f-valuehigh)*rect->getHeight(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()*(1.0f-valuelow))); 
			}

			if(kind == CI_UI_WIDGET_RSLIDER_V)
			{                
				label->drawString(rect->getX()+rect->getWidth()+padding, rect->getY()+rect->getHeight()-rect->getHeight()*valuehigh, numToString(getScaledValueHigh(), labelPrecision)); 
				label->drawString(rect->getX()+rect->getWidth()+padding, rect->getY()+rect->getHeight()-rect->getHeight()*valuelow, numToString(getScaledValueLow(), labelPrecision)); 
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
            hitValueHigh = valuehigh; 
            hitValueLow = valuelow; 
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
            hitValueHigh = valuehigh; 
            hitValueLow = valuelow; 
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
        hitHigh = false;  
        hitLow = false; 
        hitCenter = false; 
    }
	
    void keyDown( KeyEvent &event )
    {		
		if(state == CI_UI_STATE_OVER)
		{
			switch (event.getCode()) 
			{
				case ci::app::KeyEvent::KEY_RIGHT:
					setValueHigh(getScaledValueHigh()+increment); 					
					setValueLow(getScaledValueLow()+increment); 															
					triggerEvent(this); 
					break;
                    
				case ci::app::KeyEvent::KEY_LEFT:
					setValueHigh(getScaledValueHigh()-increment); 					
					setValueLow(getScaledValueLow()-increment); 										
					triggerEvent(this); 					
					break;
					
					
				case ci::app::KeyEvent::KEY_UP:
					setValueHigh(getScaledValueHigh()+increment); 
					setValueLow(getScaledValueLow()-increment); 					
					triggerEvent(this); 
					break;
					
					
				case ci::app::KeyEvent::KEY_DOWN:
					setValueHigh(getScaledValueHigh()-increment); 
					setValueLow(getScaledValueLow()+increment); 					
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
    
    void setIncrement(float _increment)
	{
		increment = _increment; 
	}

	void input(float x, float y)
	{
        float v = 0; 
        
		if(kind == CI_UI_WIDGET_RSLIDER_H)
		{	
            v = rect->percentInside(x, y).x; 						            			
		}
		else 
		{			
            v = 1.0-rect->percentInside(x, y).y; 						  				
		}
        
        if(hitHigh)
        {								
            valuehigh = v;	
        }			
        else if(hitLow)
        {
            valuelow = v; 
        }
        else if(hitCenter)
        {
            valuehigh +=(v-hitPoint); 
            valuelow +=(v-hitPoint);
            hitPoint = v; 
        }
        else 
        {
            float dvh = fabs(valuehigh - v); 
            float dvl = fabs(valuelow - v); 
            if(dvh < .05 || v > valuehigh)
            {
                valuehigh = v;
                hitHigh = true; 					
            }
            else if(dvl < .05 || v < valuelow)
            {
                valuelow = v; 
                hitLow = true; 
            }	
            else 
            {
                hitCenter = true; 
                hitPoint = v; 
            }
        }			
        
		if(valuehigh < valuelow && hitHigh)			
		{
            valuehigh = hitValueLow; 
			hitHigh = false; 
			hitLow = true; 
		}				
		else if(valuelow > valuehigh && hitLow)			
		{
            valuelow = hitValueHigh; 
			hitHigh = true; 
			hitLow = false; 
		}
        
        if(valuehigh > 1.0)
        {
            valuehigh = 1.0; 
        }
        else if(valuehigh < 0.0)
        {
            valuehigh = 0.0; 
        }
        
        if(valuelow < 0.0)
        {
            valuelow = 0.0; 
        }
        else if(valuelow > 1.0)
        {
            valuelow = 1.0; 
        }
        updateValueRef();
		updateLabel(); 
	}
    
    void updateValueRef()
    {
        (*valuelowRef) = getScaledValueLow();  
        (*valuehighRef) = getScaledValueHigh();          
    }
    
	
	void updateLabel()
	{
		if(kind == CI_UI_WIDGET_RSLIDER_H)
		{            
			label->setLabel(name + ": " + numToString(getScaledValueLow(), labelPrecision) + " " + numToString(getScaledValueHigh(), labelPrecision)); 		
		}		
	}
	
    void stateChange()
    {
        if(valuehigh > 0)
        {
            draw_fill = true; 
        }
        else 
        {
            draw_fill = false; 
        }
        
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
	
    void setVisible(bool _visible)
    {
        visible = _visible; 
        label->setVisible(visible); 
    }
    
	void setValueLow(float _value)
	{			
		valuelow = ci::lmap<float>(_value, min, max, 0.0, 1.0);
        updateValueRef();
		updateLabel(); 		
	}

	void setValueHigh(float _value)
	{
		valuehigh = ci::lmap<float>(_value, min, max, 0.0, 1.0);
        updateValueRef();
		updateLabel(); 		
	}
	
	float getPercentValueLow()
	{
		return valuelow; 
	}

	float getPercentValueHigh()
	{
		return valuehigh; 
	}
	
	float getScaledValueLow()
	{
		return ci::lmap<float>(valuelow, 0.0, 1.0, min, max);
	}

	float getScaledValueHigh()
	{
		return ci::lmap<float>(valuehigh, 0.0, 1.0, min, max);
	}
	
	ciUILabel *getLabel()
	{
		return label; 
	}
    
    void setLabelVisible(bool _labelVisible)
    {
        label->setVisible(_labelVisible);
    }
    
    void setLabelPrecision(int _precision) {
        labelPrecision = _precision;
        updateValueRef();
        updateLabel();
    }
	
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
		paddedRect->setHeight(paddedRect->getHeight() + label->getPaddingRect()->getHeight()); 
		if(kind == CI_UI_WIDGET_RSLIDER_V)
		{
			if(label->getPaddingRect()->getWidth() > paddedRect->getWidth())
			{
				paddedRect->setWidth(label->getPaddingRect()->getWidth()+padding);				
			}
		}
	}	
    
    void setMax(float _max)
    {
        setMaxAndMin(_max, min); 
    }
    
    void setMin(float _min)
    {
        setMaxAndMin(max, _min); 
    }
    
    void setMaxAndMin(float _max, float _min)
    {
        max = _max; 
        min = _min; 
		

		valuelow = ci::lmap<float>(valuelow, 0, 1.0, min, max);
        valuelow = ci::lmap<float>(valuelow, min, max, 0.0, 1.0);

		valuehigh = ci::lmap<float>(valuehigh, 0, 1.0, min, max); 
		valuehigh = ci::lmap<float>(valuehigh, min, max, 0.0, 1.0);
        updateValueRef();
        updateLabel(); 
    }

    bool isDraggable()
    {
        return true; 
    }

    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
	float valuelow, valuehigh, increment; 
    float *valuelowRef; 
    float *valuehighRef;
    bool useReference;     
    float hitValueLow, hitValueHigh; 
	bool hitHigh, hitLow, hitCenter; 
	float hitPoint; 
	float max, min; 
    int labelPrecision;    
}; 

#endif
