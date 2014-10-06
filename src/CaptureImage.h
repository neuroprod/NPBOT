//
//  CaptureImage.h
//  NPBOT
//
//  Created by Kris Temmerman on 05/10/14.
//
//

#ifndef NPBOT_CaptureImage_h
#define NPBOT_CaptureImage_h
#include "cinder/gl/gl.h"

#include "CinderOpenCV.h"

class CaptureImage
{
public:
    CaptureImage()
    {
    
    
    
    }
    void calculate()
    {
        string r = "image" +toString(id)+".png";
    writeImage( r, mSurface );
    
        cv::Mat input( toOcv( mSurface) ), output;
       // remap(input, output, *map1, *map2,cv::INTER_LINEAR);
        
        
		mTextureResult = gl::Texture( fromOcv( input ) );
    
    }
    int id;
    float posZ;
    Surface8u        mSurface;
    gl::TextureRef		mTexture;
     gl::Texture		mTextureResult;
    cv::Mat * map1;
    cv::Mat *map2;
    void draw()
    {
        float scale =700.0f/640.0f;
        gl::pushMatrices();
    gl::translate(Vec3f(350,0,0));
        gl::translate(Vec3f(0,0,posZ+350));
        gl::rotate(Vec3f(90,0,-90));
        gl::scale(scale,scale,scale);
        gl::draw( mTextureResult );
        gl::popMatrices();
    
    }

};

#endif
