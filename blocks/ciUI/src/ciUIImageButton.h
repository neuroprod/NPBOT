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

#ifndef CIUI_IMAGE_BUTTON
#define CIUI_IMAGE_BUTTON

#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "ciUIWidget.h"

class ciUIImageButton : public ciUIButton
{
public:
    ciUIImageButton() {}
    
    ciUIImageButton(float x, float y, float w, float h, bool _value, DataSourceRef _pathURL, string _name)
    {
        useReference = false; 
        rect = new ciUIRectangle(x,y,w,h);
        init(w, h, &_value, _pathURL, _name);         
    }

    ciUIImageButton(float w, float h, bool _value, DataSourceRef _pathURL, string _name)
    {
        useReference = false; 
        rect = new ciUIRectangle(0,0,w,h);
        init(w, h, &_value, _pathURL, _name);         
    }

    ciUIImageButton(float x, float y, float w, float h, bool *_value, DataSourceRef _pathURL, string _name)
    {
        useReference = true;         
        rect = new ciUIRectangle(x,y,w,h);
        init(w, h, _value, _pathURL, _name);         
    }
    
    ciUIImageButton(float w, float h, bool *_value, DataSourceRef _pathURL, string _name)
    {
        useReference = true;         
        rect = new ciUIRectangle(0,0,w,h);
        init(w, h, _value, _pathURL, _name);         
    }

    void init(float w, float h, bool *_value, DataSourceRef _pathURL, string _name)
    {
        name = _name; 		
		kind = CI_UI_WIDGET_IMAGEBUTTON; 		
        
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding*2.0f);
		paddedRect->setParent(rect); 

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

        image = ci::loadImage( _pathURL );
    }
	
    virtual void setDrawPadding(bool _draw_padded_rect)
	{
		draw_padded_rect = _draw_padded_rect; 
	}
    
    virtual void setDrawPaddingOutline(bool _draw_padded_rect_outline)
	{
		draw_padded_rect_outline = _draw_padded_rect_outline; 
	}  
    
    virtual ~ciUIImageButton()
    {
        
    }
	

    virtual void drawBack()
    {
                    
        if(draw_back)
        {
            ci::gl::color(color_back); 
            gl::draw(image, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));            
        }
    }
    
    virtual void drawFill()
    {
        if(draw_fill)
        {
            ci::gl::color(color_fill); 
            gl::draw(image, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));            
        }
    }
    
    virtual void drawFillHighlight()
    {
        if(draw_fill_highlight)
        {
            ci::gl::color(color_fill_highlight); 
            gl::draw(image, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));            
        }
    }   
    
    virtual void drawOutlineHighlight()
    {
        if(draw_outline_highlight)
        {
            ci::gl::color(color_outline_highlight); 
            gl::draw(image, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));            
        }
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
                draw_outline_highlight = false;             
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
	
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
	}	

    virtual void setValue(bool _value)
	{
		*value = _value;         
        draw_fill = *value; 
	}	
    
    virtual void setVisible(bool _visible)
    {
        visible = _visible; 
    }

    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
    gl::Texture image; 
}; 

#endif
