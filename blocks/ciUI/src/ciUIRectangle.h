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

#ifndef CIUI_RECTANGLE
#define CIUI_RECTANGLE

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"

class ciUIRectangle 
{
    
protected:         
    float x; 
    float y; 
    float width;
    float height;    
    float halfwidth;
    float halfheight;
    ciUIRectangle *parent; 

public:        
    void setParent(ciUIRectangle *_parent)
    {
        parent = _parent;
    }
    
    ciUIRectangle()
    {
        x = y = width = height = halfheight = halfwidth = 0.0f; 
        setParent(NULL);         
    }
    
    ~ciUIRectangle ()
    {
    
    }
    
    ciUIRectangle(float _x, float _y, float _w, float _h) 
    {
        x = _x;
        y = _y;
        width = _w;
        height = _h;
        halfwidth = width*.5f;                 
        halfheight = height*.5f;                
        setParent(NULL); 
    }
    
    
    ciUIRectangle(ci::Vec2f pos, float _w, float _h) 
    {
        x = pos.x;
        y = pos.y;
        width = _w;
        height = _h;        
        halfwidth = width*.5f;                 
        halfheight = height*.5f;      
        setParent(NULL);
    }
    
    
    ciUIRectangle (ciUIRectangle const & r) 
    {
        x = r.x;
        y = r.y;
        width = r.width;
        height = r.height;
        halfwidth = width*.5f;                 
        halfheight = height*.5f;        
        setParent(NULL);
    }
    
    void set(float px, float py, float w, float h)
    {
        x		= px;
        y		= py;
        width	= w;
        height	= h;
        halfwidth = width*.5f;                 
        halfheight = height*.5f;                            
    }
    
    void set(ci::Vec2f pos, float w, float h)
    {
        x		= pos.x;
        y		= pos.y;
        width	= w;
        height	= h;
        halfwidth = width*.5f;                 
        halfheight = height*.5f;  
    }
    
    void set(ciUIRectangle const & rect)
    {
        x		= rect.x;
        y		= rect.y;
        width	= rect.width;
        height	= rect.height;
        halfwidth = width*.5f;                 
        halfheight = height*.5f;            
    }
    
    void setFromCenter(float px, float py, float w, float h)
    {
        x		= px - w*0.5f;
        y		= py - h*0.5f;
        width	= w;
        height	= h;
        halfwidth = width*.5f;                 
        halfheight = height*.5f;                 
    }
    
    void setFromCenter(ci::Vec2f pos, float w, float h)
    {
        x		= pos.x - w*0.5f;
        y		= pos.y - h*0.5f;
        width	= w;
        height	= h;
        halfwidth = width*.5f;                 
        halfheight = height*.5f;              
    }
    
    ciUIRectangle & operator = (ciUIRectangle const & rect)
    {
        x = rect.x;
        y = rect.y;
        width = rect.width;
        height = rect.height;
        halfwidth = width*.5f;                 
        halfheight = height*.5f;                                 
        return *this;
    }
    
    ciUIRectangle & operator + (const ci::Vec2f & point)
    {
        x += point.x;
        y += point.y;                 
        return *this;
    }
    
    bool operator == (ciUIRectangle const & r)
    {
        return (x == r.x) && (y == r.y) && (width == r.width) && (height == r.height);
    }
    
    bool operator != (ciUIRectangle const & r)
    {
        return (x != r.x) || (y != r.y) || (width != r.width) || (height != r.height);
    }
     
    void setX(float _x)
    {
        x = _x;             
    }
    
	void setY(float _y)
    {
        y = _y;         
    }
    

    void setHeight(float _height)
	{
		height = _height; 
        halfheight = height*.5f;               
	}
	
	void setWidth(float _width)
	{
		width = _width; 
        halfwidth = width*.5f;               
	}

    
    ci::Vec2f getCenter ()
    {
        if(parent !=NULL)
        {
            return ci::Vec2f((x+parent->getX()) + halfwidth, (y+parent->getY()) + halfheight);
        }
        else
        {
            return ci::Vec2f(x + halfwidth, y + halfheight);
        }        
    }
    
    bool inside(ci::Vec2f p)
    {    
        if(parent !=NULL)
        {
            return insideParent(p.x, p.y); 
        }
        else
        {
            return insideChild(p.x, p.y);
        }
    }

    bool inside(float px, float py)
    {
        if(parent != NULL)
        {
            return insideParent(px,py); 
        }
        else
        {
            return insideChild(px,py); 
        }
    }
    
    bool insideChild(float px, float py)
    {
        if( px > x && py > y && px < x + width && py < y + height )
        {
            return true;
        }
        return false;    
    }
    
    bool insideParent(float px, float py)
    {
        if( px > (x+parent->getX()) && py > (y+parent->getY()) && px < (x+parent->getX()+width) && py < (y+parent->getY()+height))
        {
            return true;
        }
        return false;    
    }
	
	ci::Vec2f percentInside(float px, float py)				//Assumes your already inside rect 
	{
		if(parent != NULL)
		{
			return percentInsideParent(px,py); 
		}
		else
		{
			return percentInsideChild(px,py); 
		}		
	}
	
	ci::Vec2f percentInsideChild(float px, float py)
    {
		return ci::Vec2f((px-x)/(width), (py-y)/(height)); 
    }
    
    ci::Vec2f percentInsideParent(float px, float py)
    {		
		return ci::Vec2f((px-(x+parent->getX()))/(width), (py-(y+parent->getY()))/(height)); 
	}
    
    void draw()
    {
        if(parent != NULL)
        {                
            ci::gl::drawSolidRect(ci::Rectf(parent->getX()+x,parent->getY()+y,parent->getX()+x+width,parent->getY()+y+height));
        }
        else
        {
            ci::gl::drawSolidRect(ci::Rectf(x,y,x+width,y+height));
        }
    }	
    
    void drawOutline()
    {
        if(parent != NULL)
        {                
            ci::gl::drawStrokedRect(ci::Rectf(parent->getX()+x,parent->getY()+y,parent->getX()+x+width,parent->getY()+y+height));
        }
        else
        {
            ci::gl::drawStrokedRect(ci::Rectf(x,y,x+width,y+height));
        }      
    }	    
	
	float getX()
	{
        if(parent != NULL)
        {            
            return (x+parent->getX()); 
        }
        else
        {
			return x; 
        }		
	}
	
	float getY()
	{
        if(parent != NULL)
        {            
            return (y+parent->getY()); 
        }
        else
        {
			return y; 
        }				
	}
    
    float getRawX()
    {
        return x; 
    }
    
    float getRawY()
    {
        return y; 
    }
    
	float getWidth()
	{
		return width; 
	}
	
	float getHeight()
	{
		return height; 
	}
	
    float getHalfWidth()
	{
		return halfwidth; 
	}
	
	float getHalfHeight()
	{
		return halfheight; 
	}	
};

#endif














