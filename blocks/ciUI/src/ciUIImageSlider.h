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

#ifndef CIUI_IMAGE_SLIDER
#define CIUI_IMAGE_SLIDER

#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Area.h"
#include "ciUISlider.h"
#include "ciUIRectangle.h"

class ciUIImageSlider : public ciUISlider         
{
public:    
    ciUIImageSlider(float x, float y, float w, float h, float _min, float _max, float _value, string _pathURL, string _name)
    {
        useReference = false;                                 
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _min, _max, &_value, _pathURL, _name); 		
    }
    
    ciUIImageSlider(float w, float h, float _min, float _max, float _value, string _pathURL, string _name)
    {
        useReference = false;                          
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _min, _max, &_value, _pathURL, _name); 
    }    

    ciUIImageSlider(float x, float y, float w, float h, float _min, float _max, float *_value, string _pathURL, string _name)
    {
       useReference = true;                          
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _min, _max, _value, _pathURL, _name); 		
    }
    
    ciUIImageSlider(float w, float h, float _min, float _max, float *_value, string _pathURL, string _name)
    {
        useReference = true;                                 
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _min, _max, _value, _pathURL, _name); 
    }    
    
    virtual void init(float w, float h, float _min, float _max, float *_value, string _pathURL, string _name)
    {
        name = _name; 				
		if(w > h)
		{
			kind = CI_UI_WIDGET_IMAGESLIDER_H;  			
		}
		else 
		{
			kind = CI_UI_WIDGET_IMAGESLIDER_V;  			
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
		
		value = ci::lmap<float>(value, min, max, 0.0, 1.0); 
        
		if(kind == CI_UI_WIDGET_IMAGESLIDER_H)
		{
			label = new ciUILabel(0,h+padding,(name+" LABEL"), (name + ": " + numToString(max,labelPrecision)), CI_UI_FONT_SMALL); 
		}
		else 
		{
			label = new ciUILabel(0,h+padding,(name+" LABEL"), name, CI_UI_FONT_SMALL); 	
		}
        
		label->setParent(label); 
		label->setRectParent(rect); 
        label->setEmbedded(true);
        increment = .10f;         
        
        string coreURL = _pathURL;
        string extension = "";
        string period (".");
        size_t found;        
        
        found=_pathURL.find_last_of(period);
        if (found!=string::npos)        
        {
            coreURL = _pathURL.substr(0,found);
            extension = _pathURL.substr(found);
        }
        
        
        track = loadImage(coreURL+"track"+extension);                 //back
        progress = loadImage(coreURL+"progress"+extension);           //fill
        handle = loadImage(coreURL+"handle"+extension);               //handle        
        handleDown = loadImage(coreURL+"handledown"+extension);       //handleOver State                        
        
        handleHalfWidth = handle.getWidth()*.5f;
        handleHalfHeight = handle.getHeight()*.5f;
        
        if(kind == CI_UI_WIDGET_IMAGESLIDER_H)
        {
            imageRect = new ciUIRectangle(handleHalfWidth,0,rect->getWidth()-handle.getWidth(), rect->getHeight());
        }
        else
        {
            imageRect = new ciUIRectangle(0,handleHalfHeight,rect->getWidth(), rect->getHeight()-handle.getHeight());            
        }
        imageRect->setParent(rect);     
    }
    
    ~ciUIImageSlider()
    {
        delete imageRect; 
    }
    
    
    virtual void drawBack() 
    {
        if(draw_back)
        {
            ci::gl::color(ColorA(1.0f,1.0f,1.0f,1.0f));
            gl::draw(track, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));                        
        }
    }
    
    virtual void drawOutline() 
    {
        if(draw_outline)
        {
            ci::gl::color(color_outline); 
            rect->draw(); 
        }
    }
    
    virtual void drawOutlineHighlight() 
    {
        
    }    
    
    virtual void drawFill() 
    {
        if(draw_fill)
        {			 
            ci::gl::color(ColorA(1.0f,1.0f,1.0f,1.0f));
			if(kind == CI_UI_WIDGET_IMAGESLIDER_H)
			{	
                gl::draw(progress, Area(0,0,rect->getWidth()*value,rect->getHeight()), Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth()*value, rect->getY()+rect->getHeight()));
                gl::draw(handle, Vec2f(imageRect->getX()+imageRect->getWidth()*value-handleHalfWidth, imageRect->getY())); 
			}
			else 
			{
                gl::draw(progress, 
                         Area(0,rect->getHeight()*(1.0-value),rect->getWidth(),rect->getHeight()), 
                         Rectf(rect->getX(), rect->getY()+rect->getHeight()*(1.0-value), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));                            
                gl::draw(handle, Vec2f(imageRect->getX(), imageRect->getY()+imageRect->getHeight()*(1.0-value)-handleHalfWidth));            
			}
        }
    }
    
    virtual void drawFillHighlight() 
    {
        ci::gl::color(ColorA(1.0f,1.0f,1.0f,1.0f));
        if(draw_fill_highlight)
        {
            if(kind == CI_UI_WIDGET_IMAGESLIDER_H)
            {	
                gl::draw(progress, Area(0,0,rect->getWidth()*value,rect->getHeight()), Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth()*value, rect->getY()+rect->getHeight()));            
                gl::draw(handleDown, Vec2f(imageRect->getX()+imageRect->getWidth()*value-handleHalfWidth, imageRect->getY())); 
            }
            else
            {
                gl::draw(progress, 
                         Area(0,rect->getHeight()*(1.0f-value),rect->getWidth(),rect->getHeight()), 
                         Rectf(rect->getX(), rect->getY()+rect->getHeight()*(1.0f-value), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));                            
                gl::draw(handleDown, Vec2f(imageRect->getX(), imageRect->getY()+imageRect->getHeight()*(1.0f-value)-handleHalfWidth));            
                label->drawString(imageRect->getX()+imageRect->getWidth()+padding, imageRect->getY()+imageRect->getHeight()*(1.0f-value)-label->getRect()->getHalfHeight()*.5f, numToString(getScaledValue(),labelPrecision)); 
			}
        }
    }
    
    virtual void stateChange()
    {                
        switch (state) {
            case CI_UI_STATE_NORMAL:
            {            
                draw_back = true; 
                draw_fill = true; 
                draw_fill_highlight = false;             
                draw_outline_highlight = false;             
				label->unfocus(); 				
            }
                break;
            case CI_UI_STATE_OVER:
            {
                draw_back = true;       
                draw_fill = true;                
                draw_fill_highlight = false;            
                draw_outline_highlight = false;    
				label->unfocus(); 
            }
                break;
            case CI_UI_STATE_DOWN:
            {
                draw_back = true;   
                draw_fill = false;                
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

    virtual void input(float x, float y)
	{
		if(kind == CI_UI_WIDGET_IMAGESLIDER_H)
		{
			value = imageRect->percentInside(x, y).x;             
		}
		else 
		{
			value = 1.0f-imageRect->percentInside(x, y).y; 
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
    
	virtual void updateLabel()
	{
		if(kind == CI_UI_WIDGET_IMAGESLIDER_H)
		{
			label->setLabel(name + ": " + numToString(getScaledValue(),labelPrecision)); 		
		}		
	}
    
    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
    gl::Texture track;         //back
    gl::Texture progress;      //fill
    gl::Texture handle;        //handle
    gl::Texture handleDown;    //handleOver State    
    int handleHalfWidth; 
    int handleHalfHeight; 
    ciUIRectangle *imageRect;
}; 

#endif
