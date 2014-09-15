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

#ifndef CIUI_SPECTRUM
#define CIUI_SPECTRUM

#include "cinder/Shape2d.h"
#include "ciUIWidget.h"

class ciUISpectrum : public ciUIWidget
{
public:
    ciUISpectrum(float x, float y, float w, float h, float *_buffer, int _bufferSize, float _min, float _max, string _name)
    {
        rect = new ciUIRectangle(x,y,w,h); 
        init(w,h,_buffer, _bufferSize, _min, _max, _name); 
    }
    
    ciUISpectrum(float w, float h, float *_buffer, int _bufferSize, float _min, float _max, string _name)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(w,h,_buffer, _bufferSize, _min, _max, _name); 
    }    

    void init(float w, float h, float *_buffer, int _bufferSize, float _min, float _max, string _name)
    {
		name = _name; 				
		kind = CI_UI_WIDGET_SPECTRUM; 
		
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding*2.0f);
		paddedRect->setParent(rect); 
        
        draw_fill = true; 
        
        if(_buffer != NULL)
        {
            buffer = _buffer;					//the widget's value
        }
        else
        {
            buffer = NULL; 
        }
        
		bufferSize = _bufferSize; 
		max = _max; 
		min = _min; 		
		scale = rect->getHeight(); 
		inc = rect->getWidth()/((float)bufferSize-1.0f); 
    }
    
    
    virtual void drawFill()
    {
        if(draw_fill)
        {			
			if(draw_fill_highlight)
			{
				ci::gl::color(color_fill_highlight); 
			}        
			else 
			{
				ci::gl::color(color_fill); 		 	
			}
            if(buffer != NULL)
            {                
                ci::gl::pushMatrices();
                ci::gl::translate(rect->getX(),rect->getY()+scale);         
                shape.clear();
                
                shape.moveTo(0.0f, 0.0f);
                for (int i = 0; i < bufferSize; i++)
                {			
                    shape.lineTo(inc*(float)i, ci::lmap<float>(buffer[i], min, max, 0, -scale)); 
                }                
                shape.lineTo(inc*(float)(bufferSize-1), 0.0f);
                
                shape.close();
                
                ci::gl::drawSolid(shape);
                ci::gl::popMatrices();                       
            }
        }
    }
	
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
	}	
    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
	float *buffer; 
	float max, min, scale, inc; 
	int bufferSize; 
    Shape2d shape;     
}; 



#endif
