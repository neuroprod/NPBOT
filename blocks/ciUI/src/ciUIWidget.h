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

#ifndef CIUI_WIDGET
#define CIUI_WIDGET

#include <sstream>
#include "cinder/app/App.h"
#include "cinder/app/TouchEvent.h"

#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Font.h"

class ciUIWidget           
{
protected:        
	ciUIWidget *parent; 
	ciUIRectangle *rect; 	
	
    string name;            //State Properties
	int kind; 
	bool visible; 
    int id;     
    bool hit; 
    int state; 
    bool embedded;
    
    gl::TextureFontRef font;
    
	bool draw_back;         //Rendering Properties 
	bool draw_outline; 
	bool draw_outline_highlight; 
	bool draw_fill; 
	bool draw_fill_highlight; 
    
    ColorA color_back; 
	ColorA color_outline; 
	ColorA color_outline_highlight;	
	ColorA color_fill; 
	ColorA color_fill_highlight; 
    
	float padding;          //Spacing/Padding Purposes
	ciUIRectangle *paddedRect; 	
    
	bool draw_padded_rect; 
	bool draw_padded_rect_outline;     
    ColorA color_padded_rect; 
	ColorA color_padded_rect_outline;
    
#if defined( CINDER_COCOA_TOUCH )
    int touchId;     
#endif    
    bool dead;     
    
public:        
    ciUIWidget() 
    {        
        name = "base"; 
        id = -1; 
        hit = false; 
        visible = true; 
#if defined( CINDER_COCOA_TOUCH )
        touchId = -1; 
#endif
        parent = NULL; 
        state = CI_UI_STATE_NORMAL; 
        draw_back = true; 
        draw_outline = false; 
        draw_fill = false; 
        draw_fill_highlight = false;
        draw_outline_highlight = false;         

        padding = CI_UI_GLOBAL_PADDING; 
		draw_padded_rect = CI_UI_DRAW_PADDING; 						
		draw_padded_rect_outline = CI_UI_DRAW_PADDING_OUTLINE; 						        
        
        color_back = CI_UI_COLOR_BACK;								//the rect's back color
        color_outline = CI_UI_COLOR_OUTLINE;						//the rect's outline color 
        color_outline_highlight = CI_UI_COLOR_OUTLINE_HIGHLIGHT;   //the rect's onMouseOver outline highlight color         
		color_fill = CI_UI_COLOR_FILL;								//the rect's fill color 
        color_fill_highlight = CI_UI_COLOR_FILL_HIGHLIGHT;         //the rect's onMouseDown highlight color 

        color_padded_rect = CI_UI_COLOR_PADDED;
        color_padded_rect_outline = CI_UI_COLOR_PADDED_OUTLINE;     
        
        dead = false; 
        embedded = false; 
    }
    
    virtual ~ciUIWidget() 
    {
        if(rect != NULL)
        {
            delete rect; 
        }
        if(paddedRect != NULL)
        {
            delete paddedRect; 
        }
    }
    
    virtual void update() {}
    virtual void draw() 
    {
        drawPadded();
        drawPaddedOutline();        

        drawBack();
        
        drawOutline();
        drawOutlineHighlight();
        
        drawFill();
        drawFillHighlight();
    }
    
    virtual void drawBack() 
    {
        if(draw_back)
        {

            gl::color(color_back); 
            rect->draw();         
        }
    }
    
    virtual void drawOutline() 
    {
        if(draw_outline)
        {
            gl::color(color_outline); 
            rect->drawOutline(); 
        } 
    }
    
    virtual void drawOutlineHighlight() 
    {
        if(draw_outline_highlight)
        {
            gl::color(color_outline_highlight); 
            rect->drawOutline();          
        }
    }    
    
    virtual void drawFill() 
    {
        if(draw_fill)
        {
            gl::color(color_fill); 
            rect->draw(); 
        }
    }
    
    virtual void drawFillHighlight() 
    {
        if(draw_fill_highlight)
        {
            gl::color(color_fill_highlight); 
            rect->draw(); 
        }    
    }
    
    virtual void drawPadded()
    {
		if(draw_padded_rect)
		{
            gl::color(color_padded_rect); 
			paddedRect->draw(); 
		}                
    }
    
    virtual void drawPaddedOutline()
    {
        if(draw_padded_rect_outline)
		{
            gl::color(color_padded_rect_outline); 
			paddedRect->draw(); 
		}                
    }     
    
    
#if defined( CINDER_COCOA_TOUCH )
    
    void touchesBegan(TouchEvent &event)
    {
        if(touchId == -1)
        {                
            for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) 
            {
                this->mouseDown(touchIt->getX(), touchIt->getY(), 0);
                if(hit)
                {
                    touchId = touchIt->getId();    
                    break; 
                }                        
            }
        }                   
    }
    
    void touchesMoved(TouchEvent &event) 
    {
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) 
        {        
            if(touchId == touchIt->getId())
            {
                this->mouseDrag(touchIt->getX(), touchIt->getY(), 0); 
                break;
            }       
        }
    }
    
    void touchesEnded(TouchEvent &event) 
    {
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) 
        {        
            if(touchId == touchIt->getId())
            {
                this->mouseUp(touchIt->getX(), touchIt->getY(), 0); 
                touchId = -1;   
                break; 
            }
        }
    }
    
#endif

	virtual void mouseMove(int x, int y ) {}
	virtual void mouseDrag(int x, int y, int button) {}
	virtual void mouseDown(int x, int y, int button) {}
	virtual void mouseUp(int x, int y, int button) {}
    virtual void mouseWheel() {}
	
    virtual void keyDown( KeyEvent &event ) {}
	virtual void keyUp( KeyEvent &event ) {}
    
	virtual void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
        rect->setParent(parent->getRect());
        paddedRect->setParent(rect);
	}
	
	virtual void setRectParent(ciUIRectangle *_prect)
	{
		rect->setParent(_prect); 
	}

	virtual ciUIWidget *getParent()
	{
		return parent; 
	}

	virtual ciUIRectangle* getRect()
	{
		return rect; 
	}
	
	virtual void setName(string _name)
	{
		name = _name; 
	}
    
	virtual void setState(int _state)
    {
        state = _state; 
    }
    
	virtual void setDrawPadding(bool _draw_padded_rect)
	{
		draw_padded_rect = _draw_padded_rect; 
	}
    
    virtual void setDrawPaddingOutline(bool _draw_padded_rect_outline)
	{
		draw_padded_rect_outline = _draw_padded_rect_outline; 
	}    
    
	virtual void setDrawBack(bool _draw_back)
	{
		draw_back = _draw_back; 
	}

	virtual void setDrawOutline(bool _draw_outline)
	{
		draw_outline = _draw_outline; 
	}

	virtual void setDrawFill(bool _draw_fill)
	{
		draw_fill = _draw_fill; 
	}
	
	virtual void setDrawFillHighLight(bool _draw_fill_highlight)
	{
		draw_fill_highlight = _draw_fill_highlight; 
	}
	
	virtual void setDrawOutlineHighLight(bool _draw_outline_highlight)
	{
		draw_outline_highlight = _draw_outline_highlight; 
	}
	
	virtual void setColorBack(ColorA _color_back)
	{
		color_back = _color_back; 
	}
		
	virtual void setColorOutline(ColorA _color_outline)
	{
		color_outline = _color_outline; 
	}
	
	virtual void setColorOutlineHighlight(ColorA _color_outline_highlight)
	{
		color_outline_highlight = _color_outline_highlight; 
	}	

	virtual void setColorFill(ColorA _color_fill)
	{
		color_fill = _color_fill; 
	}
	
	virtual void setColorFillHighlight(ColorA _color_fill_highlight)
	{
		color_fill_highlight = _color_fill_highlight; 
	}
	
    virtual void setColorPadded(ColorA _color_padded_rect)
    {
        color_padded_rect = _color_padded_rect; 
    }
    
    virtual void setColorPaddedOutline(ColorA _color_padded_rect_outline)
    {
        color_padded_rect_outline = _color_padded_rect_outline; 
    }
    
	ColorA& getColorPadded()
	{
        return color_padded_rect;
	}

	ColorA& getColorPaddedOutline()
	{
        return color_padded_rect_outline;
	}
    
	ColorA& getColorBack()
	{
		return color_back; 
	}
	
	ColorA& getColorOutline()
	{
		return color_outline; 
	}
	
	ColorA& getColorOutlineHighlight()
	{
		return color_outline_highlight; 
	}	
	
	ColorA& getColorFill()
	{
		return color_fill; 
	}
	
	ColorA& getColorFillHighlight()
	{
		return color_fill_highlight; 
	}
		
    virtual int getKind()
	{
		return kind; 
	}
    	
    virtual void setVisible(bool _visible)
    {
        visible = _visible; 
    }
    
    virtual bool isVisible()
    {
        return visible; 
    }
    
    virtual void toggleVisible()
    {
        visible =! visible; 
    }
    
    virtual bool isHit(float x, float y)
    {
        if(visible)
        {
            return rect->inside(x, y);
        }
        else
        {
            return false; 
        }
    }
    
	virtual string getName()
	{
		return name; 
	}
	
	virtual void triggerEvent(ciUIWidget *child)
	{
		if(parent != NULL)
		{
			parent->triggerEvent(child); 
		}
	}
    
    virtual void triggerSelf()
    {
		if(parent != NULL)
		{
			parent->triggerEvent(this); 
		}        
    }
	
	virtual void setPadding(float _padding)
	{
		padding = _padding; 
		paddedRect->set(-padding, -padding, rect->getWidth()+padding*2.0f, rect->getHeight()+padding*2.0f);
	}
	
	virtual float getPadding()
	{
		return padding; 
	}
	
	virtual ciUIRectangle *getPaddingRect()
	{
		return paddedRect; 
	}

    virtual void stateChange()
    { 
        
    }
    
    virtual bool isDraggable()
    {
        return false; 
    }
    
    virtual bool isEmbedded()
    {
        return embedded; 
    }
    
    virtual void setEmbedded(bool _embedded)
    {
        embedded = _embedded; 
    }
    
    void setID(int _id)
    {
        id = _id; 
    }
    
    int getID()
    {
        return id;
    }
    
    virtual void addWidget(ciUIWidget *widget)
    {
        //Experimental
    }    

    virtual void removeWidget(ciUIWidget *widget)
    {
        //Experimental
    }    
    
    virtual bool hasLabel()
    {
        return false; 
    }
    
    virtual void setFont(gl::TextureFontRef _font)
	{
		font = _font; 
	}
    
    string numToString(float _number, int _precision)
    {
        ostringstream ss;
        ss << fixed << setprecision(_precision);
        ss << _number;
        return ss.str(); 
    }

    string numToString(float _number)
    {
        ostringstream ss;
        ss << _number;
        return ss.str(); 
    }

};

#endif
