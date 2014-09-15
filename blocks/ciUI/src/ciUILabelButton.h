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

#ifndef CIUI_LABEL_BUTTON
#define CIUI_LABEL_BUTTON

#include "ciUIWidget.h"

class ciUILabelButton : public ciUIButton
{
    
    string  labelText;
public:
    ciUILabelButton(float x, float y, float w, bool _value, string _name, int _size)
    {
        useReference = false;         
        rect = new ciUIRectangle(x,y,w,0); 
        autoSize = false;         
        init(&_value, _name, _size); 
    }

    ciUILabelButton(float w, bool _value, string _name, int _size, string _label,float h = 0)
    {
        useReference = false;         
        rect = new ciUIRectangle(0,0,w,h); 
        autoSize = false;         
        init(&_value, _name, _size,_label );
    }

    ciUILabelButton(float x, float y, bool _value, string _name, int _size,string _label)
    {
        useReference = false;         
        rect = new ciUIRectangle(x,y,0,0); 
        autoSize = true;         
        init(&_value, _name, _size,_label);
    }

    ciUILabelButton(bool _value, string _name, int _size,string _label)
    {
        useReference = false;         
        rect = new ciUIRectangle(0,0,0,0); 
        autoSize = true;         
        init(&_value, _name, _size,_label);
    }
    
    ciUILabelButton(float x, float y, float w, bool *_value, string _name, int _size)
    {
        useReference = true; 
        rect = new ciUIRectangle(x,y,w,0); 
        autoSize = false;         
        init(_value, _name, _size); 
    }
    
    ciUILabelButton(float w, bool *_value, string _name, int _size, float h = 0)
    {
        useReference = true;         
        rect = new ciUIRectangle(0,0,w,h); 
        autoSize = false;         
        init(_value, _name, _size); 
    }
    
    ciUILabelButton(float x, float y, bool *_value, string _name, int _size)
    {
        useReference = true;         
        rect = new ciUIRectangle(x,y,0,0); 
        autoSize = true;         
        init(_value, _name, _size); 
    }
    
    ciUILabelButton(bool *_value, string _name, int _size)
    {
        useReference = true;         
        rect = new ciUIRectangle(0,0,0,0); 
        autoSize = true;         
        init(_value, _name, _size); 
    }    
    
    void init(bool *_value, string _name, int _size, string _label="")
    {
        name = _name; 		
        labelText=_label;
        label = new ciUILabel(0,0,(labelText+" LABEL"),  labelText, _size);
		label->setParent(label); 
		label->setRectParent(rect);         
        label->setEmbedded(true);
		kind = CI_UI_WIDGET_LABELBUTTON; 	
        paddedRect = new ciUIRectangle(-padding, -padding, padding*2.0f, padding*2.0f);
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
            while(labelrect->getWidth() > rect->getWidth())
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
        labelrect->setX((int)(pw*.5f - w*.5f)); 
		paddedRect->setHeight(rect->getHeight()+padding*2.0f);  
        paddedRect->setWidth(rect->getWidth()+padding*2.0f);  
	}	
    
    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
    bool autoSize; 
}; 

#endif
