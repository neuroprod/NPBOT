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

#ifndef CIUI_IMAGE
#define CIUI_IMAGE

#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "ciUIWidgetWithLabel.h"

class ciUIImage : public ciUIWidgetWithLabel
{
public:
    ciUIImage(float x, float y, float w, float h, gl::Texture *_image, string _name)
    {
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _image, _name); 
    }
    
    ciUIImage(float x, float y, float w, float h, gl::Texture *_image, string _name, bool _showLabel)
    {
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _image, _name); 
        showLabel = _showLabel; 
    }
    
    ciUIImage(float w, float h, gl::Texture *_image, string _name)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _image, _name); 
    }    

    ciUIImage(float w, float h, gl::Texture *_image, string _name, bool _showLabel)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _image, _name); 
        showLabel = _showLabel; 
    }    
    
//
    
    ciUIImage(float x, float y, float w, float h, Surface *_image, string _name)
    {
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _image, _name); 
    }
    
    ciUIImage(float x, float y, float w, float h, Surface *_image, string _name, bool _showLabel)
    {
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _image, _name); 
        showLabel = _showLabel; 
    }
    
    ciUIImage(float w, float h, Surface *_image, string _name)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _image, _name); 
    }    
    
    ciUIImage(float w, float h, Surface *_image, string _name, bool _showLabel)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _image, _name); 
        showLabel = _showLabel; 
    }        
    
    void init(float w, float h, gl::Texture *_image, string _name)
    {
		name = _name; 				
		kind = CI_UI_WIDGET_IMAGE; 
        showLabel = true; 
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding);
		paddedRect->setParent(rect); 
        
        draw_back = false; 
        draw_fill = true; 
        
		image = _image; 
        imageSurface = NULL; 
        useSurface = false; 
        
		label = new ciUILabel(0,h+padding,(name+" LABEL"),name, CI_UI_FONT_SMALL); 		
		label->setParent(label); 
		label->setRectParent(rect);    
        label->setEmbedded(true);        
    }

    void init(float w, float h, Surface *_image, string _name)
    {
		name = _name; 				
		kind = CI_UI_WIDGET_IMAGE; 
        showLabel = true; 
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding);
		paddedRect->setParent(rect); 
        
        draw_back = false; 
        draw_fill = true; 
        
		image = NULL; 
        imageSurface = _image; 
        useSurface = true; 
        
		label = new ciUILabel(0,h+padding,(name+" LABEL"),name, CI_UI_FONT_SMALL); 		
		label->setParent(label); 
		label->setRectParent(rect);    
        label->setEmbedded(true);        
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
            if(useSurface)
            {
                if(image != NULL)
                {			   
                    ci::gl::color(1.0f,1.0f,1.0f,1.0f);
                    gl::draw(*imageSurface, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));
                }                
            }
            else
            {
                if(image != NULL)
                {			   
                    ci::gl::color(1.0f,1.0f,1.0f,1.0f);
                    gl::draw(*image, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));
                }                
            }
        }
    }        

    virtual void setVisible(bool _visible)
    {
        visible = _visible; 
        label->setVisible(showLabel);             
    }    
				
	ciUILabel *getLabel()
	{
		return label; 
	}
    
    void setImage(gl::Texture *_image)
    {
        image = _image; 
        useSurface = false; 
    }
    
    void setImage(Surface *_image)
    {        
        imageSurface = _image; 
        useSurface = true; 
    }
	
	virtual void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
        if(showLabel)
        {
            paddedRect->setHeight(paddedRect->getHeight() + label->getPaddingRect()->getHeight()); 		
        }    
        else
        {
            paddedRect->setHeight(paddedRect->getHeight() + padding);
        }
        label->setVisible(showLabel);             
	}	

    virtual bool isDraggable()
    {
        return false; 
    }
    
    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
    gl::Texture *image;   
    Surface *imageSurface;
    bool showLabel; 
    bool useSurface; 
}; 

#endif
