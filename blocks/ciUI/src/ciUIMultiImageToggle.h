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

#ifndef CIUI_MULTI_IMAGE_TOGGLE
#define CIUI_MULTI_IMAGE_TOGGLE

#include "ciUIToggle.h"

class ciUIMultiImageToggle : public ciUIToggle
{
public:        
    ciUIMultiImageToggle(float x, float y, float w, float h, bool _value, string _pathURL, string _name,  int _size= CI_UI_FONT_SMALL)
    {
        useReference = false;         
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, &_value, _pathURL, _name, _size);
    }
    
    ciUIMultiImageToggle(float w, float h, bool _value, string _pathURL, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = false;         
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, &_value, _pathURL, _name, _size);        
    } 
    
    ciUIMultiImageToggle(float x, float y, float w, float h, bool *_value, string _pathURL, string _name,  int _size= CI_UI_FONT_SMALL)
    {
        useReference = true;         
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _value, _pathURL, _name, _size);
    }
    
    ciUIMultiImageToggle(float w, float h, bool *_value, string _pathURL, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = true;  
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _value, _pathURL, _name, _size);        
    }     
    
    ~ciUIMultiImageToggle()
    {
    }
    
    void init(float w, float h, bool *_value, string _pathURL, string _name, int _size = CI_UI_FONT_SMALL)
    {
		name = _name; 		
		kind = CI_UI_WIDGET_MULTIIMAGETOGGLE;
        
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding*2.0f);
		paddedRect->setParent(rect); 
        
		label = new ciUILabel(w+padding*2.0f,0, (name+" LABEL"), name, _size); 
		label->setParent(label); 
		label->setRectParent(rect); 
        label->setEmbedded(true);		

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
        drawLabel = true;
        label->setVisible(drawLabel);      
        
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
        
        back = loadImage(_pathURL);         
        down = loadImage(coreURL+"down"+extension);         
        over = loadImage(coreURL+"over"+extension);         
        on = loadImage(coreURL+"on"+extension);         
    }       
    
    
    
    void drawBack()                     //NORMAL
    {
        if(draw_back)
        {
            ci::gl::color(ColorA(1.0f,1.0f,1.0f,1.0f));
            gl::draw(back, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));                        
        }
    }
    
    void drawOutlineHighlight()         //OVER
    {
        if(draw_outline_highlight)
        {
            ci::gl::color(ColorA(1.0f,1.0f,1.0f,1.0f));
            gl::draw(over, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));                        
        }
    }    
    
    void drawFill()                     
    {
        if(draw_fill)
        {
            ci::gl::color(ColorA(1.0f,1.0f,1.0f,1.0f));
            gl::draw(on, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));                        
        }        
    }
    
    void drawFillHighlight()            //DOWN/ON
    {
        if(draw_fill_highlight)
        {
            ci::gl::color(ColorA(1.0f,1.0f,1.0f,1.0f));
            gl::draw(down, Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth(), rect->getY()+rect->getHeight()));                        
        }        
    }
    
    
    virtual void stateChange()
    {                
        switch (state) {
            case CI_UI_STATE_NORMAL:
            {            
                draw_back = true; 
                draw_fill_highlight = false;             
                draw_outline_highlight = false;  
				label->unfocus(); 								
            }
                break;
            case CI_UI_STATE_OVER:
            {
                draw_back = false;                 
                draw_fill_highlight = false;            
                draw_outline_highlight = true;  
				label->focus(); 								
            }
                break;
            case CI_UI_STATE_DOWN:
            {
                draw_back = false;                  
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
    

    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
    gl::Texture back; 
    gl::Texture over; 
    gl::Texture down;
    gl::Texture on; 
}; 

#endif

