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

#ifndef CIUI
#define CIUI

using namespace ci;
using namespace ci::app;
using namespace std; 

#define CI_UI_STATE_NORMAL 0
#define CI_UI_STATE_OVER 1
#define CI_UI_STATE_DOWN 2
#define CI_UI_STATE_SUSTAINED 3

#define CI_UI_GLOBAL_PADDING 4
#define CI_UI_GLOBAL_WIDGET_SPACING 4

#define CI_UI_COLOR_BACK ci::ColorA(0.0f, 0.0f, 0.0f, 50.0f/255.0f)                       //the rect's back color
#define CI_UI_COLOR_OUTLINE ci::ColorA(1.0f, 1.0f, 1.0f, 100.0f/255.0f)                 //the rect's outline color 
#define CI_UI_COLOR_OUTLINE_HIGHLIGHT ci::ColorA(1.0f, 1.0f, 1.0f, 200.0f/255.0f)       //the rect's onMouseOver outline highlight color         
#define CI_UI_COLOR_FILL ci::ColorA(1.0f, 1.0f, 1.0f, 200.0f/255.0f)                    //the rect's fill color 
#define CI_UI_COLOR_FILL_HIGHLIGHT ci::ColorA(1.0f, 1.0f, 1.0f, 1.0f)            //the rect's onMouseDown highlight color 

#define CI_UI_COLOR_PADDED ci::ColorA(0.0f,0.0f,0.0f, 100.0f/255.0f)                                //the rect's padded color
#define CI_UI_COLOR_PADDED_OUTLINE ci::ColorA(1.0f, 1.0f, 1.0f, 200.0f/255.0f)                      //the rect's padded outline color 

#define CI_UI_DRAW_PADDING false
#define CI_UI_DRAW_PADDING_OUTLINE false

#define CI_UI_TEXTINPUT_ON_ENTER 0 
#define CI_UI_TEXTINPUT_ON_FOCUS 1
#define CI_UI_TEXTINPUT_ON_UNFOCUS 2

enum ciUIWidgetType
{
	CI_UI_WIDGET_LABEL = 0,
	CI_UI_WIDGET_CANVAS = 1,
	CI_UI_WIDGET_TOGGLE = 2,
	CI_UI_WIDGET_BUTTON = 3,
	CI_UI_WIDGET_SLIDER_H = 4,
	CI_UI_WIDGET_SLIDER_V = 5,
	CI_UI_WIDGET_RSLIDER_H = 6,
	CI_UI_WIDGET_RSLIDER_V = 7,	
	CI_UI_WIDGET_NUMBERDIALER = 8,
	CI_UI_WIDGET_RADIO = 9,
	CI_UI_WIDGET_FPS = 10,
	CI_UI_WIDGET_2DPAD = 11, 
	CI_UI_WIDGET_TEXTINPUT = 12, 
	CI_UI_WIDGET_WAVEFORM = 13,
	CI_UI_WIDGET_SPECTRUM = 14,
	CI_UI_WIDGET_IMAGE = 15, 
    CI_UI_WIDGET_LABELBUTTON = 16, 
    CI_UI_WIDGET_LABELTOGGLE = 17,
    CI_UI_WIDGET_TOGGLEMATRIX = 18,
    CI_UI_WIDGET_IMAGEBUTTON = 19, 
    CI_UI_WIDGET_IMAGETOGGLE = 20,
    CI_UI_WIDGET_ROTARYSLIDER = 21, 
    CI_UI_WIDGET_DROPDOWNLIST = 22, 
    CI_UI_WIDGET_SPACER = 23, 
    CI_UI_WIDGET_MOVINGGRAPH = 24,
    CI_UI_WIDGET_MINIMALSLIDER = 25,
    CI_UI_WIDGET_BILABELSLIDER = 26,
    CI_UI_WIDGET_CIRCLESLIDER = 27, 
    CI_UI_WIDGET_IMAGESAMPLER = 28,
	CI_UI_WIDGET_SCROLLABLECANVAS = 29, 
    CI_UI_WIDGET_MULTIIMAGEBUTTON = 30,
    CI_UI_WIDGET_MULTIIMAGETOGGLE = 31,
    CI_UI_WIDGET_MULTIIMAGESLIDER_H = 32,
    CI_UI_WIDGET_MULTIIMAGESLIDER_V = 33,
    CI_UI_WIDGET_IMAGESLIDER_H = 34,
    CI_UI_WIDGET_IMAGESLIDER_V = 35
};

enum ciUIWidgetOrientation
{
	CI_UI_ORIENTATION_VERTICAL = 0, 
	CI_UI_ORIENTATION_HORIZONTAL = 1
};

enum ciUIWidgetInputDirection
{
	CI_UI_DIRECTION_NORTHSOUTH = 0,
	CI_UI_DIRECTION_SOUTHNORTH = 1,    
	CI_UI_DIRECTION_EASTWEST = 2,
	CI_UI_DIRECTION_WESTEAST = 3
};

enum ciUIWidgetAlignment
{
	CI_UI_ALIGN_LEFT = 0,
	CI_UI_ALIGN_FREE = 1,
	CI_UI_ALIGN_RIGHT = 2,    
    CI_UI_ALIGN_TOP = 3, 
    CI_UI_ALIGN_BOTTOM = 4
};

enum ciUIWidgetFontType 
{
	CI_UI_FONT_LARGE = 0, 
	CI_UI_FONT_MEDIUM = 1, 
	CI_UI_FONT_SMALL = 2 	
};

enum ciUIWidgetColorType
{
	CI_UI_WIDGET_COLOR_BACK = 0, 
	CI_UI_WIDGET_COLOR_OUTLINE = 1,
	CI_UI_WIDGET_COLOR_OUTLINE_HIGHLIGHT = 2, 
	CI_UI_WIDGET_COLOR_FILL = 3, 
	CI_UI_WIDGET_COLOR_FILL_HIGHLIGHT = 4, 
    CI_UI_WIDGET_COLOR_PADDED = 5, 
	CI_UI_WIDGET_COLOR_PADDED_OUTLINE = 6     
}; 

enum ciUIThemeType
{
    CI_UI_THEME_DEFAULT = 0,
    CI_UI_THEME_HACKER,
    CI_UI_THEME_HIPSTER,
    CI_UI_THEME_DIETER,
    CI_UI_THEME_BARBIE,
    CI_UI_THEME_WINDOWS,
    CI_UI_THEME_MACOSX,
    CI_UI_THEME_ZOOLANDER,
    CI_UI_THEME_VEGAN,
    CI_UI_THEME_BERLIN,
    CI_UI_THEME_METALGEAR,
    CI_UI_THEME_TEALLIME,
    CI_UI_THEME_VEGAN2,
    CI_UI_THEME_RUSTIC,
    CI_UI_THEME_MIDNIGHT,
    CI_UI_THEME_MINBLUE,
    CI_UI_THEME_LIMESTONE,
    CI_UI_THEME_SPEARMINT,
    CI_UI_THEME_MINPINK,
    CI_UI_THEME_PEPTOBISMOL, 
    CI_UI_THEME_BILEBLUE,
    CI_UI_THEME_COOLCLAY,
    CI_UI_THEME_BLUEBLUE,
    CI_UI_THEME_PINKPANTHER,
    CI_UI_THEME_MAROON,
    CI_UI_THEME_PINKLATTE,
    CI_UI_THEME_MINGREEN,
    CI_UI_THEME_HELLOYELLOW,
    CI_UI_THEME_TEALTEAL,
    CI_UI_THEME_RUSTICORANGE,
    CI_UI_THEME_TEALSALMON,
    CI_UI_THEME_CITRUSBLUE,
    CI_UI_THEME_LIMEPURPLE,
    CI_UI_THEME_LIMESTONE2,
    CI_UI_THEME_COOLPURPLE,
    CI_UI_THEME_GRAYRED,
    CI_UI_THEME_METALGEAR2,
    CI_UI_THEME_LIGHTPINK,
    CI_UI_THEME_MINPINK2,
    CI_UI_THEME_MAXPINK,
    CI_UI_THEME_MINYELLOW,
    CI_UI_THEME_MINLIME,
    CI_UI_THEME_MINORANGE,
    CI_UI_THEME_GRAYDAY,
    CI_UI_THEME_MINBLACK
};

#define CI_UI_FONT_NAME "NewMediaFett.ttf"

#define CI_UI_FONT_RESOLUTION 150

#if defined( CINDER_COCOA_TOUCH )

#define CI_UI_FONT_LARGE_SIZE 48
#define CI_UI_FONT_MEDIUM_SIZE 36
#define CI_UI_FONT_SMALL_SIZE 28

#else

#define CI_UI_FONT_LARGE_SIZE 18
#define CI_UI_FONT_MEDIUM_SIZE 16
#define CI_UI_FONT_SMALL_SIZE 12

#endif 

#define CI_UI_LABEL_DRAW_BACK false
#define CI_UI_LABEL_SHADOW_OFFSET 1

#include "ciUIRectangle.h"
#include "ciUIWidget.h"
#include "ciUIEvent.h"
#include "ciUIWidgetWithLabel.h"
#include "ciUILabel.h"
#include "ciUISlider.h"
#include "ciUIFPS.h"
#include "ciUIFPSSlider.h"
#include "ciUISpacer.h"
#include "ciUIMinimalSlider.h"
#include "ciUICircleSlider.h"
#include "ciUIRangeSlider.h"
#include "ciUIBiLabelSlider.h"
#include "ciUIButton.h"
#include "ciUIToggle.h"
#include "ciUIRadio.h"
#include "ciUI2DPad.h"
#include "ciUIToggleMatrix.h"
#include "ciUILabelButton.h"
#include "ciUILabelToggle.h"
#include "ciUIDropDownList.h"
#include "ciUINumberDialer.h"
#include "ciUIRotarySlider.h"
#include "ciUITextInput.h"
#include "ciUIWaveform.h"
#include "ciUIMovingGraph.h"
#include "ciUISpectrum.h"
#include "ciUIImage.h"
#include "ciUIImageSampler.h"
#include "ciUIImageButton.h"
#include "ciUIImageToggle.h"
#include "ciUIImageSlider.h"
#include "ciUIMultiImageButton.h"
#include "ciUIMultiImageToggle.h"
#include "ciUICanvas.h"
#include "ciUIScrollableCanvas.h"

#endif
