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

#ifndef CIUI_SCROLLABLE_CANVAS
#define CIUI_SCROLLABLE_CANVAS

#include "ciUICanvas.h"

class ciUIScrollableCanvas : public ciUICanvas
{    
public:    
    ~ciUIScrollableCanvas()
    {
        delete sRect;
    }
    
    ciUIScrollableCanvas(float x, float y, float w, float h) : ciUICanvas(x, y, w, h)
    {
        initScrollable();
    }
    
    ciUIScrollableCanvas(float x, float y, float w, float h, ciUICanvas *sharedResources) : ciUICanvas(x, y, w, h, sharedResources)
    {
        initScrollable();        
    }
    
    ciUIScrollableCanvas() : ciUICanvas()
    {
        initScrollable();
    }
    
    ciUIScrollableCanvas(ciUICanvas *sharedResources) : ciUICanvas(sharedResources)
    {
        initScrollable();
    }

    void initScrollable()
    {
        kind = CI_UI_WIDGET_SCROLLABLECANVAS;
        sRect = new ciUIRectangle(rect->getRawX(), rect->getRawY(), rect->getWidth(), rect->getHeight());
        isScrolling = false; 
        vel = Vec2f(0.0f,0.0f); 
        pos = ppos = Vec2f(0.0f,0.0f); 
        acc = Vec2f(0.0f,0.0f); 
        damping = .90f; 
        scrollX = false; 
        scrollY = true; 
        
        nearTop = false; 
        nearBot = false;        
        nearRight = false; 
        nearLeft = false;
        
        hitWidget = false; 
        stickyDistance = 100;  
        hit = false; 
#if defined( CINDER_COCOA_TOUCH )
        touchId = -1; 
#endif
    }
    
    void setScrollArea(float x, float y, float w, float h)
    {
        sRect->setX(x); 
        sRect->setY(y); 
        sRect->setWidth(w);
        sRect->setHeight(h);                
    }
    
    void setScrollAreaToScreen()
    {
        sRect->setX(0); 
        sRect->setY(0); 
        sRect->setWidth((float) getWindowWidth());
        sRect->setHeight((float) getWindowHeight());                        
    }
    
    void setScrollAreaToScreenWidth()
    {
        sRect->setX(0); 
        sRect->setWidth((float) getWindowWidth());        
    }    

    void setScrollAreaToScreenHeight()
    {
        sRect->setY(0);        
        sRect->setHeight((float) getWindowHeight());                                
    }
    
    void setScrollableDirections(bool _scrollX, bool _scrollY)
    {
        scrollX = _scrollX;
        scrollY = _scrollY;
    }
    
    void update()
    {		     
        if(!isScrolling)
        {
            if(scrollX)
            {
                float dxLeft = rect->getRawX() - sRect->getRawX(); 
                float dxRight = (sRect->getRawX()+sRect->getWidth()) - (rect->getRawX()+rect->getWidth()); 
                
                if(fabs(dxLeft) < stickyDistance)
                {
                    nearLeft = true;                     
                }            
                else if(fabs(dxRight) < stickyDistance)
                {
                    nearRight = true; 
                }
                else
                {
                    nearLeft = false; 
                    nearRight = false;                     
                }
                
                if(dxLeft > 0)
                {
                    acc.x += (-dxLeft)/10.0f;
                    acc.x -=vel.x*(1.0f-damping); 
                }
                else if(nearLeft)
                {
                    acc.x += (-dxLeft)/10.0f;
                    acc.x -=vel.x*(1.0f-damping); 
                }                
                else if(dxRight > 0)
                {
                    acc.x += (dxRight)/10.0f;
                    acc.x -=vel.x*(1.0f-damping); 	
                }
                else if(nearRight)
                {
                    acc.x += (dxRight)/10.0f;
                    acc.x -=vel.x*(1.0f-damping); 	
                }                
            }
            
            if(scrollY)
            {
                float dyTop = rect->getRawY() - sRect->getRawY(); 
                float dyBot = (sRect->getRawY()+sRect->getHeight()) - (rect->getRawY()+rect->getHeight()); 
                if(fabs(dyTop) < stickyDistance)
                {
                    nearTop = true;                     
                }            
                else if(fabs(dyBot) < stickyDistance)
                {
                    nearBot = true; 
                }
                else
                {
                    nearTop = false; 
                    nearBot = false;                     
                }
                   
                if(dyTop > 0)
                {
                    acc.y += (-dyTop)/10.0f;
                    acc.y -=vel.y*(1.0f-damping); 
                }
                else if(nearTop)
                {
                    acc.y += (-dyTop)/10.0f;
                    acc.y -=vel.y*(1.0f-damping);                     
                }
                else if(dyBot > 0)
                {
                    acc.y += (dyBot)/10.0f;
                    acc.y -=vel.y*(1.0f-damping); 	
                }                                     
                else if(nearBot)
                {
                    acc.y += (dyBot)/10.0f;
                    acc.y -=vel.y*(1.0f-damping); 	
                }
            }
            
            acc.limit(10);
            vel +=acc; 
            vel.limit(50);
            if(scrollX) rect->setX(rect->getRawX() + vel.x);
            if(scrollY) rect->setY(rect->getRawY() + vel.y);             
            
            vel *=damping;    
            acc = Vec2f(0,0); 
        }
        
		for(unsigned int i = 0; i < widgets.size(); i++)
		{            
			widgets[i]->update(); 	
		}		
    }
    
    void drawScrollableRect()
    {
        sRect->draw(); 
    }
    	
#if defined( CINDER_COCOA_TOUCH )
	virtual void touchesMoved(TouchEvent &event) 
    {
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) 
        {                       
            if(touchId == touchIt->getId())
            {
                this->canvasMouseDrag(MouseEvent( MouseEvent::LEFT_DOWN, touchIt->getX(), touchIt->getY(), 0, 0.0, 0 ) );                                                                        
                break; 
            }
        }       
    }
    
	virtual void touchesEnded(TouchEvent &event) 
	{
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) 
        {                       
            if(touchId == touchIt->getId())
            {
                this->canvasMouseUp(MouseEvent( MouseEvent::LEFT_DOWN, touchIt->getX(), touchIt->getY(), 0, 0.0, 0 ) );                                                                        
                touchId = -1;                      
                break; 
            }
        }
    }
    
	virtual void touchesBegan(TouchEvent &event) 
	{		
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) 
        {               
            if(touchId == -1)
            {    
                this->canvasMouseDown(MouseEvent( MouseEvent::LEFT_DOWN, touchIt->getX(), touchIt->getY(), 0, 0.0, 0 ) );                                                                        
                if(hit)
                {
                    touchId = touchIt->getId();    
                }            
            }
        }
	}
    
#endif	
        
    bool canvasMouseDrag( MouseEvent event ) 
    {	
        for(unsigned int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible())	widgets[i]->mouseDrag(event.getX(), event.getY(), event.isRightDown());  
        }
        
        if(hit)
        {
            if(!hitWidget)
            {
                if(isScrolling != true)
                {
                    isScrolling = true; 
                    ppos = Vec2f((float) event.getX(), (float) event.getY());
                    vel = Vec2f(0,0); 
                }
                else
                {
                    pos = Vec2f((float) event.getX(), (float) event.getY());             
                    vel = pos-ppos; 
                    if(scrollX) rect->setX(rect->getRawX() + vel.x);
                    if(scrollY) rect->setY(rect->getRawY() + vel.y);             
                    ppos = pos; 
                }
            }
        }
        return false; 
    }
    
    bool canvasMouseDown( MouseEvent event ) 
    {
        if(sRect->inside((float) event.getX(), (float) event.getY()))
        {
            hit = true; 
            for(unsigned int i = 0; i < widgets.size(); i++)
            {
                if(widgets[i]->isVisible())
                {
                    if(widgets[i]->isHit((float) event.getX(), (float) event.getY()))
                    {            
                        if(widgets[i]->isDraggable())
                        {
                            hitWidget = true;                                                                        
                        }
                        widgets[i]->mouseDown(event.getX(), event.getY(), event.isRightDown()); 
                    }
                }
            }           
        }		
        
        isScrolling = false; 
        vel = Vec2f(0,0);             
        return false;              
    }
    
    bool canvasMouseUp( MouseEvent event ) 
    {	
        for(unsigned int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible()) widgets[i]->mouseUp(event.getX(), event.getY(), event.isRightDown()); 
        }            
        
        hit = false; 
        hitWidget = false; 
        if(isScrolling)
        {
            isScrolling = false; 
            pos = Vec2f((float) event.getX(), (float) event.getY());
        }
        return false;         
    }	


protected:

    ciUIRectangle *sRect; 
    bool isScrolling; 
    bool scrollX, scrollY; 
    bool nearTop, nearBot, nearRight, nearLeft;
    bool hitWidget; 
    Vec2f pos; 
    Vec2f ppos; 
    Vec2f vel; 
    Vec2f acc; 
    float damping;
    float stickyDistance;     
};
    
    
    
#endif
