//
//  CameraHandler.h
//  NPBOT
//
//  Created by Kris Temmerman on 13/10/14.
//
//

#ifndef NPBOT_CameraHandler_h
#define NPBOT_CameraHandler_h

class CameraHandler
{

public:
    CameraHandler()
    {
    
    
    }
    
    void setup()
    {
        hasCamera =false;
        for( auto device = Capture::getDevices().begin(); device != Capture::getDevices().end(); ++device ) {
            console() << "Device: " << (*device)->getName() << " "<< std::endl;
           if((*device)->getName().find("Logitech Camera") !=string::npos)
           //     if((*device)->getName().find("FaceTime HD Camera") !=string::npos)
            {
                try {
                    mCapture = Capture::create( 640, 480,(*device) );
                    mCapture->start();
                    hasCamera =true;
                
                
                }
                catch( ... ) {
                    console() << "Failed to initialize capture" << std::endl;
                }
                
            }
            
        }

    }
    void update()
    {
        if( mCapture && mCapture->checkNewFrame() ) {
            
            mSurface =mCapture->getSurface();
            mTexture = gl::Texture::create( mSurface );
        }
    
    }
    bool hasCamera;
    CaptureRef			mCapture;
    Surface8u        mSurface;
    gl::TextureRef		mTexture;
};

#endif
