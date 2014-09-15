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

#ifndef CIUI_ROTARY_SLIDER
#define CIUI_ROTARY_SLIDER

#include "cinder/TriMesh.h"
#include "cinder/Shape2d.h"
#include "cinder/CinderMath.h"
#include "ciUIWidgetWithLabel.h"

class ciUIRotarySlider : public ciUIWidgetWithLabel
{
public:    
    ciUIRotarySlider(float x, float y, float w, float _min, float _max, float _value, string _name)
    {
        useReference = false;                                 
		rect = new ciUIRectangle(x,y,w,w); 
        init(w, _min, _max, &_value, _name); 
    }
    
    ciUIRotarySlider(float w, float _min, float _max, float _value, string _name)
    {
        useReference = false;                         
		rect = new ciUIRectangle(0,0,w,w); 
        init(w, _min, _max, &_value, _name); 
    }
    
    ciUIRotarySlider(float x, float y, float w, float _min, float _max, float *_value, string _name)
    {
        useReference = true;                         
		rect = new ciUIRectangle(x,y,w,w); 
        init(w, _min, _max, _value, _name); 
    }
    
    ciUIRotarySlider(float w, float _min, float _max, float *_value, string _name)
    {
        useReference = true;                         
		rect = new ciUIRectangle(0,0,w,w); 
        init(w, _min, _max, _value, _name); 
    }    
    
    ~ciUIRotarySlider()
    {
        if(!useReference)
        {
            delete valueRef; 
        }        
    }
    
    void init(float w, float _min, float _max, float *_value, string _name)
    {
		name = _name; 				
        kind = CI_UI_WIDGET_ROTARYSLIDER;  			
		
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, w+padding);
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
        
		if(value > max)
		{
			value = max; 
		}
		if(value < min)
		{
			value = min; 
		}
        
        outerRadius = rect->getWidth()*.5f; 
        innerRadius = rect->getWidth()*.25f;         
		
		value = ci::lmap<float>(value, min, max, 0.0, 1.0);
        
        label = new ciUILabel(0,w+padding,(name+" LABEL"), (name + ": " + numToString(getScaledValue(),2)), CI_UI_FONT_SMALL); 		
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
            ci::gl::color(color_back); 
            drawArcStrip(1.0); 
        }
    }

    virtual void drawFill()    
    {
        if(draw_fill)
        {			
            ci::gl::color(color_fill); 
            drawArcStrip(value); 
        }
    }

    virtual void drawFillHighlight()
    {
        if(draw_fill_highlight)
        {
            ci::gl::color(color_fill_highlight); 
            drawArcStrip(value); 
        }        
    }   
    
    virtual void drawOutline()
    {
        if(draw_outline)
        {
            ci::gl::color(color_outline); 
            ci::gl::drawStrokedCircle(Vec2f(rect->getX()+rect->getHalfWidth(), rect->getY()+rect->getHalfHeight()), innerRadius);
            ci::gl::drawStrokedCircle(Vec2f(rect->getX()+rect->getHalfWidth(), rect->getY()+rect->getHalfHeight()), outerRadius);                                      
        }
    }
    virtual void drawOutlineHighlight()
    {
        if(draw_outline_highlight)
        {
            ci::gl::color(color_outline_highlight); 
            ci::gl::drawStrokedCircle(Vec2f(rect->getX()+rect->getHalfWidth(), rect->getY()+rect->getHalfHeight()), innerRadius);
            ci::gl::drawStrokedCircle(Vec2f(rect->getX()+rect->getHalfWidth(), rect->getY()+rect->getHalfHeight()), outerRadius);                                      
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
    
    void drawArcStrip(float percent)
    {        
        float theta = ci::lmap<float>(percent, 0.0f , 1.0f , 0.0f , 360.0f);

        ci::gl::pushMatrices();
        ci::gl::translate(rect->getX(),rect->getY());         

        TriMesh2d mesh;                         

        {
            float x = sin(-ci::toRadians(0.0f)); 
            float y = cos(-ci::toRadians(0.0f));         
            mesh.appendVertex(Vec2f(center.x+outerRadius*x,center.y+outerRadius*y)); 
        }
        
        for(int i = 0; i < theta; i+=8)
        {
            float x = sin(-ci::toRadians((float)i)); 
            float y = cos(-ci::toRadians((float)i));     
            mesh.appendVertex(Vec2f(center.x+outerRadius*x,center.y+outerRadius*y)); 
        }
        
        {
            float x = sin(-ci::toRadians(theta)); 
            float y = cos(-ci::toRadians(theta));         
            mesh.appendVertex(Vec2f(center.x+outerRadius*x,center.y+outerRadius*y)); 
            //            mesh.appendVertex(Vec2f(center.x+innerRadius*x,center.y+innerRadius*y)); 
        }
        
        {
            float x = sin(-ci::toRadians(0.0f)); 
            float y = cos(-ci::toRadians(0.0f));         
            mesh.appendVertex(Vec2f(center.x+innerRadius*x,center.y+innerRadius*y)); 
        }
        
        for(int i = 0; i < theta; i+=8)
        {
            float x = sin(-ci::toRadians((float)i)); 
            float y = cos(-ci::toRadians((float)i));             
            mesh.appendVertex(Vec2f(center.x+innerRadius*x,center.y+innerRadius*y)); 
        }
        
        {
            float x = sin(-ci::toRadians(theta)); 
            float y = cos(-ci::toRadians(theta));                  
            mesh.appendVertex(Vec2f(center.x+innerRadius*x,center.y+innerRadius*y)); 
        }
        
        int numVerts = (mesh.getNumVertices())/2; 
        for(int i = 0; i < numVerts-1; i++)
        {        
            int topIndex = i+numVerts; 
            mesh.appendTriangle(i, i+1, topIndex);
            mesh.appendTriangle(i+1, topIndex + 1, topIndex);
        }     
        
        ci::gl::draw(mesh);
        
        ci::gl::popMatrices();
    }
    
    void setIncrement(float _increment)
	{
		increment = _increment; 
	}
    
	void input(float x, float y)
	{
        hitPoint = Vec2f(x,y);         
        Vec2f mappedHitPoint = hitPoint; 
        mappedHitPoint -= Vec2f(rect->getX()+center.x, rect->getY()+center.y);         
        mappedHitPoint.normalized();
        Vec2f cVector = center-homePoint;        
        cVector.normalized();
            
        value = ci::lmap<float>(atan2(cVector.x*mappedHitPoint.y-cVector.y*mappedHitPoint.x, cVector.x*mappedHitPoint.x + cVector.y*mappedHitPoint.y ), -M_PI, M_PI, 0.0f, 1.0f);
        
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
        
	void updateLabel()
	{
        label->setLabel(name + ": " + numToString(getScaledValue(),2)); 		
	}
	
    void stateChange()
    {
        if(value > 0) 
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
    
    
	void setValue(float _value)
	{
		value = ci::lmap<float>(_value, min, max, 0.0, 1.0);
        updateValueRef();        
		updateLabel(); 		
	}
    
	float getValue()
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
	
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
		paddedRect->setHeight( paddedRect->getHeight() + label->getPaddingRect()->getHeight()); 
        if(label->getPaddingRect()->getWidth()+padding*2.0f > paddedRect->getWidth())
        {
            paddedRect->setWidth(label->getPaddingRect()->getWidth());         
        }
        center = Vec2f(rect->getWidth()*.5f, rect->getHeight()*.5f);   
        homePoint = Vec2f(rect->getWidth()*.5f, rect->getHeight());   
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
    Vec2f center; 
    Vec2f hitPoint; 
    Vec2f homePoint; 
    float outerRadius, innerRadius; 

}; 

#endif
