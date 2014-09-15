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

#ifndef CIUI_TEXT_INPUT
#define CIUI_TEXT_INPUT

#include "ciUIWidgetWithLabel.h"

class ciUITextInput : public ciUIWidgetWithLabel
{
public:
    ciUITextInput(float x, float y, float w, string _name, string _textstring, int _size, float h = 0)
    {
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, _name, _textstring, _size); 
    }
    
    ciUITextInput(float w, string _name, string _textstring, int _size, float h = 0)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, _name, _textstring, _size); 
    }    
    
    void init(float w, string _name, string _textstring, int _size)
    {
		name = _name; 		
		kind = CI_UI_WIDGET_TEXTINPUT; 		
		textstring = _textstring; 
		defaultstring = _textstring; 
		displaystring = _textstring; 
        
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, padding*2.0f);
		paddedRect->setParent(rect); 
        
		clicked = false;                                            //the widget's value
        autoclear = true; 
		
		label = new ciUILabel(padding*2.0f,0,(name+" LABEL"), _size); 
		label->setParent(label); 
		label->setRectParent(rect); 
        label->setEmbedded(true);
        
		triggerType = CI_UI_TEXTINPUT_ON_FOCUS;
		cursorWidth = 0; spaceOffset = 0; 		
		theta = 0;         
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
            ci::gl::color(color_fill); 
            rect->draw(); 
        }
        if(clicked)
		{						            
			ci::gl::color(label->getColorFillHighlight().r,label->getColorFillHighlight().g,label->getColorFillHighlight().b, fabs(cos(theta))); 
			theta +=0.05f; 
			
			spaceOffset = label->getStringWidth(displaystring)-cursorWidth; 
            if(spaceOffset > rect->getWidth()-padding*4.0f)
            {
                spaceOffset = rect->getWidth()-padding*4.0f; 
            }
			float x = label->getRect()->getX()+spaceOffset;			
			float y = label->getRect()->getY()-padding; 
			float t = label->getRect()->getHeight()+padding*2.0f; 			
            ci::gl::drawSolidRect(Rectf(x, y, x+cursorWidth, y+t)); 
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
            unClick(); 
        }
        stateChange();         
    }
    
    void mouseDrag(int x, int y, int button) 
    {
        if(hit)
        {
            state = CI_UI_STATE_DOWN;         
        }    
        else
        {
            state = CI_UI_STATE_NORMAL;  
            unClick(); 
        }
        stateChange();     
    }
    
    void mouseDown(int x, int y, int button) 
    {
        if(rect->inside((float) x, (float) y))
        {
			if(state == CI_UI_STATE_OVER)
			{
				clicked = true; 
				theta = 0; 
                hit = true; 
			}
#if defined( CINDER_COCOA_TOUCH )
			clicked = true;
			theta = 0;
			hit = true;
#endif
            state = CI_UI_STATE_DOWN;     
			triggerType = CI_UI_TEXTINPUT_ON_FOCUS; 
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
        }    
        else
        {
            state = CI_UI_STATE_NORMAL;         
        }
        hit = false; 
        stateChange();         
    }
	
    void keyDown( KeyEvent &event )
    {    
		if(clicked)            
		{
            switch (event.getCode()) 
			{
				case ci::app::KeyEvent::KEY_BACKSPACE:
					if (textstring.size() > 0) 
					{
						textstring.erase(textstring.size()-1); 
                        displaystring = textstring;                         
                        while(label->getStringWidth(displaystring) > rect->getWidth()-padding*4.0)
                        {
                            string::iterator it;
                            it=displaystring.begin();
                            displaystring.erase (it);                    
                        }
						label->setLabel(displaystring);                           
					}
					break;
					
				case ci::app::KeyEvent::KEY_RETURN:
					triggerType = CI_UI_TEXTINPUT_ON_ENTER; 					
					triggerEvent(this); 			
					if(autoclear)
					{
						textstring.clear(); 
                        displaystring = textstring;                         
						label->setLabel(displaystring); 
					}
					break;
						
                        
				default:
                {
                    textstring+=event.getChar(); 
                    displaystring+=event.getChar(); 
                    while(label->getStringWidth(displaystring) > rect->getWidth()-padding*4.0)
                    {
                        string::iterator it;
                        it=displaystring.begin();
                        displaystring.erase (it);                    
                    }
                    label->setLabel(displaystring);                         
                }
					break;
			}
            float h = label->getRect()->getHeight(); 			
			float ph = rect->getHeight(); 
			label->getRect()->setY(ph/2.0f - h/2.0f); 
        }        
    }
    
    void unClick()
    {
        if(clicked)
        {
            clicked = false;          
			triggerType = CI_UI_TEXTINPUT_ON_UNFOCUS; 
			triggerEvent(this);             
        }     
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
    
	bool isClicked()
	{
		return clicked; 
	}
	
	ciUILabel *getLabel()
	{
		return label; 
	}
	
	string getTextString()
	{
		return textstring; 
	}
	
	int getTriggerType()
	{
		return triggerType; 
	}
	
	void setTextString(string s)	
	{
		textstring = ""; 
		string temp = ""; 
		
        int length = s.length(); 
        
        if(length > 0)
        {
            for(int i = 0; i < length; i++)
            {
                temp+=s.at(i); 
                float newWidth = label->getStringWidth(temp); 
                
                if(newWidth < rect->getWidth()-padding*4.0)
                {
                    textstring+=s.at(i); 
                    label->setLabel(textstring); 
                }				
            }		
        }
        else
        {
            textstring = s; 
            label->setLabel(textstring);                
        }
        float h = label->getRect()->getHeight(); 			
        float ph = rect->getHeight(); 
        label->getRect()->setY(ph/2.0f - h/2.0f);         
        displaystring = textstring; 
	}
	
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
        if(rect->getHeight() == 0)
        {
            rect->setHeight(label->getPaddingRect()->getHeight()+padding*2.0f); 
        }
        label->setLabel(textstring);
		ciUIRectangle *labelrect = label->getRect(); 
		float h = labelrect->getHeight(); 
		float ph = rect->getHeight(); 	
		
		labelrect->setY(ph/2.0f - h/2.0f); 
		defaultY = labelrect->getRawY()+labelrect->getHeight(); 
		defaultX = labelrect->getRawX(); 
 		
		paddedRect->setHeight(rect->getHeight()+padding*2.0f);
		
		cursorWidth = label->getStringWidth("."); 
        
        while(label->getStringWidth(textstring) > rect->getWidth()-padding*4.0)
        {
            string::iterator it;
            it=textstring.begin();
            textstring.erase (it);                    
        }        
        
        defaultstring = textstring; 
		displaystring = textstring; 
        setTextString(textstring);        
	}	
	
	void setAutoClear(bool _autoclear)
	{
		autoclear = _autoclear; 
	}

    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
	string textstring; 
	string defaultstring; 
    string displaystring; 
	bool clicked; 
	float theta; 
	float cursorWidth; 
	float spaceOffset;		
	bool autoclear; 
	float defaultY, defaultX; 	
	int triggerType;
    int maxsize; 
}; 

#endif
