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

#ifndef CIUI_LABEL_TOGGLE
#define CIUI_LABEL_TOGGLE

#include "ciUIWidget.h"

class ciUILabelToggle : public ciUIToggle
{
public:
    ciUILabelToggle(float x, float y, float w, bool _value, string _name, int _size = CI_UI_FONT_MEDIUM)
    {
        useReference = false; 
        rect = new ciUIRectangle(x,y,w,0);                                                      
        autoSize = false;         
        init(&_value, _name, _size); 
    }

    ciUILabelToggle(float x, float y, float w, float h, bool _value, string _name, int _size = CI_UI_FONT_MEDIUM)
    {
        useReference = false; 
        rect = new ciUIRectangle(x,y,w,h);                                                      
        autoSize = false;         
        init(&_value, _name, _size); 
    }
    
    ciUILabelToggle(float w, bool _value, string _name, int _size = CI_UI_FONT_MEDIUM, float h = 0)
    {
        useReference = false;         
        rect = new ciUIRectangle(0,0,w,h);                                                      
        autoSize = false;         
        init(&_value, _name, _size); 
    }
    
    ciUILabelToggle(float x, float y, bool _value, string _name, int _size = CI_UI_FONT_MEDIUM)
    {				
        useReference = false;         
        rect = new ciUIRectangle(x,y,0,0);                 
        autoSize = true;         
        init(&_value, _name, _size); 
    }

    ciUILabelToggle(bool _value, string _name, int _size = CI_UI_FONT_MEDIUM)
    {				
        useReference = false;         
        rect = new ciUIRectangle(0,0,0,0);                 
        autoSize = true;         
        init(&_value, _name, _size); 
    }
    
    ciUILabelToggle(float x, float y, float w, bool *_value, string _name, int _size = CI_UI_FONT_MEDIUM)
    {
        useReference = true;         
        rect = new ciUIRectangle(x,y,w,0);                                                      
        autoSize = false;         
        init(_value, _name, _size); 
    }
    
    ciUILabelToggle(float x, float y, float w, float h, bool *_value, string _name, int _size = CI_UI_FONT_MEDIUM)
    {
        useReference = true;         
        rect = new ciUIRectangle(x,y,w,h);                                                      
        autoSize = false;         
        init(_value, _name, _size); 
    }
    
    ciUILabelToggle(float w, bool *_value, string _name, int _size = CI_UI_FONT_MEDIUM, float h = 0)
    {
        useReference = true;                 
        rect = new ciUIRectangle(0,0,w,h);                                                      
        autoSize = false;         
        init(_value, _name, _size); 
    }
    
    ciUILabelToggle(float x, float y, bool *_value, string _name, int _size = CI_UI_FONT_MEDIUM)
    {				
        useReference = true;          
        rect = new ciUIRectangle(x,y,0,0);                 
        autoSize = true;         
        init(_value, _name, _size); 
    }
    
    ciUILabelToggle(bool *_value, string _name, int _size = CI_UI_FONT_MEDIUM)
    {	
        useReference = true;                 
        rect = new ciUIRectangle(0,0,0,0);                 
        autoSize = true;         
        init(_value, _name, _size); 
    }
    
    
    void init(bool *_value, string _name, int _size)
    {        
		name = _name; 		        
        kind = CI_UI_WIDGET_LABELTOGGLE; 		
        paddedRect = new ciUIRectangle(-padding, -padding, padding*2.0f, padding*2.0f);
		paddedRect->setParent(rect); 
        
        label = new ciUILabel((name+" LABEL"), name, _size); 
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
    }	
	
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
        if(rect->getHeight() == 0)
        {
            rect->setHeight(label->getPaddingRect()->getHeight()+padding*2.0f); 
        }
		ciUIRectangle *labelrect = label->getRect(); 
        
        if(autoSize)
        {
            rect->setWidth(label->getPaddingRect()->getWidth()+padding*2.0f);             
        }
        else
        {
            while(labelrect->getWidth()+padding*4.0 > rect->getWidth())
            {
                string labelstring = label->getLabel();
                string::iterator it;
                it=labelstring.end();
                it--; 
                labelstring.erase (it); 
                label->setLabel(labelstring);
            }                        
        }

		float h = labelrect->getHeight(); 
		float ph = rect->getHeight(); 	
        
        float w = labelrect->getWidth(); 
        float pw = rect->getWidth(); 
        
		labelrect->setY((int)(ph*.5f - h*.5f));
        labelrect->setX((int)(pw*.5f - w*.5f-padding*.5f)); 
		paddedRect->setHeight(rect->getHeight()+padding*2.0f);  
        paddedRect->setWidth(rect->getWidth()+padding*2.0f);  
	}	
    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
    bool autoSize; 
}; 

#endif
