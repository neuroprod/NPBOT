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

#ifndef CIUI_FPS
#define CIUI_FPS

#include "cinder/app/App.h"
#include "ciUILabel.h"

class ciUIFPS : public ciUILabel
{
public:    
    ciUIFPS(float x, float y, int _size)
    {
        rect = new ciUIRectangle(x,y,0,0); 
        init("FPS", "FPS: 60.000", _size); 
		kind = CI_UI_WIDGET_FPS; 		
        labelPrecision = 3;
        autoSize = true; 
    }
    
    ciUIFPS(int _size)
    {
        rect = new ciUIRectangle(0,0,0,0); 
        init("FPS", "FPS: 60.000", _size); 
		kind = CI_UI_WIDGET_FPS; 		
        labelPrecision = 3;
        autoSize = true; 
    }    
    
	void update()
	{            
		setLabel("FPS: " + numToString(ci::app::App::get()->getAverageFps(), labelPrecision));
	}
	
    void setLabelPrecision(int _precision) 
    {
        labelPrecision = _precision;
    }

    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
    int labelPrecision;    
}; 

#endif