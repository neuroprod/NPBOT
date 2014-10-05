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
    
    
        cv::Mat input( toOcv( mSurface) ), output;
        const int CHESSBOARD_WIDTH = 7;
        const int CHESSBOARD_HEIGHT = 10;
        
        
        
        
        std::vector<cv::Point2f> img_corners(CHESSBOARD_WIDTH*CHESSBOARD_HEIGHT);
        bool found= cv::findChessboardCorners(input, cv::Size(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT),img_corners);
        
        
        cv::drawChessboardCorners( input, cv::Size(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT),img_corners,found);
        
        
		mTextureResult = gl::Texture( fromOcv( input ) );
    
    }
    float posZ;
    Surface8u        mSurface;
    gl::TextureRef		mTexture;
     gl::Texture		mTextureResult;
    void draw()
    {
        gl::pushMatrices();
        
        gl::translate(Vec3f(0,0,posZ));
        gl::rotate(Vec3f(90,0,0));
        gl::draw( mTextureResult );
        gl::popMatrices();
    
    }

};

#endif
