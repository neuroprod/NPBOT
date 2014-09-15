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

#ifndef CIUI_NUMBER_DIALER
#define CIUI_NUMBER_DIALER

#include "ciUIWidgetWithLabel.h"
#include <math.h>

class ciUINumberDialer : public ciUIWidgetWithLabel
{
public:
    ciUINumberDialer(float x, float y, float _min, float _max, float _value, int _precision, string _name, int _size)
    {
        useReference = false;                                 
        rect = new ciUIRectangle(x,y,0,0); 
        init(_min, _max, &_value, _precision, _name, _size); 
    }
    
    ciUINumberDialer(float _min, float _max, float _value, int _precision, string _name, int _size)
    {
        useReference = false;                                 
        rect = new ciUIRectangle(0,0,0,0); 
        init(_min, _max, &_value, _precision, _name, _size); 
    }    

    ciUINumberDialer(float x, float y, float _min, float _max, float *_value, int _precision, string _name, int _size)
    {
        useReference = true;                                         
        rect = new ciUIRectangle(x,y,0,0); 
        init(_min, _max, _value, _precision, _name, _size); 
    }
    
    ciUINumberDialer(float _min, float _max, float *_value, int _precision, string _name, int _size)
    {
        useReference = true;                                  
        rect = new ciUIRectangle(0,0,0,0); 
        init(_min, _max, _value, _precision, _name, _size); 
    }    
    
    ~ciUINumberDialer()
    {
        if(!useReference)
        {
            delete value; 
        }        
    }
    
    void init(float _min, float _max, float *_value, int _precision, string _name, int _size)
    {
		name = _name; 		
		kind = CI_UI_WIDGET_NUMBERDIALER;  		
        
        if(useReference)
        {
            value = _value; 
        }
        else
        {
            value = new float(); 
            *value = (*_value); 
        }
        
        max = _max; 
        min = _min; 
        if(*value > max)
        {
            *value = max;             
        }
        else if(*value < min)
        {
            *value = min; 
        }
        precision = _precision; 
        
        string minString = numToString(min, precision); 
        string maxString = numToString(max, precision);         
        if(minString.length() > maxString.length())
        {
            textstring = minString; 
            numOfPrecisionZones = minString.length()-1;             //1 for the "-" sign
        }
        else
        {
            textstring = maxString; 
            numOfPrecisionZones = maxString.length(); 
        }
        
        if(precision > 0)
        {
            numOfPrecisionZones -= 1; 
        }
        
		displaystring = textstring; 
        
		paddedRect = new ciUIRectangle(-padding, -padding, padding*2.0f, padding*2.0f);
		paddedRect->setParent(rect); 
		
		label = new ciUILabel(padding*2.0f,0,(name+" LABEL"), textstring, _size); 
		label->setParent(label); 
		label->setRectParent(rect);
        label->setEmbedded(true);        
    }
        
    virtual void update()
    {
        if(useReference)
        {            
            setTextString(numToString(*value, precision));                      
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
	
    float getValue()
    {
        return *value; 
    }

    void setValue(float _value)
    {
        if(_value > max)
        {
            _value = max;             
        }
        else if(_value < min)
        {
            _value = min; 
        }            
        *value = _value; 
        setTextString(numToString(*value, precision));         
    }
    
    void mouseMove(int x, int y ) 
    {
        if(rect->inside((float) x, (float) y))
        {
            state = CI_UI_STATE_OVER;         			
            currentPrecisionZone = ceil(ci::lmap<float>(x,rect->getX(),rect->getX()+rect->getWidth(),0,numOfPrecisionZones));             
            zoneMultiplier = pow(10.0f,numOfPrecisionZones-currentPrecisionZone-precision);                 
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
            *value += zoneMultiplier*(hitPoint.y-y); 
            if(*value > max)
            {
                *value = max;             
            }
            else if(*value < min)
            {
                *value = min; 
            }            
            hitPoint = Vec2f((float) x, (float) y); 
    
            setTextString(numToString(*value, precision));     
			triggerEvent(this);             
            state = CI_UI_STATE_DOWN;         
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
            hitPoint = Vec2f((float) x, (float) y); 
            currentPrecisionZone = ceil(ci::lmap<float>(x,rect->getX(),rect->getX()+rect->getWidth(),0,numOfPrecisionZones));             
            zoneMultiplier = pow(10.0f, numOfPrecisionZones-currentPrecisionZone-precision);     
            state = CI_UI_STATE_DOWN;     
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
                    *value += zoneMultiplier; 
                    if(*value > max)
                    {
                        *value = max;             
                    }
                    else if(*value < min)
                    {
                        *value = min; 
                    }            
            
                    setTextString(numToString(*value, precision));     
                    triggerEvent(this); 
                    break;
                    
                case ci::app::KeyEvent::KEY_UP:
                    *value += zoneMultiplier; 
                    if(*value > max)
                    {
                        *value = max;             
                    }
                    else if(*value < min)
                    {
                        *value = min; 
                    }     
            
                    setTextString(numToString(*value, precision));     
                    triggerEvent(this); 
                    break;
                    
                case ci::app::KeyEvent::KEY_LEFT:
                    *value -= zoneMultiplier; 
                    if(*value > max)
                    {
                        *value = max;             
                    }
                    else if(*value < min)
                    {
                        *value = min; 
                    }     
            
                    setTextString(numToString(*value, precision));     
                    triggerEvent(this); 
                    break;
                    
                case ci::app::KeyEvent::KEY_DOWN:
                    *value -= zoneMultiplier; 
                    if(*value > max)
                    {
                        *value = max;             
                    }
                    else if(*value < min)
                    {
                        *value = min; 
                    }      
            
                    setTextString(numToString(*value, precision));     
                    triggerEvent(this); 
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
				label->focus(); 								
            }
                break;
            case CI_UI_STATE_DOWN:
            {
                draw_fill_highlight = false;            
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
    
	ciUILabel *getLabel()
	{
		return label; 
	}
	
	string getTextString()
	{
		return textstring; 
	}
	
	void setTextString(string s)	
	{
        textstring = s; 
        displaystring = s; 
        while(label->getStringWidth(displaystring) > rect->getWidth()-padding*4.0)
        {
            string::iterator it;
            it=displaystring.begin();
            displaystring.erase (it);                    
        }
        label->setLabel(displaystring);         
    }
	
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
        rect->setHeight(label->getPaddingRect()->getHeight()+padding*2.0f); 
        rect->setWidth(label->getPaddingRect()->getWidth()+padding*2.0f); 
		ciUIRectangle *labelrect = label->getRect(); 
		float h = labelrect->getHeight(); 
		float ph = rect->getHeight(); 	
		
		labelrect->setY(ph/2.0f - h/2.0f); 
 		
		paddedRect->setHeight(rect->getHeight()+padding*2.0f); 
		paddedRect->setWidth(rect->getWidth()+padding*2.0f);

        setTextString(numToString(*value, precision));                     
	}	
    
    bool isDraggable()
    {
        return true; 
    }

    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
	string textstring;  
    string displaystring; 
    int precision; 
	float zoneMultiplier; 
	float *value;  
    bool useReference;     
	float max, min; 
    Vec2f hitPoint; 
    int numOfPrecisionZones; 
    int currentPrecisionZone;
    
}; 

#endif
