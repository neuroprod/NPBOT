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

#ifndef CIUI_MOVING_GRAPH
#define CIUI_MOVING_GRAPH

#include "cinder/Shape2d.h"
#include "ciUIWidget.h"

class ciUIMovingGraph : public ciUIWidget
{
public:    
    ciUIMovingGraph(float x, float y, float w, float h, vector<float> _buffer, int _bufferSize, float _min, float _max, string _name)
    {
        rect = new ciUIRectangle(x,y,w,h); 
        init(w, h, _buffer, _bufferSize, _min, _max, _name);
    }
    
    ciUIMovingGraph(float w, float h, vector<float> _buffer, int _bufferSize, float _min, float _max, string _name)
    {
        rect = new ciUIRectangle(0,0,w,h); 
        init(w, h, _buffer, _bufferSize, _min, _max, _name);
    }    
    
    void init(float w, float h, vector<float> _buffer, int _bufferSize, float _min, float _max, string _name)
    {
		name = _name; 				
		kind = CI_UI_WIDGET_MOVINGGRAPH; 
		
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0f, h+padding*2.0f);
		paddedRect->setParent(rect); 
		
        draw_fill = true; 
        
        buffer = _buffer;					//the widget's value
        
		bufferSize = _bufferSize; 
		max = _max; 
		min = _min; 		
		scale = rect->getHeight()*.5f; 
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
            
                ci::gl::pushMatrices();
                ci::gl::translate(rect->getX(),rect->getY()+scale);         
                shape.clear();
                
                shape.moveTo(0.0f, ci::lmap<float>(buffer[0], min, max, scale, -scale));
                for (int i = 1; i < bufferSize; i++)
                {			
                    shape.lineTo(inc*(float)i, ci::lmap<float>(buffer[i], min, max, scale, -scale)); 
                }                
                ci::gl::draw(shape);
                ci::gl::popMatrices();                
        }
    }
		        
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent; 
	}
	
    void addPoint(float _point)
    {
        buffer.push_back(_point); 
                
        if( buffer.size() >= bufferSize )
        {
            buffer.erase(buffer.begin(), buffer.begin()+1);
        }	        
    }
    
    vector<float> &getBuffer()
    {
        return buffer; 
    }
    
    void setBuffer(vector<float> _buffer)
    {
        buffer = _buffer; 
    }
    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
    vector<float> buffer;
	float max, min, scale, inc; 
	int bufferSize; 
    Shape2d shape; 
}; 



#endif
