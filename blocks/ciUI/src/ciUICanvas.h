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

#ifndef CIUI_CANVAS
#define CIUI_CANVAS

#include "cinder/app/App.h"

#if defined( CINDER_COCOA_TOUCH )
#include "cinder/app/AppCocoaTouch.h"
#endif
#include "cinder/app/AppBasic.h"

#include "cinder/Xml.h"
#include "cinder/Utilities.h"
#include "cinder/Font.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/gl/gl.h"
#include "ciUIWidget.h"

#include <vector>
#include <map>

class ciUICanvas : public ciUIWidget
{    
public:	
    ~ciUICanvas() 
    {
		delete GUIevent; 
		for(unsigned int i = 0; i < widgets.size(); i++)
		{
			ciUIWidget *w = widgets[i]; 
			delete w; 
		}
		widgets.clear();             
    }
    
    ciUICanvas(float x, float y, float w, float h) 
    {
        rect = new ciUIRectangle(x,y,w,h);        
        init(w, h);
    }

    ciUICanvas(float x, float y, float w, float h, ciUICanvas *sharedResources) 
    {
        rect = new ciUIRectangle(x,y,w,h);        
        init(w, h, sharedResources);
    }
    
    ciUICanvas() 
    {
        float w = (float) ci::app::getWindowWidth();
        float h = (float) ci::app::getWindowHeight();
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h);
        setDrawBack(false); 
    }

    ciUICanvas(ciUICanvas *sharedResources) 
    {
        float w = (float) ci::app::getWindowWidth();
        float h = (float) ci::app::getWindowHeight();
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, sharedResources);
        setDrawBack(false); 
    }
    
    void init(int w, int h, ciUICanvas *sharedResources = NULL)
    {
        name = "CI_UI_WIDGET_CANVAS"; 
		kind = CI_UI_WIDGET_CANVAS; 

#if defined( CINDER_COCOA_TOUCH )
        mApp = (app::AppCocoaTouch *) app::App::get();                 
#else
        mApp = app::App::get();                 
#endif 

		enabled = false; 		
		enable(); 
		
		enable_highlight_outline = false; 
		enable_highlight_fill = false; 
        
		GUIevent = new ciUIEvent(this); 
        
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding*2.0f);
		paddedRect->setParent(rect);

        if(sharedResources != NULL)
        {
            hasSharedResources = true; 
            
            font_large = sharedResources->getFontLarge();
            font_medium = sharedResources->getFontMedium();
            font_small = sharedResources->getFontSmall();
            
            fontName = sharedResources->getFontName();
        }
        else
        {
            hasSharedResources = false;             
            fontName = CI_UI_FONT_NAME;
            setFont(fontName);         
        }
                
		font = font_medium; 
		lastAdded = NULL; 
        uniqueIDs = 0;         
        widgetSpacing = CI_UI_GLOBAL_WIDGET_SPACING; 
        hasKeyBoard = false; 
    }   

	// Gaz: This function may not be completely stable yet...
	void resize(int w, int h)
	{
		rect->set(0, 0, w, h);
		paddedRect->set(-padding, -padding, w+padding*2.0f, h+padding*2.0f);
	}
    
    void saveSettings(string fileName)
    {
        try
        {
            XmlTree settings( "Settings", "" );
            for(unsigned int i = 0; i < widgetsWithState.size(); i++)
            {                
                XmlTree widget( "Widget", "" );
                widget.push_back( XmlTree( "Kind", numToString(widgetsWithState[i]->getKind(),0) ) );
                widget.push_back( XmlTree( "Name", widgetsWithState[i]->getName() ) );
                writeSpecificWidgetData(widgetsWithState[i], widget); 
                settings.push_back( widget );
            }
            std::string filePath = "settings/"+fileName;
#if defined( CINDER_COCOA )
            filePath = ci::app::App::getResourcePath().string()+"/"+filePath;
#endif
            settings.write( writeFile( filePath , true) );
        }
        catch (Exception e)
        {
            cout << "CIUI: Could not save file: " << fileName << endl; 
        }
    }
    
    void writeSpecificWidgetData(ciUIWidget *widget, XmlTree &XML)
    {
        int kind = widget->getKind();        
        switch (kind) {
            case CI_UI_WIDGET_IMAGETOGGLE:    
            case CI_UI_WIDGET_MULTIIMAGETOGGLE: 
            case CI_UI_WIDGET_LABELTOGGLE:                
            case CI_UI_WIDGET_TOGGLE:
            {
                ciUIToggle *toggle = (ciUIToggle *) widget; 
                XML.push_back( XmlTree( "Value", numToString( toggle->getValue() ? 1 : 0) ) );                
            }
                break;
                
            case CI_UI_WIDGET_MULTIIMAGESLIDER_H:
            case CI_UI_WIDGET_MULTIIMAGESLIDER_V:                                 
            case CI_UI_WIDGET_IMAGESLIDER_H:
            case CI_UI_WIDGET_IMAGESLIDER_V:
            case CI_UI_WIDGET_BILABELSLIDER:
            case CI_UI_WIDGET_CIRCLESLIDER:
            case CI_UI_WIDGET_MINIMALSLIDER:                
            case CI_UI_WIDGET_SLIDER_H:
            case CI_UI_WIDGET_SLIDER_V:
            {
                ciUISlider *slider = (ciUISlider *) widget; 
                XML.push_back( XmlTree( "Value", numToString( slider->getScaledValue() ) )  );
            }
                break;

            case CI_UI_WIDGET_RSLIDER_H:
            case CI_UI_WIDGET_RSLIDER_V:
            {
                ciUIRangeSlider *rslider = (ciUIRangeSlider *) widget; 
                XML.push_back( XmlTree( "HighValue", numToString( rslider->getScaledValueHigh() ) ) ); 
                XML.push_back( XmlTree( "LowValue", numToString( rslider->getScaledValueLow() ) ) );              
            }
                break;

            case CI_UI_WIDGET_NUMBERDIALER:
            {
                ciUINumberDialer *numdialer = (ciUINumberDialer *) widget; 
                XML.push_back( XmlTree( "Value", numToString( numdialer->getValue() ) ) );
            }
                break;

            case CI_UI_WIDGET_2DPAD:
            {
                ciUI2DPad *pad = (ciUI2DPad *) widget; 
                XML.push_back( XmlTree( "XValue", numToString( pad->getScaledValue().x ) ) ); 
                XML.push_back( XmlTree( "YValue", numToString( pad->getScaledValue().y ) ) );            
            }
                break;

            case CI_UI_WIDGET_TEXTINPUT:
            {
                ciUITextInput *textInput = (ciUITextInput *) widget; 
                XML.push_back( XmlTree( "Value", textInput->getTextString() ) );                 
            }
                break;
                                
            case CI_UI_WIDGET_ROTARYSLIDER:
            {
                ciUIRotarySlider *rotslider = (ciUIRotarySlider *) widget;
                XML.push_back( XmlTree( "Value", numToString( rotslider->getScaledValue() ) ) ); 
            }
                break;
                
            case CI_UI_WIDGET_IMAGESAMPLER:
            {
                ciUIImageSampler *imageSampler = (ciUIImageSampler *) widget;                 
                XML.push_back( XmlTree( "XValue", numToString( imageSampler->getValue().x ) ) ); 
                XML.push_back( XmlTree( "YValue", numToString( imageSampler->getValue().y ) ) );                
                XML.push_back( XmlTree( "RColor", numToString( imageSampler->getColor().r ) ) );                               
                XML.push_back( XmlTree( "GColor", numToString( imageSampler->getColor().g ) ) );                               
                XML.push_back( XmlTree( "BColor", numToString( imageSampler->getColor().b ) ) );                                               
                XML.push_back( XmlTree( "AColor", numToString( imageSampler->getColor().a ) ) );                               
            }
                break;

            default:
                break;
        }
    }

    void loadSettings(string fileName)
    {
        try
        {
            std::string filePath = "settings/"+fileName;
#if defined( CINDER_COCOA )
            filePath = ci::app::App::getResourcePath().string()+"/"+filePath;
#endif
            XmlTree XML( loadFile(filePath) );

            XmlTree settings = XML.getChild( "Settings" );        
            for( XmlTree::Iter item = settings.begin(); item != settings.end(); ++item )
            {        
                string wName = item->getChild("Name").getValue(); 
                ciUIWidget *widget = getWidget(wName); 
                if(widget != NULL)
                {
                    loadSpecificWidgetData(widget, *item); 
                    triggerEvent(widget); 
                }                     
            }
        }
        catch (Exception e)
        {
            cout << "CIUI: Could not load file: " << fileName << endl; 
        }
        hasKeyBoard = false;                
    }


    void loadSpecificWidgetData(ciUIWidget *widget, XmlTree &XML)
    {
        int kind = XML.getChild("Kind").getValue<int>();       
        switch (kind) 
        {
            case CI_UI_WIDGET_IMAGETOGGLE:    
            case CI_UI_WIDGET_MULTIIMAGETOGGLE: 
            case CI_UI_WIDGET_LABELTOGGLE:                
            case CI_UI_WIDGET_TOGGLE:
            {
                ciUIToggle *toggle = (ciUIToggle *) widget; 
                int value = XML.getChild("Value").getValue<int>();
                toggle->setValue((value ? 1 : 0)); 
            }
                break;

            case CI_UI_WIDGET_MULTIIMAGESLIDER_H:
            case CI_UI_WIDGET_MULTIIMAGESLIDER_V:                 
            case CI_UI_WIDGET_IMAGESLIDER_H:
            case CI_UI_WIDGET_IMAGESLIDER_V:                
            case CI_UI_WIDGET_BILABELSLIDER:    
            case CI_UI_WIDGET_CIRCLESLIDER:               
            case CI_UI_WIDGET_MINIMALSLIDER:
            case CI_UI_WIDGET_SLIDER_H:
            case CI_UI_WIDGET_SLIDER_V:
            {
                ciUISlider *slider = (ciUISlider *) widget; 
                float value = XML.getChild("Value").getValue<float>();
                slider->setValue(value); 
            }
                break;
                
            case CI_UI_WIDGET_RSLIDER_H:
            case CI_UI_WIDGET_RSLIDER_V:
            {
                ciUIRangeSlider *rslider = (ciUIRangeSlider *) widget; 
                float valueHigh = XML.getChild("HighValue").getValue<float>();
                float valueLow = XML.getChild("LowValue").getValue<float>();
                rslider->setValueHigh(valueHigh);
                rslider->setValueLow(valueLow);
            }
                break;
                
            case CI_UI_WIDGET_NUMBERDIALER:
            {
                ciUINumberDialer *numdialer = (ciUINumberDialer *) widget; 
                float value = XML.getChild("Value").getValue<float>();
                numdialer->setValue(value);                 
            }
                break;
                
            case CI_UI_WIDGET_2DPAD:
            {
                ciUI2DPad *pad = (ciUI2DPad *) widget; 
                float valueX = XML.getChild("XValue").getValue<float>();
                float valueY = XML.getChild("YValue").getValue<float>();
                pad->setValue(Vec2f(valueX, valueY)); 
            }
                break;
                
            case CI_UI_WIDGET_TEXTINPUT:
            {
                ciUITextInput *textInput = (ciUITextInput *) widget; 
                string value = XML.getChild("Value").getValue();
                textInput->setTextString(value); 
            }
                break;                
                
            case CI_UI_WIDGET_ROTARYSLIDER:
            {
                ciUIRotarySlider *rotslider = (ciUIRotarySlider *) widget;
                float value = XML.getChild("Value").getValue<float>();
                rotslider->setValue(value); 
            }
                break;
                
            case CI_UI_WIDGET_IMAGESAMPLER:
            {
                ciUIImageSampler *imageSampler = (ciUIImageSampler *) widget; 
                float valueX = XML.getChild("XValue").getValue<float>();
                float valueY = XML.getChild("YValue").getValue<float>();
                
                float r = XML.getChild("RColor").getValue<float>();
                float g = XML.getChild("GColor").getValue<float>();
                float b = XML.getChild("BColor").getValue<float>();
                float a = XML.getChild("AColor").getValue<float>();
                
                imageSampler->setValue(Vec2f(valueX, valueY));
                imageSampler->setColor(ColorA(r,g,b,a));
            }
                break;
                
            default:
                break;
        }        
    }

    
    gl::TextureFontRef getFontLarge()
    {
        return font_large;
    }    
    
    gl::TextureFontRef getFontMedium()
    {
        return font_medium;
    }

    gl::TextureFontRef getFontSmall()
    {
        return font_small;
    }

    bool setFont(string filename)
    {
        
        filename = "NewMedia";
        bool large = updateFont(CI_UI_FONT_LARGE, filename, CI_UI_FONT_LARGE_SIZE);
        bool medium = updateFont(CI_UI_FONT_MEDIUM, filename, CI_UI_FONT_MEDIUM_SIZE);
        bool small = updateFont(CI_UI_FONT_SMALL, filename, CI_UI_FONT_SMALL_SIZE);
        bool successful = large && medium && small;
        if( successful ) fontName = filename;
        return successful;
    }
    
    string getFontName()
    {
        return fontName; 
    }
    
    void setFontSize(ciUIWidgetFontType _kind, int _size, int _resolution = CI_UI_FONT_RESOLUTION)
    {
        switch(_kind)
        {
            case CI_UI_FONT_LARGE:                                              
                fontLarge = Font( fontName, _size);                
                font_large = gl::TextureFont::create(fontLarge);                
                break; 

            case CI_UI_FONT_MEDIUM:
                fontMedium = Font( fontName, _size);                
                font_medium = gl::TextureFont::create(fontMedium);                
                
                break; 

            case CI_UI_FONT_SMALL:
                fontSmall = Font( fontName, _size);
                font_small = gl::TextureFont::create(fontSmall);                
                
                break; 
        }
    }
    
    void setWidgetSpacing(float _widgetSpacing)
    {
        widgetSpacing = _widgetSpacing; 
    }
    
    float getWidgetSpacing()
    {
        return widgetSpacing;
    }
   
    bool isEnabled()
	{
		return enabled; 
	}
	
    void setVisible(bool _visible)
    {
        visible = _visible; 
        if(visible)
        {
            enable();
        }
        else
        {
            disable();
        }
    }
    
    
	void toggleVisible()
	{
		if(isEnabled())
		{
			disable(); 
		}
		else {
			enable(); 
		}
	}

    bool hasKeyboardFocus()
    {
        return hasKeyBoard; 
    }
	
	void enable()
	{
        if(!isEnabled())
        {            
            enabled = true; 
#if defined( CINDER_COCOA_TOUCH )
            enableTouchEventCallbacks();
#else
            enableMouseEventCallbacks();
            enableKeyEventCallbacks();            
#endif         
        }
	}
	
	void disable()
	{
        if(isEnabled())
        {                    
            enabled = false;       
#if defined( CINDER_COCOA_TOUCH )
            disableTouchEventCallbacks();
#else
            disableMouseEventCallbacks();    
            disableKeyEventCallbacks();
#endif
        }
    }
		
#if defined( CINDER_COCOA_TOUCH )
	
	//Touch Callbacks
    void enableTouchEventCallbacks()
    {
		mCbTouchesBegan = mApp->getWindow()->getSignalTouchesBegan().connect( std::bind( &ciUICanvas::canvasTouchesBegan, this, std::_1 ) );
		mCbTouchesMoved = mApp->getWindow()->getSignalTouchesMoved().connect( std::bind( &ciUICanvas::canvasTouchesMoved, this, std::_1 ) );
		mCbTouchesEnded = mApp->getWindow()->getSignalTouchesEnded().connect( std::bind( &ciUICanvas::canvasTouchesEnded, this, std::_1 ) );
    }	

	void disableTouchEventCallbacks()
    {
		mCbTouchesBegan.disconnect();
		mCbTouchesMoved.disconnect();
		mCbTouchesEnded.disconnect();
    }	
	
#else
	
	//Mouse Callbacks
    void enableMouseEventCallbacks()
    {
		mCbMouseDown = mApp->getWindow()->getSignalMouseDown().connect( std::bind( &ciUICanvas::canvasMouseDown, this, std::_1 ) );
		mCbMouseUp = mApp->getWindow()->getSignalMouseUp().connect( std::bind( &ciUICanvas::canvasMouseUp, this, std::_1 ) );
		mCbMouseMove = mApp->getWindow()->getSignalMouseMove().connect( std::bind( &ciUICanvas::canvasMouseMove, this, std::_1 ) );
		mCbMouseDrag = mApp->getWindow()->getSignalMouseDrag().connect( std::bind( &ciUICanvas::canvasMouseDrag, this, std::_1 ) );
    }

	//Mouse Callbacks
    void disableMouseEventCallbacks()
    {
		mCbMouseDown.disconnect();
		mCbMouseUp.disconnect();
		mCbMouseMove.disconnect();
		mCbMouseDrag.disconnect();
    }	

    //KeyBoard Callbacks
	void enableKeyEventCallbacks()
	{
		mCbKeyDown = mApp->getWindow()->getSignalKeyDown().connect( std::bind( &ciUICanvas::canvasKeyDown, this, std::_1 ) );
		mCbKeyUp = mApp->getWindow()->getSignalKeyUp().connect( std::bind( &ciUICanvas::canvasKeyUp, this, std::_1 ) );
	}

	//KeyBoard Callbacks
	void disableKeyEventCallbacks()
	{
		mCbKeyDown.disconnect();
		mCbKeyUp.disconnect();       
	}
    
#endif	    
    virtual void update()
    {		
        if(enabled)
        {           
            for(unsigned int i = 0; i < widgets.size(); i++)
            {
                widgets[i]->update(); 	
            }		
        }
    }
     
    void draw()
    {
        if(enabled)
        {                   
            glDisable(GL_DEPTH_TEST);       
            glDisable(GL_LIGHTING);
            gl::enableAlphaBlending(); 
            
            glLineWidth(1.5f);
            
            drawPadded();
            
            drawPaddedOutline(); 
            
            drawBack(); 
            
            drawFill(); 
            
            drawFillHighlight(); 
            
            drawOutline(); 
            
            drawOutlineHighlight();

            for(int i = widgets.size()-1; i >= 0; i--)
            {
                if(widgets[i]->isVisible())
                {
                    widgets[i]->draw(); 	
                }
            }
            
            glDisable(GL_DEPTH_TEST); 
        }
    }
    
    void exit()
    {

    }	
        
#if defined( CINDER_COCOA_TOUCH )

	virtual bool canvasTouchesBegan(TouchEvent event) 
	{		
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) 
        {                
            if(rect->inside(touchIt->getX(), touchIt->getY()))
            {
                for(int i = 0; i < widgets.size(); i++)
                {
                    if(widgets[i]->isVisible())	widgets[i]->touchesBegan(event); 
                }
            }
        }
        return false;         
	}
    
	virtual bool canvasTouchesMoved(TouchEvent event) 
	{        
        for(int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible())	widgets[i]->touchesMoved(event);
        }
        return false;         
    }
    
	virtual bool canvasTouchesEnded(TouchEvent event) 
	{
        for(int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible())	widgets[i]->touchesEnded(event); 
        }
        return false;         
	}
    	
#else	
    
	virtual bool canvasMouseMove( MouseEvent event ) 
    {
        if(rect->inside((float) event.getX(), (float) event.getY()))
        {
			for(unsigned int i = 0; i < widgets.size(); i++)
			{                
				if(widgets[i]->isVisible()) widgets[i]->mouseMove(event.getX(), event.getY()); 
			}
		}	
        return false; 
    }
    
    virtual bool canvasMouseDrag( MouseEvent event ) 
    {		
        for(unsigned int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible())	widgets[i]->mouseDrag(event.getX(), event.getY(), event.isRightDown()); 
        }     
        return false;         
    }
    
    virtual bool canvasMouseDown( MouseEvent event ) 
    {
        if(rect->inside((float) event.getX(), (float) event.getY()))
        {
			for(unsigned int i = 0; i < widgets.size(); i++)
			{
				if(widgets[i]->isVisible()) widgets[i]->mouseDown(event.getX(), event.getY(), event.isRightDown()); 
			}
		}		
        return false;         
    }
    
    virtual bool canvasMouseUp( MouseEvent event ) 
    {		        
        for(unsigned int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible()) widgets[i]->mouseUp(event.getX(), event.getY(), event.isRightDown()); 
        }    
       return false;  
    }
	    
#endif	

    virtual bool canvasKeyDown( KeyEvent event )
    {
        for(unsigned int i = 0; i < widgets.size(); i++)
		{
			widgets[i]->keyDown(event);
		}
        return false; 
    }

    virtual bool canvasKeyUp( KeyEvent event )
    {
		for(unsigned int i = 0; i < widgets.size(); i++)
		{
			widgets[i]->keyUp(event);
		}
        return false; 
    }
	
    bool isHit(int x, int y)
    {
        if(isEnabled())
        {
            return rect->inside((float) x, (float) y);
        }
        else
        {
            return false; 
        }
    }
    
    ciUIWidget *getWidgetHit(float x, float y)
    {
        if(isEnabled() && rect->inside(x, y))
        {
            for(unsigned int i = 0; i < widgets.size(); i++)
            {
                if(widgets[i]->isHit(x, y))
                {
                    return widgets[i]; 
                }
            }
            return NULL;
        }
        else
        {
            return NULL; 
        }        
    }
    

    void stateChange()
    {        
        switch (state) 
		{
            case CI_UI_STATE_NORMAL:
            {            
                draw_fill_highlight = false;             
                draw_outline_highlight = false;             
            }
                break;
            case CI_UI_STATE_OVER:
            {
                draw_fill_highlight = false;        
				if(enable_highlight_outline)
				{
					draw_outline_highlight = true;                         
				}
            }
                break;
            case CI_UI_STATE_DOWN:
            {
				if(enable_highlight_fill)
				{
					draw_fill_highlight = true;     				
				}
                draw_outline_highlight = false;             
            }
                break;
            case CI_UI_STATE_SUSTAINED:
            {
                draw_fill_highlight = false;            
                draw_outline_highlight = false;                         
            }
                break;            
                
            default:
                break;
        }        
    }
    
    void autoSizeToFitWidgets()
    {        
        float maxWidth = 0;
        float maxHeight = 0;

        for(unsigned int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible())
            {
                ciUIRectangle* wr = widgets[i]->getRect(); 
                float widgetwidth = wr->getRawX()+wr->getWidth();

                float widgetheight = wr->getRawY()+wr->getHeight();
                
                if(widgetwidth > maxWidth)
                {
                    maxWidth = wr->getRawX()+widgets[i]->getPaddingRect()->getWidth();
                }                        
                if(widgetheight > maxHeight)
                {
                    maxHeight = wr->getRawY()+widgets[i]->getPaddingRect()->getHeight();                                                                        
                }        
            }
        }
        
        rect->setWidth(maxWidth);
        rect->setHeight(maxHeight);
        paddedRect->setWidth(rect->getWidth()+padding*2.0f);
        paddedRect->setHeight(rect->getHeight()+padding*2.0f);        
    }
    
    void centerWidgetsOnCanvas(bool centerHorizontally=true, bool centerVertically=true)
    {            
        float xMin = 0; 
        float yMin = 0;
        
        float xMax = 0; 
        float yMax = 0;
        
        float w = 0; 
        float h = 0; 
        
        for(unsigned int i = 0; i < widgets.size(); i++)
        {
//            if(widgets[i]->isVisible())
//            {
                ciUIRectangle* wr = widgets[i]->getRect(); 
                if(wr->getRawX() < xMin)
                {
                    xMin = wr->getRawX(); 
                }
                if((wr->getRawX() + wr->getWidth()) > xMax)
                {
                    xMax = (wr->getRawX() + wr->getWidth()); 
                }
                
                if(wr->getRawY() < yMin)
                {
                    yMin = wr->getRawY(); 
                }
                if((wr->getRawY() + wr->getHeight()) > yMax)
                {
                    yMax = (wr->getRawY() + wr->getHeight()); 
                }                                                                    
//            }
        }     
        
        w = xMax - xMin;
        h = yMax - yMin;            
        
        float moveDeltaX = rect->getHalfWidth() - w*.5f; 
        float moveDeltaY = rect->getHalfHeight() - h*.5f;
                
        for(unsigned int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible() && !(widgets[i]->isEmbedded()))
            {
                ciUIRectangle* wr = widgets[i]->getRect(); 
                if(centerHorizontally) wr->setX(wr->getRawX()+ moveDeltaX-padding);             
                if(centerVertically) wr->setY(wr->getRawY()+ moveDeltaY-padding);                             
            }
        }           
//        addWidget(new ciUISpacer(xMin+moveDeltaX, yMin+moveDeltaY, w, h));
    }    
    
    void centerWidgetsHorizontallyOnCanvas()
    {
        centerWidgetsOnCanvas(true, false);
    }

    void centerWidgetsVerticallyOnCanvas()
    {
        centerWidgetsOnCanvas(false, true);
    }
    
    void centerWidgets()
    {
        centerWidgetsOnCanvas(); 
    }
    
    void removeWidget(ciUIWidget *widget)
    {
//        cout << endl; 
//        cout << "Widget to find: " << widget->getName() << endl; 
//        cout << endl; 
        
        //for the map
        map<string, ciUIWidget*>::iterator it;        
        it=widgets_map.find(widget->getName());
        if(it != widgets_map.end())
        {
            widgets_map.erase(it);     
        }
                
        for(unsigned int i = 0; i < widgetsWithState.size(); i++)
        {
            ciUIWidget *other = widgetsWithState[i]; 
            if(widget->getName() == other->getName())
            {
                widgetsWithState.erase(widgetsWithState.begin()+i);                
                break; 
            }
        }
        vector<ciUIWidget *>::iterator wit;
        //for all the widgets 
        for(wit=widgets.begin(); wit != widgets.end(); wit++)
        {
            ciUIWidget *other = *wit;                   
            if(widget->getName() == other->getName())
            { 
                widgets.erase(wit);                             
                break; 
            }
        }
        
        if(widget->hasLabel())
        {
            ciUIWidgetWithLabel *wwl = (ciUIWidgetWithLabel *) widget; 
            ciUILabel *label = wwl->getLabelWidget();
            removeWidget(label);
        }        

        delete widget;
    }    

    void addWidget(ciUIWidget *widget)
	{
        if(widget->getKind() == CI_UI_WIDGET_LABEL)
		{
			ciUILabel *label = (ciUILabel *) widget;
			setLabelFont(label); 
		}
		else if(widget->getKind() == CI_UI_WIDGET_SLIDER_H || widget->getKind() == CI_UI_WIDGET_SLIDER_V || widget->getKind() == CI_UI_WIDGET_BILABELSLIDER || widget->getKind() == CI_UI_WIDGET_MINIMALSLIDER || widget->getKind() == CI_UI_WIDGET_CIRCLESLIDER || widget->getKind() == CI_UI_WIDGET_IMAGESLIDER_H || widget->getKind() == CI_UI_WIDGET_IMAGESLIDER_V || widget->getKind() == CI_UI_WIDGET_MULTIIMAGESLIDER_H || widget->getKind() == CI_UI_WIDGET_MULTIIMAGESLIDER_V)           
		{
			ciUISlider *slider = (ciUISlider *) widget;
			ciUILabel *label = (ciUILabel *) slider->getLabel();
			setLabelFont(label); 			
			pushbackWidget(label); 				

            if(widget->getKind() == CI_UI_WIDGET_BILABELSLIDER)
            {
                ciUIBiLabelSlider *biSlider = (ciUIBiLabelSlider *) widget;
                ciUILabel *rlabel = (ciUILabel *) biSlider->getRightLabel();
                setLabelFont(rlabel); 			
                pushbackWidget(rlabel); 				                
            }
            
            widgetsWithState.push_back(widget);                         
		}
		else if(widget->getKind() == CI_UI_WIDGET_2DPAD)		
		{
			ciUI2DPad *pad = (ciUI2DPad *) widget;
			ciUILabel *label = (ciUILabel *) pad->getLabel();
			setLabelFont(label); 			
			pushbackWidget(label); 				

            widgetsWithState.push_back(widget);             
		}		
		else if(widget->getKind() == CI_UI_WIDGET_IMAGE)		
		{
			ciUIImage *image = (ciUIImage *) widget;
			ciUILabel *label = (ciUILabel *) image->getLabel();
			setLabelFont(label); 			
			pushbackWidget(label); 				
		}	
		else if(widget->getKind() == CI_UI_WIDGET_IMAGESAMPLER)		
		{
			ciUIImage *image = (ciUIImage *) widget;
			ciUILabel *label = (ciUILabel *) image->getLabel();
			setLabelFont(label); 			
			pushbackWidget(label); 				
            
            widgetsWithState.push_back(widget);                                     
		}	        
		else if(widget->getKind() == CI_UI_WIDGET_RSLIDER_H || widget->getKind() == CI_UI_WIDGET_RSLIDER_V)
		{
			ciUIRangeSlider *rslider = (ciUIRangeSlider *) widget;
			ciUILabel *label = (ciUILabel *) rslider->getLabel();
			setLabelFont(label); 			
			pushbackWidget(label); 			
            
            widgetsWithState.push_back(widget);                         
		}		
		else if(widget->getKind() == CI_UI_WIDGET_ROTARYSLIDER)
		{
			ciUIRotarySlider *rslider = (ciUIRotarySlider *) widget;
			ciUILabel *label = (ciUILabel *) rslider->getLabel();
			setLabelFont(label); 			
			pushbackWidget(label); 				
            
            widgetsWithState.push_back(widget);                         
		}		
		else if(widget->getKind() == CI_UI_WIDGET_BUTTON || widget->getKind() == CI_UI_WIDGET_TOGGLE || widget->getKind() ==  CI_UI_WIDGET_LABELBUTTON || widget->getKind() == CI_UI_WIDGET_LABELTOGGLE || widget->getKind() == CI_UI_WIDGET_MULTIIMAGEBUTTON || widget->getKind() == CI_UI_WIDGET_MULTIIMAGETOGGLE)
		{
			ciUIButton *button = (ciUIButton *) widget;
			ciUILabel *label = (ciUILabel *) button->getLabel();
			setLabelFont(label); 			
			pushbackWidget(label); 		
            if(widget->getKind() != CI_UI_WIDGET_BUTTON && widget->getKind() != CI_UI_WIDGET_LABELBUTTON && widget->getKind() != CI_UI_WIDGET_MULTIIMAGEBUTTON)
            {
                widgetsWithState.push_back(widget);                         
            }
		}
        else if(widget->getKind() == CI_UI_WIDGET_DROPDOWNLIST)            
        { 
			ciUIDropDownList *list = (ciUIDropDownList *) widget;
			ciUILabel *label = (ciUILabel *) list->getLabel();
			setLabelFont(label); 			
			pushbackWidget(label); 		
            
            vector<ciUILabelToggle *> toggles = list->getToggles(); 
			for(unsigned int i = 0; i < toggles.size(); i++)
			{
				ciUILabelToggle *t = toggles[i]; 
				ciUILabel *l2 = (ciUILabel *) t->getLabel();
				setLabelFont(l2); 	
                pushbackWidget(l2); 					
                pushbackWidget(t); 

                widgetsWithState.push_back(t);             
			}            
        }
		else if(widget->getKind() == CI_UI_WIDGET_TEXTINPUT)
		{
			ciUITextInput *textinput = (ciUITextInput *) widget;
			ciUILabel *label = (ciUILabel *) textinput->getLabel();
			setLabelFont(label); 			
			pushbackWidget(label); 	
            
            widgetsWithState.push_back(widget);             
		}		
		else if(widget->getKind() == CI_UI_WIDGET_NUMBERDIALER)
		{
			ciUINumberDialer *numberDialer = (ciUINumberDialer *) widget;
			ciUILabel *label = (ciUILabel *) numberDialer->getLabel();
			setLabelFont(label); 			
			pushbackWidget(label); 			
            
            widgetsWithState.push_back(widget);                         
		}		        
		else if(widget->getKind() == CI_UI_WIDGET_RADIO)
		{
			ciUIRadio *radio = (ciUIRadio *) widget;
			ciUILabel *label = (ciUILabel *) radio->getLabel();			
			setLabelFont(label); 
			pushbackWidget(label); 				
			
			vector<ciUIToggle *> toggles = radio->getToggles(); 
			
			for(unsigned int i = 0; i < toggles.size(); i++)
			{
				ciUIToggle *t = toggles[i]; 
				ciUILabel *l2 = (ciUILabel *) t->getLabel();
				setLabelFont(l2); 	
				pushbackWidget(t); 
				pushbackWidget(l2);
                
                widgetsWithState.push_back(t);                             
			}
		}
        else if(widget->getKind() == CI_UI_WIDGET_TOGGLEMATRIX)
		{
			ciUIToggleMatrix *matrix = (ciUIToggleMatrix *) widget;
			ciUILabel *label = (ciUILabel *) matrix->getLabel();			
			setLabelFont(label); 
			pushbackWidget(label); 				
			
			vector<ciUIToggle *> toggles = matrix->getToggles(); 
			
			for(unsigned int i = 0; i < toggles.size(); i++)
			{
				ciUIToggle *t = toggles[i]; 
				ciUILabel *l2 = (ciUILabel *) t->getLabel();
				setLabelFont(l2); 	
				pushbackWidget(t); 
				pushbackWidget(l2); 			
                
                widgetsWithState.push_back(t);                             
			}
		}
		else if(widget->getKind() == CI_UI_WIDGET_FPS)
		{
			ciUILabel *fps = (ciUILabel *) widget;
			setLabelFont(fps); 
		}
        else if(widget->getKind() == CI_UI_WIDGET_IMAGETOGGLE)
        {
            widgetsWithState.push_back(widget);              
        }
	
		widget->setRectParent(this->rect); 		
		widget->setParent(this); 
		pushbackWidget(widget); 	
	}
    
	ciUIWidget* addWidgetDown(ciUIWidget *widget, ciUIWidgetAlignment align = CI_UI_ALIGN_LEFT, bool reAdd = false)
	{
        if(!reAdd) addWidget(widget); 
        ciUIRectangle *widgetRect = widget->getRect();         
		if(lastAdded != NULL)
		{
			ciUIRectangle *lastPaddedRect = lastAdded->getPaddingRect(); 
			widgetRect->setY(lastPaddedRect->getY()+lastPaddedRect->getHeight()-rect->getY()+widgetSpacing); 
		}
		else 
		{
			widgetRect->setY(widgetSpacing);
		}
        switch(align)
        {
            case CI_UI_ALIGN_TOP: 
                
                break; 
            case CI_UI_ALIGN_BOTTOM:
                
                break;
                
            case CI_UI_ALIGN_LEFT: 
                widgetRect->setX(widgetSpacing);             
                break;                    
            case CI_UI_ALIGN_FREE: 
                
                break; 
            case CI_UI_ALIGN_RIGHT: 
                widgetRect->setX(rect->getWidth()-widgetRect->getWidth()-widgetSpacing); 
                break;                     
        }                           
		lastAdded = widget; 
		return widget;
	}
    
	ciUIWidget* addWidgetUp(ciUIWidget *widget, ciUIWidgetAlignment align = CI_UI_ALIGN_LEFT, bool reAdd = false)
	{
        if(!reAdd) addWidget(widget); 
        ciUIRectangle *widgetRect = widget->getRect();                 
		if(lastAdded != NULL)
		{
			ciUIRectangle *lastPaddedRect = lastAdded->getPaddingRect(); 
            ciUIRectangle *widgetPaddedRect = widget->getPaddingRect();                                     
			widgetRect->setY(lastPaddedRect->getY()-widgetPaddedRect->getHeight()-rect->getY());                                     
		}
		else 
		{
			widgetRect->setY(widgetSpacing);             
		}
        switch(align)
        {
            case CI_UI_ALIGN_TOP: 
                
                break; 
            case CI_UI_ALIGN_BOTTOM:
                
                break;
                
            case CI_UI_ALIGN_LEFT: 
                widgetRect->setX(widgetSpacing);             
                break;                    
            case CI_UI_ALIGN_FREE: 
                
                break; 
            case CI_UI_ALIGN_RIGHT: 
                widgetRect->setX(rect->getWidth()-widgetRect->getWidth()-widgetSpacing); 
                break;                     
        }          
		lastAdded = widget; 
		return widget;
	}    
    
	ciUIWidget* addWidgetRight(ciUIWidget *widget, ciUIWidgetAlignment align = CI_UI_ALIGN_FREE, bool reAdd = false)
	{
        if(!reAdd) addWidget(widget); 
        ciUIRectangle *widgetRect = widget->getRect();                         
		if(lastAdded != NULL)
		{
			ciUIRectangle *lastRect = lastAdded->getRect(); 
			ciUIRectangle *lastPaddedRect = lastAdded->getPaddingRect();                         
			
            widgetRect->setX(lastPaddedRect->getX()+lastPaddedRect->getWidth()-rect->getX()+widgetSpacing); 
			widgetRect->setY(lastRect->getY()-rect->getY()); 
		}
		else 
		{
			widgetRect->setX(widgetSpacing); 
            widgetRect->setY(widgetSpacing);                         
		}
        switch(align)
        {
            case CI_UI_ALIGN_LEFT: 
                widgetRect->setX(widgetSpacing);             
                break;                    
            case CI_UI_ALIGN_FREE: 
                
                break; 
            case CI_UI_ALIGN_RIGHT: 
                widgetRect->setX(rect->getWidth()-widgetRect->getWidth()-widgetSpacing); 
                break;                     
            case CI_UI_ALIGN_TOP: 
                widgetRect->setY(widgetSpacing);                         
                break;                     
            case CI_UI_ALIGN_BOTTOM: 
                widgetRect->setY(rect->getHeight()-widgetRect->getHeight()-widgetSpacing); 
                break;                                     
        }                  
        lastAdded = widget; 
        return widget; 	
	}
    
    ciUIWidget* addWidgetLeft(ciUIWidget *widget, ciUIWidgetAlignment align = CI_UI_ALIGN_FREE, bool reAdd = false)
	{
        if(!reAdd) addWidget(widget);     
        ciUIRectangle *widgetRect = widget->getRect(); 
		if(lastAdded != NULL)
		{
			ciUIRectangle *lastRect = lastAdded->getRect(); 
			ciUIRectangle *lastPaddedRect = lastAdded->getPaddingRect();                         
            ciUIRectangle *widgetPaddedRect = widget->getPaddingRect();                         
			
            widgetRect->setX(lastPaddedRect->getX()-widgetPaddedRect->getWidth()-rect->getX()); 
			widgetRect->setY(lastRect->getY()-rect->getY()); 
		}
		else 
		{
			widgetRect->setX(widgetSpacing); 
            widgetRect->setY(widgetSpacing);                         
		}
        switch(align)
        {
            case CI_UI_ALIGN_LEFT: 
                widgetRect->setX(widgetSpacing);             
                break;                    
            case CI_UI_ALIGN_FREE: 
                
                break; 
            case CI_UI_ALIGN_RIGHT: 
                widgetRect->setX(rect->getWidth()-widgetRect->getWidth()-widgetSpacing); 
                break;        
            case CI_UI_ALIGN_TOP: 
                widgetRect->setY(widgetSpacing);                         
                break;                     
            case CI_UI_ALIGN_BOTTOM: 
                widgetRect->setY(rect->getHeight()-widgetRect->getHeight()-widgetSpacing); 
                break;                                                     
        }                          
        lastAdded = widget;  	
        return widget;
	}    
    
    ciUIWidget* addWidgetSouthOf(ciUIWidget *widget, string referenceName, bool reAdd = false)
	{
        if(!reAdd) addWidget(widget); 
        ciUIWidget *referenceWidget = getWidget(referenceName);
		if(referenceWidget != NULL)
		{
			ciUIRectangle *referencePaddedRect = referenceWidget->getPaddingRect(); 
			ciUIRectangle *referenceRect = referenceWidget->getRect();             
			ciUIRectangle *widgetRect = widget->getRect(); 
            
			widgetRect->setY(referencePaddedRect->getY()+referencePaddedRect->getHeight()-rect->getY()+widgetSpacing); 
            widgetRect->setX(referenceRect->getX()-rect->getX());   
		}
		else 
		{
			ciUIRectangle *widgetRect = widget->getRect(); 
			widgetRect->setY(widgetSpacing); 
		}
        lastAdded = widget;  		
        return widget;
    }    
    
    ciUIWidget* addWidgetNorthOf(ciUIWidget *widget, string referenceName, bool reAdd = false)
	{
        if(!reAdd) addWidget(widget); 
        ciUIWidget *referenceWidget = getWidget(referenceName);
		if(referenceWidget != NULL)
		{
			ciUIRectangle *referencePaddedRect = referenceWidget->getPaddingRect(); 
			ciUIRectangle *referenceRect = referenceWidget->getRect();             
			ciUIRectangle *widgetRect = widget->getRect(); 
            ciUIRectangle *widgetPaddedRect = widget->getPaddingRect();             
            
			widgetRect->setY(referencePaddedRect->getY()-widgetPaddedRect->getHeight()-rect->getY());             
            widgetRect->setX(referenceRect->getX()-rect->getX());   
		}
		else 
		{
			ciUIRectangle *widgetRect = widget->getRect(); 
			widgetRect->setY(widgetSpacing); 
		}
        lastAdded = widget;  	        
        return widget;
    }      
    
    ciUIWidget* addWidgetWestOf(ciUIWidget *widget, string referenceName, bool reAdd = false)
	{
        if(!reAdd) addWidget(widget);      
        ciUIWidget *referenceWidget = getWidget(referenceName);
		if(referenceWidget != NULL)
		{
			ciUIRectangle *referencePaddedRect = referenceWidget->getPaddingRect(); 
			ciUIRectangle *referenceRect = referenceWidget->getRect();             			
            ciUIRectangle *widgetRect = widget->getRect(); 
            ciUIRectangle *widgetPaddedRect = widget->getPaddingRect();             
            
            widgetRect->setY(referenceRect->getY()-rect->getY());                         
            widgetRect->setX(referencePaddedRect->getX()-rect->getX()-widgetPaddedRect->getWidth());   
		}
		else 
		{			
            ciUIRectangle *currentRect = widget->getRect(); 			
            currentRect->setY(widgetSpacing); 
		}
        lastAdded = widget;  	
        return widget;
    }        
    
    ciUIWidget* addWidgetEastOf(ciUIWidget *widget, string referenceName, bool reAdd = false)
	{
        if(!reAdd) addWidget(widget);     
        ciUIWidget *referenceWidget = getWidget(referenceName);
		if(referenceWidget != NULL)
		{
			ciUIRectangle *referencePaddedRect = referenceWidget->getPaddingRect(); 
			ciUIRectangle *referenceRect = referenceWidget->getRect();             			
            ciUIRectangle *widgetRect = widget->getRect(); 	            
            widgetRect->setY(referenceRect->getY()-rect->getY());                        
            widgetRect->setX(referencePaddedRect->getX()-rect->getX()+referencePaddedRect->getWidth()+widgetSpacing); 
		}
		else 
		{			
            ciUIRectangle *currentRect = widget->getRect(); 			
            currentRect->setY(widgetSpacing); 
		}
        lastAdded = widget;  	
        return widget;
    }         
    
    void resetPlacer()
    {
        lastAdded = NULL; 
    }
    
    void setPlacer(ciUIWidget *referenceWidget)
    {
        lastAdded = referenceWidget; 
    }
    
	void setLabelFont(ciUILabel *label)
	{
		switch(label->getSize())
		{
			case CI_UI_FONT_LARGE:
				label->setFont(font_large); 					
				break; 
			case CI_UI_FONT_MEDIUM:
				label->setFont(font_medium); 					
				break; 
			case CI_UI_FONT_SMALL:
				label->setFont(font_small); 					
				break; 					
		}		
	}
	
	void triggerEvent(ciUIWidget *child)
	{        
        checkForKeyFocus(child); 
		GUIevent->widget = child; 		
        uiEventCallbackMgr.call( GUIevent ); 
	}
    
    void setUIColors(ColorA &cb, ColorA &co, ColorA &coh, ColorA &cf, ColorA &cfh, ColorA &cp, ColorA &cpo)
    {
        setWidgetColor(CI_UI_WIDGET_COLOR_BACK, cb);
        setWidgetColor(CI_UI_WIDGET_COLOR_OUTLINE, co);                
        setWidgetColor(CI_UI_WIDGET_COLOR_OUTLINE_HIGHLIGHT, coh);
        setWidgetColor(CI_UI_WIDGET_COLOR_FILL, cf);                
        setWidgetColor(CI_UI_WIDGET_COLOR_FILL_HIGHLIGHT, cfh);
        setWidgetColor(CI_UI_WIDGET_COLOR_PADDED, cp);
        setWidgetColor(CI_UI_WIDGET_COLOR_PADDED_OUTLINE, cpo);
        
        setColorBack(cb);
        setColorOutline(co);
        setColorOutlineHighlight(coh);
        setColorFill(cf);
        setColorFillHighlight(cfh);
        setColorPadded(cp);
        setColorPaddedOutline(cpo); 
    }
	    
    void setTheme(int theme)
    {
        switch(theme)
        {
            case CI_UI_THEME_DEFAULT:
            {
                ColorA cb = CI_UI_COLOR_BACK; 
                ColorA co = CI_UI_COLOR_OUTLINE; 
                ColorA coh = CI_UI_COLOR_OUTLINE_HIGHLIGHT;
                ColorA cf = CI_UI_COLOR_FILL; 
                ColorA cfh = CI_UI_COLOR_FILL_HIGHLIGHT;
                ColorA cp = CI_UI_COLOR_PADDED;
                ColorA cpo = CI_UI_COLOR_PADDED_OUTLINE;
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_HACKER:
            {
                ColorA cb = ColorA( 0.294118f, 0, 0.0588235f, 0.196078f );
                ColorA co = ColorA( 0.254902f, 0.239216f, 0.239216f, 0.392157f );
                ColorA coh = ColorA( 0.294118f, 0, 0.0588235f, 0.784314f );
                ColorA cf = ColorA( 0.784314f, 1, 0, 0.784314f );
                ColorA cfh = ColorA( 0.980392f, 0.00784314f, 0.235294f, 1 );
                ColorA cp = ColorA( 0.0156863f, 0, 0.0156863f, 0.392157f );
                ColorA cpo = ColorA( 0.254902f, 0.239216f, 0.239216f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

                
        case CI_UI_THEME_HIPSTER:
            {
                ColorA cb = ColorA( 0.607843f, 0.6f, 0.509804f, 0.196078f );
                ColorA co = ColorA( 0.231373f, 0.392157f, 0.501961f, 0.392157f );
                ColorA coh = ColorA( 0.607843f, 0.6f, 0.509804f, 0.784314f );
                ColorA cf = ColorA( 1, 0.52549f, 0.0666667f, 0.784314f );
                ColorA cfh = ColorA( 0.0313725f, 0.101961f, 0.188235f, 1 );
                ColorA cp = ColorA( 0.196078f, 0.25098f, 0.352941f, 0.392157f );
                ColorA cpo = ColorA( 0.231373f, 0.392157f, 0.501961f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_DIETER:
            {
                ColorA cb = ColorA( 0.803922f, 0.741176f, 0.682353f, 0.196078f );
                ColorA co = ColorA( 0.478431f, 0.356863f, 0.243137f, 0.392157f );
                ColorA coh = ColorA( 0.803922f, 0.741176f, 0.682353f, 0.784314f );
                ColorA cf = ColorA( 0.980392f, 0.294118f, 0, 0.784314f );
                ColorA cfh = ColorA( 0.980392f, 0.980392f, 0.980392f, 1 );
                ColorA cp = ColorA( 0.121569f, 0.121569f, 0.121569f, 0.392157f );
                ColorA cpo = ColorA( 0.478431f, 0.356863f, 0.243137f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_BARBIE:
            {
                ColorA cb = ColorA( 0.92549f, 0, 0.54902f, 0.196078f );
                ColorA co = ColorA( 0, 0, 0, 0.392157f );
                ColorA coh = ColorA( 0, 0.678431f, 0.937255f, 0.784314f );
                ColorA cf = ColorA( 0.92549f, 0, 0.54902f, 0.784314f );
                ColorA cfh = ColorA( 1, 0.94902f, 0, 1 );
                ColorA cp = ColorA( 0, 0, 0, 0.392157f );
                ColorA cpo = ColorA( 0, 0.678431f, 0.937255f, 0.784314f ); 
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_WINDOWS:
            {
                ColorA cb = ColorA( 0.0470588f, 0.0588235f, 0.4f, 0.196078f );
                ColorA co = ColorA( 0.0431373f, 0.0627451f, 0.54902f, 0.392157f );
                ColorA coh = ColorA( 0.0470588f, 0.0588235f, 0.4f, 0.784314f );
                ColorA cf = ColorA( 0.054902f, 0.305882f, 0.678431f, 0.784314f );
                ColorA cfh = ColorA( 0.0627451f, 0.498039f, 0.788235f, 1 );
                ColorA cp = ColorA( 0.027451f, 0.0352941f, 0.239216f, 0.392157f );
                ColorA cpo = ColorA( 0.0431373f, 0.0627451f, 0.54902f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_MACOSX:
            {
                ColorA cb = ColorA( 0, 0.678431f, 0.937255f, 0.196078f );
                ColorA co = ColorA( 1, 0.94902f, 0, 0.392157f );
                ColorA coh = ColorA( 0, 0, 0, 0.784314f );
                ColorA cf = ColorA( 0, 0.678431f, 0.937255f, 0.784314f );
                ColorA cfh = ColorA( 0.92549f, 0, 0.54902f, 1 );
                ColorA cp = ColorA( 1, 0.94902f, 0, 0.392157f );
                ColorA cpo = ColorA( 0, 0, 0, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_ZOOLANDER:
            {
                ColorA cb = ColorA( 0.160784f, 0.133333f, 0.121569f, 0.196078f );
                ColorA co = ColorA( 0.0745098f, 0.454902f, 0.490196f, 0.392157f );
                ColorA coh = ColorA( 0.160784f, 0.133333f, 0.121569f, 0.784314f );
                ColorA cf = ColorA( 0.988235f, 0.207843f, 0.298039f, 0.784314f );
                ColorA cfh = ColorA( 0.988235f, 0.968627f, 0.772549f, 1 );
                ColorA cp = ColorA( 0.0392157f, 0.74902f, 0.737255f, 0.392157f );
                ColorA cpo = ColorA( 0.0745098f, 0.454902f, 0.490196f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_VEGAN2:
            {
                ColorA cb = ColorA( 0.745098f, 0.94902f, 0.00784314f, 0.196078f );
                ColorA co = ColorA( 0.533333f, 0.768627f, 0.145098f, 0.392157f );
                ColorA coh = ColorA( 0.745098f, 0.94902f, 0.00784314f, 0.784314f );
                ColorA cf = ColorA( 0.917647f, 0.992157f, 0.901961f, 0.784314f );
                ColorA cfh = ColorA( 0.105882f, 0.403922f, 0.419608f, 1 );
                ColorA cp = ColorA( 0.317647f, 0.584314f, 0.282353f, 0.392157f );
                ColorA cpo = ColorA( 0.533333f, 0.768627f, 0.145098f, 0.784314f );                
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_BERLIN:
            {
                ColorA cb = ColorA( 0.6f, 0.894118f, 1, 0.196078f );
                ColorA co = ColorA( 0.294118f, 0.34902f, 0.419608f, 0.392157f );
                ColorA coh = ColorA( 0.6f, 0.894118f, 1, 0.784314f );
                ColorA cf = ColorA( 0.968627f, 0.309804f, 0.309804f, 0.784314f );
                ColorA cfh = ColorA( 1, 0.231373f, 0.231373f, 1 );
                ColorA cp = ColorA( 0.105882f, 0.12549f, 0.14902f, 0.392157f );
                ColorA cpo = ColorA( 0.294118f, 0.34902f, 0.419608f, 0.784314f ); 
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            case CI_UI_THEME_METALGEAR:
            {
                ColorA cb = ColorA( 0.2f, 0.172549f, 0.172549f, 0.294118f );
                ColorA co = ColorA( 0.0980392f, 0.101961f, 0.141176f, 0.392157f );
                ColorA coh = ColorA( 0.2f, 0.172549f, 0.172549f, 0.784314f );
                ColorA cf = ColorA( 0.980392f, 0.396078f, 0.341176f, 0.784314f );
                ColorA cfh = ColorA( 1, 1, 1, 1 );
                ColorA cp = ColorA( 0, 0, 0, 0.392157f );
                ColorA cpo = ColorA( 0.0980392f, 0.101961f, 0.141176f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            case CI_UI_THEME_TEALLIME:
            {
                ColorA cb = ColorA( 0.105882f, 0.403922f, 0.419608f, 0.294118f );
                ColorA co = ColorA( 0.917647f, 0.992157f, 0.901961f, 0.392157f );
                ColorA coh = ColorA( 0.105882f, 0.403922f, 0.419608f, 0.784314f );
                ColorA cf = ColorA( 0.317647f, 0.584314f, 0.282353f, 0.784314f );
                ColorA cfh = ColorA( 0.533333f, 0.768627f, 0.145098f, 1 );
                ColorA cp = ColorA( 0.745098f, 0.94902f, 0.00784314f, 0.392157f );
                ColorA cpo = ColorA( 0.917647f, 0.992157f, 0.901961f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_VEGAN:
            {
                ColorA cb = ColorA( 0.317647f, 0.584314f, 0.282353f, 0.294118f );
                ColorA co = ColorA( 0.105882f, 0.403922f, 0.419608f, 0.392157f );
                ColorA coh = ColorA( 0.317647f, 0.584314f, 0.282353f, 0.784314f );
                ColorA cf = ColorA( 0.533333f, 0.768627f, 0.145098f, 0.784314f );
                ColorA cfh = ColorA( 0.745098f, 0.94902f, 0.00784314f, 1 );
                ColorA cp = ColorA( 0.917647f, 0.992157f, 0.901961f, 0.392157f );
                ColorA cpo = ColorA( 0.105882f, 0.403922f, 0.419608f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_RUSTIC:
            {
                ColorA cb = ColorA( 0.768627f, 0.713725f, 0.427451f, 0.294118f );
                ColorA co = ColorA( 0.968627f, 0.427451f, 0.235294f, 0.392157f );
                ColorA coh = ColorA( 0.768627f, 0.713725f, 0.427451f, 0.784314f );
                ColorA cf = ColorA( 0.835294f, 0.152941f, 0.0196078f, 0.784314f );
                ColorA cfh = ColorA( 0.941176f, 0.827451f, 0.466667f, 1 );
                ColorA cp = ColorA( 0.952941f, 0.909804f, 0.894118f, 0.392157f );
                ColorA cpo = ColorA( 0.968627f, 0.427451f, 0.235294f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                            
            case CI_UI_THEME_MIDNIGHT:
            {
                ColorA cb = ColorA( 0.0431373f, 0.282353f, 0.419608f, 0.294118f );
                ColorA co = ColorA( 0.811765f, 0.941176f, 0.619608f, 0.392157f );
                ColorA coh = ColorA( 0.0431373f, 0.282353f, 0.419608f, 0.784314f );
                ColorA cf = ColorA( 0.231373f, 0.52549f, 0.52549f, 0.784314f );
                ColorA cfh = ColorA( 0.47451f, 0.741176f, 0.603922f, 1 );
                ColorA cp = ColorA( 0.658824f, 0.858824f, 0.658824f, 0.392157f );
                ColorA cpo = ColorA( 0.811765f, 0.941176f, 0.619608f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_MINBLUE:
            {
                ColorA cb = ColorA( 0.996078f, 0.976471f, 0.941176f, 0.294118f );
                ColorA co = ColorA( 0.690196f, 0.972549f, 1, 0.392157f );
                ColorA coh = ColorA( 0.996078f, 0.976471f, 0.941176f, 0.784314f );
                ColorA cf = ColorA( 0, 0.737255f, 0.819608f, 0.784314f );
                ColorA cfh = ColorA( 0.462745f, 0.827451f, 0.870588f, 1 );
                ColorA cp = ColorA( 0.682353f, 0.909804f, 0.984314f, 0.392157f );
                ColorA cpo = ColorA( 0.690196f, 0.972549f, 1, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                            
            case CI_UI_THEME_LIMESTONE:
            {
                ColorA cb = ColorA( 0.423529f, 0.564706f, 0.52549f, 0.294118f );
                ColorA co = ColorA( 0.988235f, 0.329412f, 0.388235f, 0.392157f );
                ColorA coh = ColorA( 0.423529f, 0.564706f, 0.52549f, 0.784314f );
                ColorA cf = ColorA( 0.662745f, 0.8f, 0.0941176f, 0.784314f );
                ColorA cfh = ColorA( 0.811765f, 0.286275f, 0.423529f, 1 );
                ColorA cp = ColorA( 0.921569f, 0.917647f, 0.737255f, 0.392157f );
                ColorA cpo = ColorA( 0.988235f, 0.329412f, 0.388235f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;                  
                
            case CI_UI_THEME_SPEARMINT:
            {
                ColorA cb = ColorA( 0.0980392f, 0.54902f, 0.0352941f, 0.294118f );
                ColorA co = ColorA( 1, 0.772549f, 0.372549f, 0.392157f );
                ColorA coh = ColorA( 0.0980392f, 0.54902f, 0.0352941f, 0.784314f );
                ColorA cf = ColorA( 0.862745f, 0.980392f, 0.980392f, 0.784314f );
                ColorA cfh = ColorA( 0.937255f, 0.345098f, 0.552941f, 1 );
                ColorA cp = ColorA( 0.996078f, 0.662745f, 0.0705882f, 0.392157f );
                ColorA cpo = ColorA( 1, 0.772549f, 0.372549f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;                  
                
            case CI_UI_THEME_MINPINK:
            {
                ColorA cb = ColorA( 0.862745f, 0.980392f, 0.980392f, 0.294118f );
                ColorA co = ColorA( 0.0980392f, 0.54902f, 0.0352941f, 0.392157f );
                ColorA coh = ColorA( 0.862745f, 0.980392f, 0.980392f, 0.784314f );
                ColorA cf = ColorA( 0.937255f, 0.345098f, 0.552941f, 0.784314f );
                ColorA cfh = ColorA( 0.996078f, 0.662745f, 0.0705882f, 1 );
                ColorA cp = ColorA( 1, 0.772549f, 0.372549f, 0.392157f );
                ColorA cpo = ColorA( 0.0980392f, 0.54902f, 0.0352941f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            case CI_UI_THEME_PEPTOBISMOL:
            {
                ColorA cb = ColorA( 0.87451f, 0.0823529f, 0.101961f, 0.294118f );
                ColorA co = ColorA( 0, 0.854902f, 0.235294f, 0.392157f );
                ColorA coh = ColorA( 0.87451f, 0.0823529f, 0.101961f, 0.784314f );
                ColorA cf = ColorA( 0.956863f, 0.952941f, 0.156863f, 0.784314f );
                ColorA cfh = ColorA( 0.992157f, 0.52549f, 0.0117647f, 1 );
                ColorA cp = ColorA( 0, 0.796078f, 0.905882f, 0.392157f );
                ColorA cpo = ColorA( 0, 0.854902f, 0.235294f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            case CI_UI_THEME_BILEBLUE:
            {
                ColorA cb = ColorA( 0.992157f, 0.52549f, 0.0117647f, 0.294118f );
                ColorA co = ColorA( 0.956863f, 0.952941f, 0.156863f, 0.392157f );
                ColorA coh = ColorA( 0.992157f, 0.52549f, 0.0117647f, 0.784314f );
                ColorA cf = ColorA( 0, 0.796078f, 0.905882f, 0.784314f );
                ColorA cfh = ColorA( 0, 0.854902f, 0.235294f, 1 );
                ColorA cp = ColorA( 0.87451f, 0.0823529f, 0.101961f, 0.392157f );
                ColorA cpo = ColorA( 0.956863f, 0.952941f, 0.156863f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_COOLCLAY:
            {
                ColorA cb = ColorA( 0.6f, 0.894118f, 1, 0.294118f );
                ColorA co = ColorA( 0.294118f, 0.34902f, 0.419608f, 0.392157f );
                ColorA coh = ColorA( 0.6f, 0.894118f, 1, 0.784314f );
                ColorA cf = ColorA( 0.968627f, 0.309804f, 0.309804f, 0.784314f );
                ColorA cfh = ColorA( 1, 0.231373f, 0.231373f, 1 );
                ColorA cp = ColorA( 0.105882f, 0.12549f, 0.14902f, 0.392157f );
                ColorA cpo = ColorA( 0.294118f, 0.34902f, 0.419608f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_BLUEBLUE:
            {
                ColorA cb = ColorA( 0, 0.678431f, 0.937255f, 0.294118f );
                ColorA co = ColorA( 1, 0.94902f, 0, 0.392157f );
                ColorA coh = ColorA( 0, 0, 0, 0.784314f );
                ColorA cf = ColorA( 0, 0.678431f, 0.937255f, 0.784314f );
                ColorA cfh = ColorA( 0.92549f, 0, 0.54902f, 1 );
                ColorA cp = ColorA( 1, 0.94902f, 0, 0.392157f );
                ColorA cpo = ColorA( 0, 0, 0, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_PINKPANTHER:
            {
                ColorA cb = ColorA( 0.92549f, 0, 0.54902f, 0.294118f );
                ColorA co = ColorA( 0, 0, 0, 0.392157f );
                ColorA coh = ColorA( 0, 0.678431f, 0.937255f, 0.784314f );
                ColorA cf = ColorA( 0.92549f, 0, 0.54902f, 0.784314f );
                ColorA cfh = ColorA( 1, 0.94902f, 0, 1 );
                ColorA cp = ColorA( 0, 0, 0, 0.392157f );
                ColorA cpo = ColorA( 0, 0.678431f, 0.937255f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            case CI_UI_THEME_MAROON:
            {
                ColorA cb = ColorA( 0.396078f, 0.588235f, 0.619608f, 0.294118f );
                ColorA co = ColorA( 0.858824f, 0.85098f, 0.823529f, 0.392157f );
                ColorA coh = ColorA( 0.396078f, 0.588235f, 0.619608f, 0.784314f );
                ColorA cf = ColorA( 0.670588f, 0.0784314f, 0.172549f, 0.784314f );
                ColorA cfh = ColorA( 0.741176f, 0.858824f, 0.870588f, 1 );
                ColorA cp = ColorA( 0.803922f, 0.831373f, 0.423529f, 0.392157f );
                ColorA cpo = ColorA( 0.858824f, 0.85098f, 0.823529f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
       
            case CI_UI_THEME_PINKLATTE:
            {
                ColorA cb = ColorA( 0.854902f, 0.847059f, 0.654902f, 0.294118f );
                ColorA co = ColorA( 0.498039f, 0.780392f, 0.686275f, 0.392157f );
                ColorA coh = ColorA( 0.854902f, 0.847059f, 0.654902f, 0.784314f );
                ColorA cf = ColorA( 1, 0.239216f, 0.498039f, 0.784314f );
                ColorA cfh = ColorA( 1, 0.619608f, 0.615686f, 1 );
                ColorA cp = ColorA( 0.247059f, 0.721569f, 0.686275f, 0.392157f );
                ColorA cpo = ColorA( 0.498039f, 0.780392f, 0.686275f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_MINGREEN:
            {
                ColorA cb = ColorA( 1, 1, 1, 0.294118f );
                ColorA co = ColorA( 0.94902f, 0.901961f, 0.760784f, 0.392157f );
                ColorA coh = ColorA( 1, 1, 1, 0.784314f );
                ColorA cf = ColorA( 0.435294f, 0.74902f, 0.635294f, 0.784314f );
                ColorA cfh = ColorA( 0.74902f, 0.721569f, 0.682353f, 1 );
                ColorA cp = ColorA( 0.94902f, 0.780392f, 0.466667f, 0.392157f );
                ColorA cpo = ColorA( 0.94902f, 0.901961f, 0.760784f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_HELLOYELLOW:
            {
                ColorA cb = ColorA( 1, 0.827451f, 0, 0.294118f );
                ColorA co = ColorA( 0.290196f, 0.729412f, 0.690196f, 0.392157f );
                ColorA coh = ColorA( 0.596078f, 0.129412f, 0, 0.784314f );
                ColorA cf = ColorA( 1, 0.827451f, 0, 0.784314f );
                ColorA cfh = ColorA( 1, 0.960784f, 0.619608f, 1 );
                ColorA cp = ColorA( 0.290196f, 0.729412f, 0.690196f, 0.392157f );
                ColorA cpo = ColorA( 0.596078f, 0.129412f, 0, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_TEALTEAL:
            {
                ColorA cb = ColorA( 0.290196f, 0.729412f, 0.690196f, 0.294118f );
                ColorA co = ColorA( 1, 0.827451f, 0, 0.392157f );
                ColorA coh = ColorA( 1, 0.960784f, 0.619608f, 0.784314f );
                ColorA cf = ColorA( 0.290196f, 0.729412f, 0.690196f, 0.784314f );
                ColorA cfh = ColorA( 0.596078f, 0.129412f, 0, 1 );
                ColorA cp = ColorA( 1, 0.827451f, 0, 0.392157f );
                ColorA cpo = ColorA( 1, 0.960784f, 0.619608f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_RUSTICORANGE:
            {
                ColorA cb = ColorA( 0.419608f, 0.333333f, 0.188235f, 0.294118f );
                ColorA co = ColorA( 0.192157f, 0.188235f, 0.258824f, 0.392157f );
                ColorA coh = ColorA( 0.419608f, 0.333333f, 0.188235f, 0.784314f );
                ColorA cf = ColorA( 1, 0.427451f, 0.141176f, 0.784314f );
                ColorA cfh = ColorA( 1, 0.921569f, 0.419608f, 1 );
                ColorA cp = ColorA( 0.164706f, 0.529412f, 0.196078f, 0.392157f );
                ColorA cpo = ColorA( 0.192157f, 0.188235f, 0.258824f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_TEALSALMON:
            {
                ColorA cb = ColorA( 0.305882f, 0.521569f, 0.533333f, 0.294118f );
                ColorA co = ColorA( 0.219608f, 0.270588f, 0.231373f, 0.392157f );
                ColorA coh = ColorA( 0.305882f, 0.521569f, 0.533333f, 0.784314f );
                ColorA cf = ColorA( 1, 0.27451f, 0.329412f, 0.784314f );
                ColorA cfh = ColorA( 1, 0.835294f, 0.415686f, 1 );
                ColorA cp = ColorA( 1, 0.996078f, 0.827451f, 0.392157f );
                ColorA cpo = ColorA( 0.219608f, 0.270588f, 0.231373f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_CITRUSBLUE:
            {
                ColorA cb = ColorA( 0.223529f, 0.556863f, 0.713725f, 0.294118f );
                ColorA co = ColorA( 0.133333f, 0.407843f, 0.533333f, 0.392157f );
                ColorA coh = ColorA( 0.223529f, 0.556863f, 0.713725f, 0.784314f );
                ColorA cf = ColorA( 1, 0.635294f, 0, 0.784314f );
                ColorA cfh = ColorA( 1, 0.839216f, 0, 1 );
                ColorA cp = ColorA( 1, 0.960784f, 0, 0.392157f );
                ColorA cpo = ColorA( 0.133333f, 0.407843f, 0.533333f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_LIMEPURPLE:
            {
                ColorA cb = ColorA( 0.341176f, 0.211765f, 1, 0.294118f );
                ColorA co = ColorA( 0.14902f, 0.14902f, 0.14902f, 0.392157f );
                ColorA coh = ColorA( 0.341176f, 0.211765f, 1, 0.784314f );
                ColorA cf = ColorA( 0.905882f, 1, 0.211765f, 0.784314f );
                ColorA cfh = ColorA( 1, 0.211765f, 0.435294f, 1 );
                ColorA cp = ColorA( 0.137255f, 0.454902f, 0.870588f, 0.392157f );
                ColorA cpo = ColorA( 0.14902f, 0.14902f, 0.14902f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_LIMESTONE2:
            {
                ColorA cb = ColorA( 0.396078f, 0.384314f, 0.45098f, 0.294118f );
                ColorA co = ColorA( 0.34902f, 0.729412f, 0.662745f, 0.392157f );
                ColorA coh = ColorA( 0.396078f, 0.384314f, 0.45098f, 0.784314f );
                ColorA cf = ColorA( 0.847059f, 0.945098f, 0.443137f, 0.784314f );
                ColorA cfh = ColorA( 0.988235f, 1, 0.85098f, 1 );
                ColorA cp = ColorA( 0.25098f, 0.0705882f, 0.172549f, 0.392157f );
                ColorA cpo = ColorA( 0.34902f, 0.729412f, 0.662745f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                                
            case CI_UI_THEME_COOLPURPLE:
            {
                ColorA cb = ColorA( 0.14902f, 0.537255f, 0.913725f, 0.294118f );
                ColorA co = ColorA( 0.0431373f, 0.964706f, 0.576471f, 0.392157f );
                ColorA coh = ColorA( 0.14902f, 0.537255f, 0.913725f, 0.784314f );
                ColorA cf = ColorA( 0.913725f, 0.101961f, 0.615686f, 0.784314f );
                ColorA cfh = ColorA( 0.964706f, 0.713725f, 0.0431373f, 1 );
                ColorA cp = ColorA( 0.964706f, 0.94902f, 0.0431373f, 0.392157f );
                ColorA cpo = ColorA( 0.0431373f, 0.964706f, 0.576471f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            case CI_UI_THEME_GRAYRED:
            {
                ColorA cb = ColorA( 0.160784f, 0.133333f, 0.121569f, 0.294118f );
                ColorA co = ColorA( 0.0745098f, 0.454902f, 0.490196f, 0.392157f );
                ColorA coh = ColorA( 0.160784f, 0.133333f, 0.121569f, 0.784314f );
                ColorA cf = ColorA( 0.988235f, 0.207843f, 0.298039f, 0.784314f );
                ColorA cfh = ColorA( 0.988235f, 0.968627f, 0.772549f, 1 );
                ColorA cp = ColorA( 0.0392157f, 0.74902f, 0.737255f, 0.392157f );
                ColorA cpo = ColorA( 0.0745098f, 0.454902f, 0.490196f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            case CI_UI_THEME_METALGEAR2:
            {
                ColorA cb = ColorA( 0.803922f, 0.741176f, 0.682353f, 0.294118f );
                ColorA co = ColorA( 0.478431f, 0.356863f, 0.243137f, 0.392157f );
                ColorA coh = ColorA( 0.803922f, 0.741176f, 0.682353f, 0.784314f );
                ColorA cf = ColorA( 0.980392f, 0.294118f, 0, 0.784314f );
                ColorA cfh = ColorA( 0.980392f, 0.980392f, 0.980392f, 1 );
                ColorA cp = ColorA( 0.121569f, 0.121569f, 0.121569f, 0.392157f );
                ColorA cpo = ColorA( 0.478431f, 0.356863f, 0.243137f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            case CI_UI_THEME_LIGHTPINK:
            {
                ColorA cb = ColorA( 0.619608f, 0.117647f, 0.298039f, 0.294118f );
                ColorA co = ColorA( 0.560784f, 0.560784f, 0.560784f, 0.392157f );
                ColorA coh = ColorA( 0.619608f, 0.117647f, 0.298039f, 0.784314f );
                ColorA cf = ColorA( 0.92549f, 0.92549f, 0.92549f, 0.784314f );
                ColorA cfh = ColorA( 1, 0.0666667f, 0.407843f, 1 );
                ColorA cp = ColorA( 0.145098f, 0.00784314f, 0.0588235f, 0.392157f );
                ColorA cpo = ColorA( 0.560784f, 0.560784f, 0.560784f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            case CI_UI_THEME_MINPINK2:
            {
                ColorA cb = ColorA( 0.92549f, 0.92549f, 0.92549f, 0.294118f );
                ColorA co = ColorA( 0.619608f, 0.117647f, 0.298039f, 0.392157f );
                ColorA coh = ColorA( 0.92549f, 0.92549f, 0.92549f, 0.784314f );
                ColorA cf = ColorA( 1, 0.0666667f, 0.407843f, 0.784314f );
                ColorA cfh = ColorA( 0.145098f, 0.00784314f, 0.0588235f, 1 );
                ColorA cp = ColorA( 0.560784f, 0.560784f, 0.560784f, 0.392157f );
                ColorA cpo = ColorA( 0.619608f, 0.117647f, 0.298039f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            case CI_UI_THEME_MAXPINK:
            {
                ColorA cb = ColorA( 1, 0.0784314f, 0.341176f, 0.294118f );
                ColorA co = ColorA( 0.0392157f, 0.0392157f, 0.0392157f, 0.392157f );
                ColorA coh = ColorA( 0.890196f, 0.964706f, 1, 0.784314f );
                ColorA cf = ColorA( 1, 0.0784314f, 0.341176f, 0.784314f );
                ColorA cfh = ColorA( 1, 0.847059f, 0.490196f, 1 );
                ColorA cp = ColorA( 0.0392157f, 0.0392157f, 0.0392157f, 0.392157f );
                ColorA cpo = ColorA( 0.890196f, 0.964706f, 1, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            case CI_UI_THEME_MINYELLOW:
            {
                ColorA cb = ColorA( 0.898039f, 0.894118f, 0.854902f, 0.294118f );
                ColorA co = ColorA( 0.847059f, 0.823529f, 0.6f, 0.392157f );
                ColorA coh = ColorA( 0.898039f, 0.894118f, 0.854902f, 0.784314f );
                ColorA cf = ColorA( 0.960784f, 0.878431f, 0.219608f, 0.784314f );
                ColorA cfh = ColorA( 0.0901961f, 0.0862745f, 0.360784f, 1 );
                ColorA cp = ColorA( 0.745098f, 0.74902f, 0.619608f, 0.392157f );
                ColorA cpo = ColorA( 0.847059f, 0.823529f, 0.6f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_MINLIME:
            {
                ColorA cb = ColorA( 0.960784f, 0.882353f, 0.886275f, 0.294118f );
                ColorA co = ColorA( 0.882353f, 0.717647f, 0.929412f, 0.392157f );
                ColorA coh = ColorA( 0.960784f, 0.882353f, 0.886275f, 0.784314f );
                ColorA cf = ColorA( 0.72549f, 0.870588f, 0.317647f, 0.784314f );
                ColorA cfh = ColorA( 0.819608f, 0.890196f, 0.537255f, 1 );
                ColorA cp = ColorA( 0.878431f, 0.282353f, 0.568627f, 0.392157f );
                ColorA cpo = ColorA( 0.882353f, 0.717647f, 0.929412f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_MINORANGE:
            {
                ColorA cb = ColorA( 0.8f, 0.8f, 0.8f, 0.294118f );
                ColorA co = ColorA( 0.435294f, 0.435294f, 0.435294f, 0.392157f );
                ColorA coh = ColorA( 0.8f, 0.8f, 0.8f, 0.784314f );
                ColorA cf = ColorA( 1, 0.392157f, 0, 0.784314f );
                ColorA cfh = ColorA( 1, 1, 1, 1 );
                ColorA cp = ColorA( 0.2f, 0.2f, 0.2f, 0.392157f );
                ColorA cpo = ColorA( 0.435294f, 0.435294f, 0.435294f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_GRAYDAY:
            {
                ColorA cb = ColorA( 0.694118f, 0.776471f, 0.8f, 0.294118f );
                ColorA co = ColorA( 1, 1, 1, 0.392157f );
                ColorA coh = ColorA( 0.0784314f, 0.0784314f, 0.0784314f, 0.784314f );
                ColorA cf = ColorA( 0.694118f, 0.776471f, 0.8f, 0.784314f );
                ColorA cfh = ColorA( 1, 0.937255f, 0.368627f, 1 );
                ColorA cp = ColorA( 1, 1, 1, 0.392157f );
                ColorA cpo = ColorA( 0.0784314f, 0.0784314f, 0.0784314f, 0.784314f );
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  

            case CI_UI_THEME_MINBLACK:
            {
                ColorA cb = ColorA( 1, 1, 1, 0.294118f );
                ColorA co = ColorA( 0.819608f, 0.905882f, 0.317647f, 0.392157f );
                ColorA coh = ColorA( 1, 1, 1, 0.784314f );
                ColorA cf = ColorA( 0, 0, 0, 0.784314f );
                ColorA cfh = ColorA( 0.14902f, 0.678431f, 0.894118f, 1 );
                ColorA cp = ColorA( 0.301961f, 0.737255f, 0.913725f, 0.392157f );
                ColorA cpo = ColorA( 0.819608f, 0.905882f, 0.317647f, 0.784314f );                                
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break;  
                
            default:
            {
                ColorA cb = CI_UI_COLOR_BACK; 
                ColorA co = CI_UI_COLOR_OUTLINE; 
                ColorA coh = CI_UI_COLOR_OUTLINE_HIGHLIGHT;
                ColorA cf = CI_UI_COLOR_FILL; 
                ColorA cfh = CI_UI_COLOR_FILL_HIGHLIGHT;
                ColorA cp = CI_UI_COLOR_PADDED;
                ColorA cpo = CI_UI_COLOR_PADDED_OUTLINE;
                setUIColors( cb, co, coh, cf, cfh, cp, cpo );                                 
            }
                break; 
        }
    }
    
    
	void setWidgetColor(int _target, ColorA _color)
	{
		switch (_target) 
		{
			case CI_UI_WIDGET_COLOR_BACK:
				for(unsigned int i = 0; i < widgets.size(); i++)
				{
					widgets[i]->setColorBack(_color); 
				}				
				break;

			case CI_UI_WIDGET_COLOR_OUTLINE:
				for(unsigned int i = 0; i < widgets.size(); i++)
				{
					widgets[i]->setColorOutline(_color); 
				}				
				break;
			
			case CI_UI_WIDGET_COLOR_OUTLINE_HIGHLIGHT:
				for(unsigned int i = 0; i < widgets.size(); i++)
				{
					widgets[i]->setColorOutlineHighlight(_color); 
				}				
				break;
			
			case CI_UI_WIDGET_COLOR_FILL:
				for(unsigned int i = 0; i < widgets.size(); i++)
				{
					widgets[i]->setColorFill(_color); 
				}				
				break;
			
			case CI_UI_WIDGET_COLOR_FILL_HIGHLIGHT:
				for(unsigned int i = 0; i < widgets.size(); i++)
				{
					widgets[i]->setColorFillHighlight(_color); 
				}					
				break;
                
			case CI_UI_WIDGET_COLOR_PADDED:
				for(unsigned int i = 0; i < widgets.size(); i++)
				{
					widgets[i]->setColorPadded(_color); 
				}					
				break;
                
			case CI_UI_WIDGET_COLOR_PADDED_OUTLINE:
				for(unsigned int i = 0; i < widgets.size(); i++)
				{
					widgets[i]->setColorPaddedOutline(_color); 
				}					
				break;                
				
			default:
				break;
		}
	}
    
    ciUIWidget *getWidget(string _name)
	{
		return widgets_map[_name]; 
	}
	
    void removeWidget(string _name)    
    {
        ciUIWidget *toDelete = getWidget(_name);
        if(toDelete != NULL)
        {
            removeWidget(toDelete);
        }    
    }
    
	void setDrawPadding(bool _draw_padded_rect)
	{
		draw_padded_rect = _draw_padded_rect; 
	}

    void setDrawWidgetPadding(bool _draw_padded_rect)
    {
		for(unsigned int i = 0; i < widgets.size(); i++)
		{
			widgets[i]->setDrawPadding(_draw_padded_rect); 
		}		        
    }

	void setDrawPaddingOutline(bool _draw_padded_rect_outline)
	{
		draw_padded_rect_outline = _draw_padded_rect_outline; 
	}

    void setDrawWidgetPaddingOutline(bool _draw_padded_rect_outline)
	{
		for(unsigned int i = 0; i < widgets.size(); i++)
		{
			widgets[i]->setDrawPaddingOutline(_draw_padded_rect_outline); 
		}		
	}

    
    vector<ciUIWidget*> getWidgets()
    {
        return widgets;
    }
    
    vector<ciUIWidget*> getWidgetsOfType(ciUIWidgetType type)
    {
        vector<ciUIWidget*> widgetToReturn; 
        for(unsigned int i = 0; i < widgets.size(); i++)
		{
            if(widgets[i]->getKind() == type)
            {
                widgetToReturn.push_back(widgets[i]);
            }
		}	 
        return widgetToReturn;                                         
    }

    template<typename T> ci::CallbackId registerUIEvents( T *obj, void (T::*callback)( ciUIEvent *event ) )
    {
        return uiEventCallbackMgr.registerCb( std::bind1st( std::mem_fun( callback ), obj ) );
    }
    
    void unregisterUIEvents( ci::CallbackId id ) 
    {
        uiEventCallbackMgr.unregisterCb( id ); 
    }

	
protected:    
    
    void pushbackWidget(ciUIWidget *widget)
    {
        widget->setID(uniqueIDs); 
        uniqueIDs++;
        widgets.push_back(widget);    
		widgets_map[widget->getName()] = widget;                             
    }
    
    ci::CallbackMgr<void(ciUIEvent*)> uiEventCallbackMgr;
    
    
#if defined( CINDER_COCOA_TOUCH )
    app::AppCocoaTouch *mApp;
    ci::CallbackId mCbTouchesBegan, mCbTouchesMoved, mCbTouchesEnded; 
#else
    app::App *mApp;

    ci::signals::scoped_connection mCbMouseDown, mCbMouseDrag, mCbMouseUp, mCbMouseMove;
    ci::signals::scoped_connection mCbKeyDown, mCbKeyUp;
#endif 
    
    gl::TextureFontRef font_large; 	
    gl::TextureFontRef font_medium; 		
    gl::TextureFontRef font_small;
 	
    Font fontLarge; 
    Font fontMedium; 
    Font fontSmall;     
    
    
	ciUIEvent *GUIevent; 
    int state; 
    bool hasSharedResources;
    
    map<string, ciUIWidget*> widgets_map;     
	vector<ciUIWidget*> widgets; 
	vector<ciUIWidget*> widgetsWithState;     
	ciUIWidget *lastAdded; 
	ciUIWidget *activeFocusedWidget; 
	bool enable_highlight_outline; 
	bool enable_highlight_fill; 	
	bool enabled; 
    int uniqueIDs; 
    bool hasKeyBoard; 
    
    float widgetSpacing; 
    
    string fontName;

    bool updateFont(ciUIWidgetFontType _kind, string filename, int fontsize) 
    {
        bool success = true;
        switch(_kind)
        {
            case CI_UI_FONT_LARGE:              
                fontLarge = Font(  filename, fontsize);
                font_large = gl::TextureFont::create(fontLarge);
                break; 
                
            case CI_UI_FONT_MEDIUM:
                fontMedium = Font( filename, fontsize);                
                font_medium = gl::TextureFont::create(fontMedium);
                
                break; 
                
            case CI_UI_FONT_SMALL:
                fontSmall = Font( filename, fontsize);                
                font_small = gl::TextureFont::create(fontSmall);

                break; 

        }
        return success;
    }
    
    void checkForKeyFocus(ciUIWidget *child)
    {
        if(child->getKind() == CI_UI_WIDGET_TEXTINPUT)
        {
            ciUITextInput *textinput = (ciUITextInput *) child;         
            switch(textinput->getTriggerType())
            {
                case CI_UI_TEXTINPUT_ON_FOCUS:
                {
                    cout << "has focus" << endl; 
                    hasKeyBoard = true;         
                }
                    break; 
                    
                case CI_UI_TEXTINPUT_ON_ENTER:
                {
                    cout << "has focus" << endl;                     
                    hasKeyBoard = true; 
                }        
                    break; 
                    
                case CI_UI_TEXTINPUT_ON_UNFOCUS:
                {
                    cout << "lost focus" << endl;  
                    hasKeyBoard = false; 
                }        
                    break; 
                    
                default:
                {
                    hasKeyBoard = false; 
                }
                    break;                     
            }
        }
    }
    
};

#endif

