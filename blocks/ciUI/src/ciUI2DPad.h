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

#ifndef CIUI_2D_PAD
#define CIUI_2D_PAD

#include "ciUIWidgetWithLabel.h"

class ciUI2DPad : public ciUIWidgetWithLabel
{
public:
    ciUI2DPad(float x, float y, float w, float h, Vec2f _value, string _name)
    {
        useReference = false;                 
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, Vec2f(0,w), Vec2f(0,h), &_value, _name);
    }	

    ciUI2DPad(float x, float y, float w, float h, Vec2f _rangeX, Vec2f _rangeY, Vec2f _value, string _name)
    {
        useReference = false;                 
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _rangeX, _rangeY, &_value, _name);
    }	
    
    ciUI2DPad(float w, float h, Vec2f _value, string _name)
    {
        useReference = false;                 
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, Vec2f(0,w), Vec2f(0,h), &_value, _name);
    }
	
    ciUI2DPad(float w, float h, Vec2f _rangeX, Vec2f _rangeY, Vec2f _value, string _name)
    {
        useReference = false;         
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _rangeX, _rangeY, &_value, _name);
    }    

    ciUI2DPad(float x, float y, float w, float h, Vec2f *_value, string _name)
    {
        useReference = true; 
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, Vec2f(0,w), Vec2f(0,h), _value, _name);
    }	
    
    ciUI2DPad(float x, float y, float w, float h, Vec2f _rangeX, Vec2f _rangeY, Vec2f *_value, string _name)
    {
        useReference = true; 
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _rangeX, _rangeY, _value, _name);
    }	
    
    ciUI2DPad(float w, float h, Vec2f *_value, string _name)
    {
        useReference = true;         
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, Vec2f(0,w), Vec2f(0,h), _value, _name);
    }
	
    ciUI2DPad(float w, float h, Vec2f _rangeX, Vec2f _rangeY, Vec2f *_value, string _name)
    {
        useReference = true;         
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _rangeX, _rangeY, _value, _name);
    }    
    
    ~ciUI2DPad()
    {
        if(!useReference)
        {
            delete valueRef; 
        }        
    }
    
    void init(float w, float h, Vec2f _rangeX, Vec2f _rangeY, Vec2f *_value, string _name)
    {
		name = _name; 				
		kind = CI_UI_WIDGET_2DPAD; 		
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
            valueRef = new Vec2f(); 
            *valueRef = value; 
        }

		labelPrecision = 2; 
        rangeX = _rangeX; 
        rangeY = _rangeY; 
        
		value.x = ci::lmap<float>(value.x, rangeX.x, rangeX.y, 0.0, 1.0);
		value.y = ci::lmap<float>(value.y, rangeY.x, rangeY.y, 0.0, 1.0);
		
		if(value.x > 1)
		{
			value.x = 1; 
		}
		else if(value.x < 0)
		{
			value.x = 0; 
		}
		
		if(value.y > 1)
		{
			value.y = 1; 
		}
		else if(value.y < 0)
		{
			value.y = 0; 
		}
		
		label = new ciUILabel(0,h+padding,(name+" LABEL"), (name + ": " + numToString( getScaledValue().x , labelPrecision ) + ", " + numToString( getScaledValue().y , labelPrecision)), CI_UI_FONT_SMALL); 		
		label->setParent(label); 
		label->setRectParent(rect);         
        label->setEmbedded(true);
        increment = 1.0; 
    }
    
    virtual void update()
    {
        if(useReference)
        {
            value.x = ci::lmap<float>(valueRef->x, rangeX.x, rangeX.y, 0.0, 1.0);
            value.y = ci::lmap<float>(valueRef->y, rangeY.x, rangeY.y, 0.0, 1.0);
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
            float x = rect->getX()+value.x*rect->getWidth();
            float y = rect->getY()+value.y*rect->getHeight();             
            ci::gl::drawSolidRect(Rectf(x-CI_UI_GLOBAL_PADDING,y-CI_UI_GLOBAL_PADDING,x+CI_UI_GLOBAL_PADDING,y+CI_UI_GLOBAL_PADDING));
            ci::gl::drawLine(Vec2f(rect->getX()+value.x*rect->getWidth(),  rect->getY()), Vec2f(rect->getX()+value.x*rect->getWidth(),  rect->getY()+rect->getHeight()));
            ci::gl::drawLine(Vec2f(rect->getX(), rect->getY()+value.y*rect->getHeight()), Vec2f(rect->getX()+rect->getWidth(), rect->getY()+value.y*rect->getHeight())); 			
        }
    }
    
    virtual void drawFillHighlight()
    {
        if(draw_fill_highlight)
        {
             
            ci::gl::color(color_fill_highlight); 
            float x = rect->getX()+value.x*rect->getWidth();
            float y = rect->getY()+value.y*rect->getHeight();                         
            ci::gl::drawSolidRect(Rectf(x-CI_UI_GLOBAL_PADDING,y-CI_UI_GLOBAL_PADDING,x+CI_UI_GLOBAL_PADDING,y+CI_UI_GLOBAL_PADDING));
            ci::gl::drawLine(Vec2f(rect->getX()+value.x*rect->getWidth(),  rect->getY()), Vec2f(rect->getX()+value.x*rect->getWidth(),  rect->getY()+rect->getHeight()));
            ci::gl::drawLine(Vec2f(rect->getX(), rect->getY()+value.y*rect->getHeight()), Vec2f(rect->getX()+rect->getWidth(), rect->getY()+value.y*rect->getHeight())); 			                        
            
			label->drawString(rect->getX()+value.x*rect->getWidth()+CI_UI_GLOBAL_WIDGET_SPACING, rect->getY()+value.y*rect->getHeight()+CI_UI_GLOBAL_WIDGET_SPACING, "(" + numToString(getScaledValue().x, labelPrecision) + ", " + numToString(getScaledValue().y, labelPrecision)+")"); 
        }        
    }   

    float getIncrement()
    {
        return increment; 
    }
    
    void setIncrement(float _increment)
	{
		increment = _increment; 
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
                {
                    Vec2f p = getScaledValue();         
                    p.x+=increment; 
                    value.x = ci::lmap<float>(p.x, rangeX.x, rangeX.y, 0.0, 1.0);                    
                    updateValueRef();                                        
                    updateLabel();                     
                    triggerEvent(this);				                    
                }
					break;
					
				case ci::app::KeyEvent::KEY_DOWN:
                {
                    Vec2f p = getScaledValue();         
                    p.y +=increment; 
                    value.y = ci::lmap<float>(p.y, rangeY.x, rangeY.y, 0.0, 1.0);                    
                    updateValueRef();          							        
                    updateLabel();                     
                    triggerEvent(this);			
                }
					break;
					
				case ci::app::KeyEvent::KEY_LEFT:
                {
                    Vec2f p = getScaledValue();         
                    p.x-=increment; 
                    value.x = ci::lmap<float>(p.x, rangeX.x, rangeX.y, 0.0, 1.0);                    
                    updateValueRef();          							        
                    updateLabel();                     
                    triggerEvent(this);			
                }
					break;
					
				case ci::app::KeyEvent::KEY_UP:
                {
                    Vec2f p = getScaledValue();         
                    p.y -=increment; 
                    value.y = ci::lmap<float>(p.y, rangeY.x, rangeY.y, 0.0, 1.0);                    
                    updateValueRef();          							        
                    updateLabel();                     
                    triggerEvent(this);			
                }
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
    
	void input(float x, float y)
	{
		value.x = rect->percentInside(x, y).x; 			
		value.y = rect->percentInside(x, y).y; 	
        if(value.x > 1.0)
        {
            value.x = 1.0;             
        }
        else if(value.x < 0.0)
        {
            value.x = 0.0;             
        }
        
        if(value.y > 1.0)
        {
            value.y = 1.0;             
        }
        else if(value.y < 0.0)
        {
            value.y = 0.0; 
        }
//		cout << "X: " << rect->percentInside(x, y).x << endl;		
//		cout << "Y: " << rect->percentInside(x, y).y << endl;
        updateValueRef();          							                
		updateLabel(); 
	}
	
    void updateValueRef()
    {
        (*valueRef) = getScaledValue();
    }
    
	void updateLabel()
	{        
		label->setLabel(name + ": " + numToString( getScaledValue().x , labelPrecision ) + ", " + numToString( getScaledValue().y , labelPrecision)); 		
	}
	
    void stateChange()
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
	
    void setVisible(bool _visible)
    {
        visible = _visible; 
        label->setVisible(visible); 
    }
    
	void setValue(Vec2f _value)
	{
		if(_value.x > rangeX.y)
		{
			_value.x = rangeX.y;
		}
		else if(_value.x < rangeX.x)
		{
			_value.x = rangeX.x; 
		}
		
		if(_value.y > rangeY.y)
		{
			_value.y = rangeY.y; 
		}
		else if(_value.y < rangeY.x)
		{
			_value.y = rangeY.x; 
		}
		                        
        value.x = ci::lmap<float>(_value.x, rangeX.x, rangeX.y, 0.0, 1.0);
		value.y = ci::lmap<float>(_value.y, rangeY.x, rangeY.y, 0.0, 1.0);
        updateValueRef();        
		updateLabel(); 		
	}
	
	Vec2f getValue()
	{
		return value; 
	}
	
	Vec2f getPercentValue()
	{
		return value; 
	}
	
	Vec2f getScaledValue()
	{
		Vec2f p = value; 
		p.x = ci::lmap<float>(p.x, 0, 1, rangeX.x, rangeX.y); 
		p.y = ci::lmap<float>(p.y, 0, 1, rangeY.x, rangeY.y); 
		return p; 
	}
    
	ciUILabel *getLabel()
	{
		return label; 
	}
	
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
		paddedRect->setHeight(paddedRect->getHeight() + label->getPaddingRect()->getHeight()); 
	}	
    
    bool isDraggable()
    {
        return true; 
    }

    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
	Vec2f value; 
	Vec2f *valueRef;     
    float increment;
    int labelPrecision; 
    bool useReference; 
    Vec2f rangeX, rangeY; 
}; 

#endif
