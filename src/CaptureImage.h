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
    
        scale=700.0f/640.0f;
    
    }
     vector<Cube *>  cubes;
    float scale;
    void calculate()
    {
        string r = "image" +toString(id)+".png";
    writeImage( r, mSurface );
    
        cv::Mat input( toOcv( mSurface) ), output;
       // remap(input, output, *map1, *map2,cv::INTER_LINEAR);
        
        
        
        
        
        int iLowH =35;
        int iHighH =37;
        
        int iLowS = 0;
        int iHighS = 255;
        
        int iLowV = 0;
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
            if( approx.size() == 4 &&
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
           // const cv::Point* p = &squares[i][0];
            //int n = (int)squares[i].size();
           // cv::polylines(input, &p, &n, 1, true, cv::Scalar(0,0,255), 3);
            
            Vec2f squareVector;
            squareVector.x =squares[i][0].x -squares[i][1].x;
            squareVector.y =squares[i][0].y -squares[i][1].y;
            float squareSize =squareVector.length()*scale;
            float angle =atan2(squareVector.y,squareVector.x);
            
            Vec2f center;
            center.x =(squares[i][0].x +squares[i][1].x +squares[i][2].x +squares[i][3].x)/4;
              center.y =(squares[i][0].y +squares[i][1].y +squares[i][2].y +squares[i][3].y)/4;
            
            Vec2f distVec;
            distVec.x  = center.x -(640/2);
            distVec.y  = center.y -(480/2);
           
            float distToCenter = distVec.length();
           // cout <<center<<"  -----------   " <<distToCenter<<endl;;
            Vec3f squareCenter;
            squareCenter.z =-center.x    *scale +posZ+350;
            squareCenter.x =-center.y * scale+SNAP_X ;
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
    gl::translate(Vec3f(730,0,0));
        
        
        
        gl::translate(Vec3f(0,0,posZ+350));
        gl::rotate(Vec3f(90,180,-90));
        gl::scale(scale,scale,scale);
        gl::draw( mTextureResult );
        gl::popMatrices();
    
    }

};

#endif
