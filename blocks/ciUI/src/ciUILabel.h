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

#ifndef CIUI_LABEL
#define CIUI_LABEL

#include "ciUIWidget.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"

class ciUILabel : public ciUIWidget
{
public:    
    ciUILabel()
    {
        
    }
    
    ciUILabel(float x, float y, std::string _name, std::string _label, int _size)
    {
        rect = new ciUIRectangle(x,y,0,0); 
        init(_name, _label, _size); 		
        autoSize = true;
    }

    ciUILabel(float x, float y, std::string _name, int _size)
    {
        rect = new ciUIRectangle(x,y,0,0); 
        init(_name, _name, _size); 		
        autoSize = true;
    }

    ciUILabel(std::string _name, std::string _label, int _size)
    {
        rect = new ciUIRectangle(0,0,0,0); 
        init(_name, _label, _size); 		
        autoSize = true;        
    }	

    ciUILabel(std::string _name, int _size)
    {
        rect = new ciUIRectangle(0,0,0,0); 
        init(_name, _name, _size); 		
        autoSize = true;
    }   
    
    ciUILabel(float x, float y, float w, std::string _name, std::string _label, int _size)
    {
        rect = new ciUIRectangle(x,y,w,0); 
        init(_name, _label, _size); 		
        autoSize = false;
    }
    
    ciUILabel(float x, float y, float w, std::string _name, int _size)
    {
        rect = new ciUIRectangle(x,y,w,0); 
        init(_name, _name, _size); 		
        autoSize = false;
    }
    
    ciUILabel(float w, std::string _name, std::string _label, int _size)
    {
        rect = new ciUIRectangle(0,0,w,0); 
        init(_name, _label, _size); 		
        autoSize = false;        
    }	
    
    ciUILabel(float w, std::string _name, int _size)
    {
        rect = new ciUIRectangle(0,0,w,0); 
        init(_name, _name, _size); 		
        autoSize = false;
    }   

    ciUILabel(float w, std::string _name, int _size, float h)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(_name, _name, _size); 		
        autoSize = false;
    }   
    

    void init(std::string _name, std::string _label, int _size)
    {
		name = _name;
		kind = CI_UI_WIDGET_LABEL; 
		label = _label; 		 				
		size = _size;		
              
        draw_back = CI_UI_LABEL_DRAW_BACK; 
        draw_fill = true; 
		paddedRect = new ciUIRectangle(-padding, -padding, padding*2.0f, padding*2.0f);
		paddedRect->setParent(rect); 
    }
    
    void drawBack()
    {
        if(draw_back)
        {
            drawBackLabel(); 
        }
    }
    
    void drawFill()
    {
        if(draw_fill)
        {
            ci::gl::color(color_fill);
//            font->drawString(label, Vec2f(floor(rect->getX())+xOffset, floor(rect->getY()+padding+yOffset)));
            font->drawString(label, Vec2f(floor(rect->getX())+xOffset, floor(rect->getY()+yOffset)));               //Wonky
        }
	}
	
    void drawFillHighlight()
    {
		if(draw_fill_highlight)
        {
            ci::gl::color(color_fill_highlight);
//            font->drawString(label, Vec2f(floor(rect->getX())+xOffset, floor(rect->getY()+padding+yOffset)));            
            font->drawString(label, Vec2f(floor(rect->getX())+xOffset, floor(rect->getY()+yOffset)));               //Wonky
        }           
	}	
            
    void drawBackLabel()
    {      
        ci::gl::color(color_back);
        font->drawString(label, Vec2f(floor(rect->getX())+CI_UI_LABEL_SHADOW_OFFSET+xOffset, floor(rect->getY()+CI_UI_LABEL_SHADOW_OFFSET+yOffset)));            
    }
    
	void drawString(float x, float y, std::string _string)
	{
        if(visible)
        {
            ci::gl::color(color_fill);
            font->drawString(_string,Vec2f(floor(x), floor(y+yOffset))); 
        }
	}

    void drawStringShadow(float x, float y, std::string _string)
	{
        if(visible)
        {
            ci::gl::color(color_back);
            font->drawString(_string,Vec2f(floor(x)+CI_UI_LABEL_SHADOW_OFFSET, floor(y+yOffset)+CI_UI_LABEL_SHADOW_OFFSET));             
        }
	}
    
	float getStringHeight(std::string s)
	{
        return font->measureString(s).y; 
//        return font->getAscent()+font->getDescent();
	}
    
    float getStringWidth(std::string s)
    {
        return font->measureString(s).x; 
    }
    
    void setLabel(std::string _label)
	{
		label = _label; 		
        if(autoSize)
        {
            float w = getStringWidth(label);                       
            float h = getStringHeight(label);
            
            rect->setWidth(w); 
            rect->setHeight(h); 		 
            paddedRect->setWidth(w+padding*2.0f); 
            paddedRect->setHeight(h+padding*2.0f);
            xOffset = 0;

			// Gaz: Replace with numeric cast eventually...
			yOffset = (int) getStringHeight(label)-padding;
        }
        else
        {
            while(getStringWidth(label) > rect->getWidth()-padding*4.0f)
            {
                std::string::iterator it;
                it=label.begin();
                label.erase (it);

				// Gaz: Quick fix
				if (label.size() == 0)
					break;
            }                        
            float w = getStringWidth(label);                       
            float h = getStringHeight(label);
            if(!(rect->getHeight() > 0))
            {
                rect->setHeight(h);
            }

            paddedRect->setHeight(rect->getHeight()+padding*2.0f);            
            paddedRect->setWidth(rect->getWidth()+padding*2.0f);            
            xOffset = (int) (rect->getWidth()*.5f - w*.5f);
            yOffset = getStringHeight(label)-padding;
        }
	}
        
    std::string getLabel()
    {
        return label; 
    }
	
	void setFont(gl::TextureFontRef _font)
	{
		font = _font; 
		setLabel(label); 
	}
	
	int getSize()
	{
		return size; 
	}
	
	void focus()
	{
		draw_fill_highlight = true; 
		draw_fill = false; 
	}
	
	void unfocus()
	{
		draw_fill_highlight	= false; 		
		draw_fill = true; 
	}
        
protected:    //inherited: ofxUIRectangle *rect; ofxUIWidget *parent; 
	int size; 
	std::string label; 
    bool autoSize;
    int xOffset, yOffset; 
}; 

#endif