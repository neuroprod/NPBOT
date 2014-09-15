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

#ifndef CIUI_IMAGE_SAMPLER
#define CIUI_IMAGE_SAMPLER

#include "ciUIImage.h"

class ciUIImageSampler : public ciUIImage
{
public:
    ciUIImageSampler(float x, float y, float w, float h, Surface *_image, string _name) : ciUIImage(x, y, w, h, _image, _name)
    {
        initSampler();
    }
    
    ciUIImageSampler(float w, float h, Surface *_image, string _name) : ciUIImage(w, h, _image, _name)
    {
        initSampler();
    }    
    
    void initSampler()
    {
        label->setVisible(false);          
        value.x = .5f; 
		value.y = .5f; 
        kind = CI_UI_WIDGET_IMAGESAMPLER; 
        squareSize = CI_UI_GLOBAL_WIDGET_SPACING;
        squareSizeHalf = .5f*squareSize; 
        input(value.x*rect->getWidth(),value.y*rect->getHeight());
        increment = 1.0; 
    }
    
    void setSquareSize(float _squareSize)
    {
        squareSize = _squareSize; 
    }
    
    void drawFill()
    {
        if(draw_fill)
        {			            
			if(imageSurface != NULL)
			{			   
                ci::gl::color(1.0f,1.0f,1.0f,1.0f);
                gl::draw(*imageSurface, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));
			}
                               
            float x = rect->getX()+value.x*rect->getWidth();
            float y = rect->getY()+value.y*rect->getHeight();             
            
            ci::gl::color(color_fill); 		            
            ci::gl::drawLine(Vec2f(rect->getX()+value.x*rect->getWidth(),  rect->getY()), Vec2f(rect->getX()+value.x*rect->getWidth(),  rect->getY()+rect->getHeight()));
            ci::gl::drawLine(Vec2f(rect->getX(), rect->getY()+value.y*rect->getHeight()), Vec2f(rect->getX()+rect->getWidth(), rect->getY()+value.y*rect->getHeight())); 			

            ci::gl::color(sampledColor); 		            
            ci::gl::drawSolidRect(Rectf(x-squareSizeHalf,y-squareSizeHalf,x+squareSizeHalf,y+squareSizeHalf));                    
            ci::gl::drawSolidRect(Rectf(rect->getX(),rect->getY(),rect->getX()+squareSize,rect->getY()+squareSize));                     
        }
    }
    
    void drawFillHighlight()
    {
        if(draw_fill_highlight)
        {
            float x = rect->getX()+value.x*rect->getWidth();
            float y = rect->getY()+value.y*rect->getHeight();             
            
            ci::gl::color(color_fill_highlight); 		            
            ci::gl::drawLine(Vec2f(rect->getX()+value.x*rect->getWidth(),  rect->getY()), Vec2f(rect->getX()+value.x*rect->getWidth(),  rect->getY()+rect->getHeight()));
            ci::gl::drawLine(Vec2f(rect->getX(), rect->getY()+value.y*rect->getHeight()), Vec2f(rect->getX()+rect->getWidth(), rect->getY()+value.y*rect->getHeight())); 			
            
            ci::gl::color(sampledColor); 		            
            ci::gl::drawSolidRect(Rectf(x-squareSize,y-squareSize,x+squareSize,y+squareSize));            
            ci::gl::drawSolidRect(Rectf(rect->getX(),rect->getY(),rect->getX()+squareSize,rect->getY()+squareSize));                    
        }        
    }
        
    void setVisible(bool _visible)
    {
        visible = _visible; 
        label->setVisible(false); 
    }    
    	
    void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
        paddedRect->setHeight(paddedRect->getHeight() + padding);
	}	        
    
    void keyDown( KeyEvent &event )
    {		
		if(state == CI_UI_STATE_OVER)
		{
			switch (event.getCode()) 
			{
				case ci::app::KeyEvent::KEY_RIGHT:
                {
                    input(rect->getX()+value.x*rect->getWidth()+increment, rect->getY()+value.y*rect->getHeight());
                    triggerEvent(this);										                        
                }
					break;
					
				case ci::app::KeyEvent::KEY_DOWN:
                {
                    input(rect->getX()+value.x*rect->getWidth(), rect->getY()+value.y*rect->getHeight()+increment);
                    triggerEvent(this);										                        
                }
					break;
					
				case ci::app::KeyEvent::KEY_LEFT:
                {
                    input(rect->getX()+value.x*rect->getWidth()-increment, rect->getY()+value.y*rect->getHeight());
                    triggerEvent(this);										                         
                }
					break;
					
				case ci::app::KeyEvent::KEY_UP:
                {
                    input(rect->getX()+value.x*rect->getWidth(), rect->getY()+value.y*rect->getHeight()-increment);
                    triggerEvent(this);										                           
                }
					break;					
					
				default:
					break;
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
			input(x, y); 
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
			input(x, y); 
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
			input(x, y); 
			triggerEvent(this); 			
        }    
        else
        {
            state = CI_UI_STATE_NORMAL;         
        }
        stateChange();         
        hit = false; 
    }

    void stateChange()
    {
        switch (state) {
            case CI_UI_STATE_NORMAL:
            {            
                draw_fill_highlight = false;             
                draw_outline_highlight = false;             			
            }
                break;
            case CI_UI_STATE_OVER:
            {
                draw_fill_highlight = false;            
                draw_outline_highlight = true;    
            }
                break;
            case CI_UI_STATE_DOWN:
            {
                draw_fill_highlight = true;            
                draw_outline_highlight = true;             
            }
                break;
            case CI_UI_STATE_SUSTAINED:
            {
                draw_fill_highlight = false;            
                draw_outline_highlight = false;
            }
                break;            
                
            default:
                break;
        }        
    }
    
    void input(int x, int y)
    {
		value.x = rect->percentInside((float) x, (float) y).x; 			
		value.y = rect->percentInside((float) x, (float) y).y; 	
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
        if(imageSurface != NULL)
        {
            sampledColor = imageSurface->getPixel(Vec2i(value.x*(imageSurface->getWidth()-1), value.y*(imageSurface->getHeight()-1)));          
        }
    }
    
    ColorA& getColor()
    {
        return sampledColor; 
    }
    
    void setColor(ColorA _sampledColor)
    {
        sampledColor = _sampledColor; 
    }
    
    Vec2f getValue()
    {
        return value;
    }
    
    void setValue(Vec2f _value)
    {
        value = _value; 
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
        if(imageSurface != NULL)
        {
            sampledColor = imageSurface->getPixel(Vec2i(value.x*(imageSurface->getWidth()-1), value.y*(imageSurface->getHeight()-1)));          
        }
    }
    
    bool isDraggable()
    {
        return true; 
    }

protected: 
    ColorA sampledColor; 
	Vec2f value;     
    float increment;    
    float squareSize, squareSizeHalf;    
}; 

#endif
