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
#include "Cube.h"
#include "Constants.h"
class CaptureImage
{
public:
    CaptureImage()
    {
    
       
    
    }
     vector<Cube *>  cubes;
    float imgW;
    float imgH;
    void calculate()
    {
  
        
        cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
        
        
        cameraMatrix.at<Float64>(0,0) =503.910252793615;
        cameraMatrix.at<Float64>(0,1) =0;
        cameraMatrix.at<Float64>(0,2) =319.5;
        
        cameraMatrix.at<Float64>(1,0) =0;
        cameraMatrix.at<Float64>(1,1) =503.910252793615;
        cameraMatrix.at<Float64>(1,2) =239.5;
        
        cameraMatrix.at<Float64>(2,0) =0;
        cameraMatrix.at<Float64>(2,1) =0;
        cameraMatrix.at<Float64>(2,2) =1;
                
       cv::Mat distCoeffs = cv::Mat::zeros(8, 1, CV_64F);
        distCoeffs.at<Float64>(0,0) =-0.00449581614979722;
       distCoeffs.at<Float64>(1,0) =-0.01090092124412815;
        distCoeffs.at<Float64>(2,0) =0;
         distCoeffs.at<Float64>(3,0) =0;
         distCoeffs.at<Float64>(4,0) =-0.07410480760405815;
        
        string r = "image" +toString(id)+".png";
        writeImage( r, mSurface );
    
        cv::Mat inputPre( toOcv( mSurface) ), output;
        
         cv::Size  imageSize;
        
        imageSize = inputPre.size();
        cv::Mat  map1, map2;
        // undistort( view, rview, cameraMatrix, distCoeffs, cameraMatrix );
        cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
                                getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0),
                                imageSize, CV_16SC2, map1, map2);
        cv::Mat inputPre2;
        remap(inputPre, inputPre2, map1, map2, cv::INTER_LINEAR);

        
        
        
        
        
         //undistort( inputPre, inputPre2, cameraMatrix, distCoeffs, cameraMatrix );
        
        float realWith =700;
        // remap(input, output, *map1, *map2,cv::INTER_LINEAR);
        float scaleL=realWith/640.0f;
        imgW = realWith;
        imgH =480.f*scaleL;
        cv::Size size( imgW ,imgH);//the dst image size,e.g.100x100
        cv:: Mat input;//dst image
        
        cv::resize(inputPre2,input,size);
        
        
        int iLowH =5;
        int iHighH =30;
        
        int iLowS = 0;
        int iHighS = 255;
        
        int iLowV = 230;
        int iHighV = 255;
        
        
        cv::Mat imgHSV;
        
        cvtColor(input, imgHSV, cv::COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
        
        cv::Mat imgThresholded;
        
        inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
        
       //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
        
        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)) );
        
        cv::Mat gray;
        float thresh =50;
        Canny(imgThresholded, gray, 0, thresh, 5);
        // dilate canny output to remove potential
        // holes between edge segments
        dilate(gray, gray,cv::Mat(),cv::Point(-1,-1));
        
       
        
        vector<vector<cv::Point> > contours;
        findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
        
       // cv::polylines(gray, &p, &n, 1, true, cv::Scalar(0,0,255), 3);
        vector<cv::Point> approx;
       
        vector<vector<cv::Point> > squares;
        // test each contour
        for( size_t i = 0; i < contours.size(); i++ )
        {
            // approximate contour with accuracy proportional
            // to the contour perimeter
            approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
            
            // square contours should have 4 vertices after approximation
            // relatively large area (to filter out noisy contours)
            // and be convex.
            // Note: absolute value of an area is used because
            // area may be positive or negative - in accordance with the
            // contour orientation
            if( approx.size() == 4  &&
               fabs(contourArea(cv::Mat(approx))) > 1000 &&
               isContourConvex(cv::Mat(approx)) )
            {
                double maxCosine = 0;
                
                for( int j = 2; j < 5; j++ )
                {
                    // find the maximum cosine of the angle between joint edges
                    double cosine = fabs(findAngle(approx[j%4], approx[j-2], approx[j-1]));
                    maxCosine = MAX(maxCosine, cosine);
                }
                
                // if cosines of all angles are small
                // (all angles are ~90 degree) then write quandrange
                // vertices to resultant sequence
                if( maxCosine < 0.3 )
                    squares.push_back(approx);
                
                
            }
        }
        //console()<<"num squares"<<squares.size()<<endl;
        for( size_t i = 0; i < squares.size(); i++ )
        {
           //const cv::Point* p = &squares[i][0];
           // int n = (int)squares[i].size();
         // cv::polylines(input, &p, &n, 1, true, cv::Scalar(0,0,255), 3);
            
            Vec2f squareVector;
            squareVector.x =squares[i][0].x -squares[i][1].x;
            squareVector.y =squares[i][0].y -squares[i][1].y;
            float squareSize =squareVector.length();
            float angle =atan2(squareVector.y,squareVector.x);
            
            Vec2f center;
            center.x =(squares[i][0].x +squares[i][1].x +squares[i][2].x +squares[i][3].x)/4;
              center.y =(squares[i][0].y +squares[i][1].y +squares[i][2].y +squares[i][3].y)/4;
            
            Vec2f distVec;
            distVec.x  = center.x -(imgW/2);
            distVec.y  = center.y -(imgH/2);
           
            float distToCenter = distVec.length();
           // cout <<center<<"  -----------   " <<distToCenter<<endl;;
            Vec3f squareCenter;
            squareCenter.z =-center.x    +posZ+imgW/2;
            squareCenter.x =-center.y +SNAP_X +imgH/2+CAMERA_X ;
            squareCenter.y =squareSize/2;
         //   cout << "dist"<<distToCenter<<endl;;
            
           // if(distToCenter<200){
            Cube *cube =new Cube();
            cube->distFactor =distToCenter;
            cube->angle = angle*180/3.1415;
                        cube->center =squareCenter;
            cube->size = squareSize;
            cubes.push_back(cube);
            //}
        }
		mTextureResult = gl::Texture( fromOcv( input) );
    
    }
    int id;
    float posZ;
    Surface8u        mSurface;
    gl::TextureRef		mTexture;
     gl::Texture		mTextureResult;
    cv::Mat * map1;
    cv::Mat *map2;
    
    
   double findAngle( cv::Point pt1, cv::Point pt2, cv::Point pt0 )
    {
        double dx1 = pt1.x - pt0.x;
        double dy1 = pt1.y - pt0.y;
        double dx2 = pt2.x - pt0.x;
        double dy2 = pt2.y - pt0.y;
        return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
    }
    
    void draw()
    {
        
        
        
        gl::pushMatrices();
    gl::translate(Vec3f(imgH/2 +SNAP_X+CAMERA_X,0,0));
        
        gl::color(1.f,1.f,1.f,1.0f);
        
        gl::translate(Vec3f(0,0,posZ+imgW/2));
        gl::rotate(Vec3f(90,180,-90));
      
        gl::draw( mTextureResult );
        gl::popMatrices();
    
    }

};

#endif
