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

#ifndef CIUI_DROP_DOWN_LIST
#define CIUI_DROP_DOWN_LIST

#include "ciUIWidget.h"

class ciUIDropDownList : public ciUIToggle
{
public:    
    ciUIDropDownList(float x, float y, float w, string _name, vector<string> items, int _size)
    {
        useReference = false; 
        rect = new ciUIRectangle(x,y,w,0);                     
        autoSize = false;         
        init(_name, items, _size);         
    }
    
    ciUIDropDownList(float w, string _name, vector<string> items, int _size)
    {
        useReference = false; 
        rect = new ciUIRectangle(0,0,w,0);                     
        autoSize = false;         
        init(_name, items, _size);         
    }    
    
    ciUIDropDownList(float x, float y, string _name, vector<string> items, int _size)
    {
        useReference = false;         
        rect = new ciUIRectangle(x,y,0,0); 
        autoSize = true; 
        init(_name, items, _size);         
    }
    
    ciUIDropDownList(string _name, vector<string> items, int _size)
    {
        useReference = false;                 
        rect = new ciUIRectangle(0,0,0,0); 
        autoSize = true; 
        init(_name, items, _size);         
    }    
    
    void init(string _name, vector<string> items, int _size)
    {
		name = _name; 		        
		kind = CI_UI_WIDGET_DROPDOWNLIST; 		        
		paddedRect = new ciUIRectangle(-padding, -padding, padding*2.0f, padding*2.0f);
		paddedRect->setParent(rect);     
        
        size = _size; 
        label = new ciUILabel(0,0,(name+" LABEL"), name, _size); 
		label->setParent(label); 
		label->setRectParent(rect);
        label->setEmbedded(true);

        value = new bool(); 
        *value = false; 
        draw_fill = *value;
        
        allowMultiple = false; 
        initToggles(items, _size);         
        autoClose = false; 
    }

    void clearToggles()
    {        
        while(toggles.size())
		{
			ciUILabelToggle *t = toggles[0]; 			
            removeToggle(t->getName());
        }
    }    
    
    
    void addToggle(string toggleName)
    {        
        float yt = rect->getHeight();
        
		for(unsigned int i = 0; i < toggles.size(); i++)
		{
			ciUILabelToggle *t = toggles[i]; 			
            yt +=t->getRect()->getHeight();         
		}	
        
        ciUILabelToggle *ltoggle;         
        if(autoSize)
        {
            ltoggle = new ciUILabelToggle(0, yt, false, toggleName, size);                 
        }
        else
        {
            ltoggle = new ciUILabelToggle(0, yt, rect->getWidth(), rect->getHeight(), false, toggleName, size);                 
        }                
        ltoggle->getRect()->setParent(this->getRect());
        ltoggle->getRect()->setY(rect->getRawY()+yt); 			       
        ltoggle->getRect()->setX(rect->getRawX()); 			        
        ltoggle->setVisible(*value); 
        ltoggle->setLabelVisible(*value);             
        toggles.push_back(ltoggle);                 
        parent->addWidget(ltoggle);
        ltoggle->setParent(this);        
        if(isOpen())
        {
            open(); 
        }           
        else
        {
            close();
        }        
    }    
    
    void removeToggle(string toggleName)
    {
        ciUILabelToggle *t = NULL; 
        for(unsigned int i = 0; i < toggles.size(); i++)
        {
            ciUILabelToggle *other = (ciUILabelToggle *)toggles[i];
            if(other->getName() == toggleName)
            {
                t = other;
                toggles.erase(toggles.begin()+i);                                             
                break; 
            }
        }
        for(unsigned int i = 0; i < selected.size(); i++)
        {
            ciUILabelToggle *other = (ciUILabelToggle *)selected[i];
            if(other->getName() == toggleName)
            {
                selected.erase(selected.begin()+i);                                             
                break; 
            }
        }                        
        if(t != NULL)
        {
            parent->removeWidget(t);
            
            float yt = rect->getHeight();
            for(unsigned int i = 0; i < toggles.size(); i++)
            {
                ciUILabelToggle *t = toggles[i]; 			
                t->setParent(this); 
                t->getRect()->setParent(this->getRect());                 
                t->getRect()->setY(yt); 
                t->getRect()->setX(0); 
                yt +=t->getRect()->getHeight();         
            }		            
            
        }
        
    }
    
    vector<ciUIWidget *> & getSelected()
    {                 
        return selected;         
    }
    
    virtual void setDrawPadding(bool _draw_padded_rect)
	{
		draw_padded_rect = _draw_padded_rect; 
        label->setDrawPadding(false);
//        for(int i = 0; i < toggles.size(); i++)s
//        {
//            ciUILabelToggle * toggle = (ciUILabelToggle *) toggles[i];
//            toggle->setDrawPadding(false);
//        }
	}
    
    virtual void setDrawPaddingOutline(bool _draw_padded_rect_outline)
	{
		draw_padded_rect_outline = _draw_padded_rect_outline; 
        label->setDrawPaddingOutline(false);
//        for(int i = 0; i < toggles.size(); i++)
//        {
//            ciUILabelToggle * toggle = (ciUILabelToggle *) toggles[i];
//            toggle->setDrawPaddingOutline(false);
//        }
    }  

    void initToggles(vector<string> &items, int _size)
    {
        float ty = 20;
		for(unsigned int i = 0; i < items.size(); i++)
		{
			string tname = items[i]; 
			ciUILabelToggle *ltoggle; 
            if(autoSize)
            {
                ltoggle = new ciUILabelToggle(0, ty, false, tname, _size);                 
            }
            else
            {
                ltoggle = new ciUILabelToggle(0, ty, rect->getWidth(), false, tname, _size);                 
            }
            ltoggle->setVisible(*value); 
            ltoggle->setLabelVisible(*value);             
			toggles.push_back(ltoggle); 
            ty+=20; 
		}        
    }
    
    void setParent(ciUIWidget *_parent)
	{
		parent = _parent;         
        rect->setHeight(label->getPaddingRect()->getHeight()+padding*2.0f); 
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
                    
		float yt = rect->getHeight();
		for(int i = 0; i < toggles.size(); i++)
		{
			ciUILabelToggle *t = toggles[i]; 			
			t->setParent(this); 
			t->getRect()->setParent(this->getRect()); 
        
            t->getRect()->setY(yt); 			
            yt +=t->getRect()->getHeight();         
            if(autoSize)
            {
                t->getRect()->setWidth(rect->getWidth());
            }
            t->getPaddingRect()->setWidth(paddedRect->getWidth()); 
		}			
	}	    
    
    void mouseUp(int x, int y, int button) 
    {
        if(rect->inside((float) x, (float) y) && hit)
        {
            setValue(!(*value));
            setToggleVisibility(*value); 
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
        
    void setAutoClose(bool _autoClose)
    {
        autoClose = _autoClose; 
    }
    
    void open()
    {
        *value = true; 
        setToggleVisibility(*value); 
    }
    
    void close()
    {
        *value = false; 
        setToggleVisibility(*value); 
    }
    
    void setVisible(bool _visible)
    {
        visible = _visible; 
        label->setVisible(visible); 
        for(unsigned int i = 0; i < toggles.size(); i++)
        {
            ciUILabelToggle * toggle = (ciUILabelToggle *) toggles[i];
            toggle->setVisible(visible); 
        }
    }
    
    void setToggleVisibility(bool _value)
    {
        for(unsigned int i = 0; i < toggles.size(); i++)
        {
            ciUILabelToggle * toggle = (ciUILabelToggle *) toggles[i];
            toggle->setVisible(_value); 
            toggle->setLabelVisible(_value); 
        }
    }
    
    vector<ciUILabelToggle *> &getToggles()
	{
		return toggles; 
	}

    void triggerEvent(ciUIWidget *child)
	{        
        if(child == this)
        {
            parent->triggerEvent(child); 
            return; 
        }

        if(autoClose)
        {
            if(isOpen())
            {
                close();
            }            
        }            
        
        if(!allowMultiple)
        {
            activateToggle(child->getName().c_str()); 
        }
        
        selected.clear();
        for(unsigned int i = 0; i < toggles.size(); i++)
		{
			ciUILabelToggle *t = toggles[i]; 		
            if(t->getValue())
            {
                selected.push_back(t);
            }                           
        }        
        
		if(parent != NULL)
		{
            parent->triggerEvent(this);
			parent->triggerEvent(child); 
		}        
	}	    
    
	void activateToggle(string _name)
	{
		for(unsigned int i = 0; i < toggles.size(); i++)
		{
			ciUILabelToggle *t = toggles[i]; 			
			if(!(t->getName().compare(_name.c_str())))
			{
				t->setValue(true); 					
			}
			else 
			{
                t->setValue(false); 
			}			
		}
	}    
    
    void setAllowMultiple(bool _allowMultiple)
    {
        allowMultiple = _allowMultiple; 
    }
    
    bool isOpen()
    {
        return *value; 
    }
    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
    bool autoSize; 
    bool autoClose; 
    vector<ciUILabelToggle *> toggles; 
    vector<ciUIWidget *> selected; 
    bool allowMultiple; 
    int size;     
}; 

#endif
