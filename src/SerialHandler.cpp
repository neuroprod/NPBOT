//
//  SerialHandler.cpp
//  NPBOT
//
//  Created by Kris Temmerman on 16/09/14.
//
//

#include "SerialHandler.h"



void SerialHandler::setup()
{
    isDone =true;
  
	///////// MAIN
	try {
		Serial::Device dev = Serial::findDeviceByNameContains("tty.usbmodem1411");
		serial = Serial( dev, 115200);
        console() << "Serial Connected" << std::endl;
        isonline =true;
	}
	catch( ... ) {
		console() << "There was an error initializing the serial device!" << std::endl;
		//exit( -1 );
        isonline =false;
        
        const vector<Serial::Device> &devices( Serial::getDevices() );
        for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
            console() << "Device for MAIN?: " << deviceIt->getName() << endl;
        }
	}
    
    
    
    
    
    //////////// HAND
    try {
		Serial::Device dev = Serial::findDeviceByNameContains("tty.usbserial-14P52911");
		serialHand = Serial( dev, 9600);
        console() << "Serial Hand Connected" << std::endl;
        handisonline =true;
	}
	catch( ... ) {
		console() << "There was an error initializing the serial device Hand!" << std::endl;
		//exit( -1 );
        handisonline =false;
        
        const vector<Serial::Device> &devices( Serial::getDevices() );
        for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
            console() << "Device For HAND?: " << deviceIt->getName() << endl;
        }
	}
    
    
}


//hand
bool SerialHandler::sendHandRelease()
{
    
    if(!handisonline )return true;
    
    
    console()<<"hand release"<<endl;
    serialHand.writeByte(2);
   
    
    
    
    
    
    serialHand.writeByte(0xff);
    return true;
}
bool SerialHandler::sendHandGrab()
{
    
    if(!handisonline )return true;
     console()<<"hand grab"<<endl;
    
    
    serialHand.writeByte(1);
    serialHand.writeByte(60);
    serialHand.writeByte(100);
    
    
    
    
    
    serialHand.writeByte(0xff);
    return true;
}
bool SerialHandler::sendHandPos(int pos1,int pos2)
{
    
    if(!handisonline )return true;
     console()<<"hand set"<<endl;
    
    
     serialHand.writeByte(3);
    serialHand.writeByte(pos1);
    serialHand.writeByte(pos2);
    
    
    
    
    
    serialHand.writeByte(0xff);
    return true;
}


void SerialHandler::updateHand()
{
    if(!handisonline)return ;
    if (serialHand.getNumBytesAvailable()>0)
	{
		uint8 b;
		try
        {
            b = serialHand.readByte();
		}
        catch(SerialTimeoutExc e)
        {
			console() << "timeout" << endl;
		}
        console() <<"input byte"<< (int)b << endl;
    }
    
    
    
}




////////// main
bool SerialHandler::sendHoming()
{
    if(!isDone)return false;
    if(!isonline)return false;
    
    isDone =false;
    
    
    serial.writeByte(0x00);
    
    
    
    
    
    
    serial.writeByte(0xff);
    return true;
}
bool SerialHandler::sendPositions()
{
    if(!isDone)return false;
    if(!isonline)return false;
    
    isDone =false;
    
    serial.writeByte(0x01);
    //command.write(&serial );
    
    for(int i=0;i<axisData.size();i++)
    {
        writeInt(axisData[i]->targetStep);
    
    }
    
    
    
    
    serial.writeByte(0xff);
    return true;
}

bool SerialHandler::setPositions()
{
    //if(!isDone)return false;
    if(!isonline)return true;
    
    //isDone =false;
    
    serial.writeByte(0x02);
    //command.write(&serial );
    
    for(int i=0;i<axisData.size();i++)
    {
        writeInt(axisData[i]->targetStep);
        
    }
    
    
    
    
    serial.writeByte(0xff);
    return true;
}


void SerialHandler::writeInt (int val)
{
    if(val<0)
    {
     throw std::invalid_argument( "received negative value" );
    
    }
    ci::app::console()<<"sendVal"<<val<<std::endl;
    float workVal = (float) val;
    int val1 = floor(workVal/10000);
    workVal -= val1*10000;
    int val2 = floor(workVal/100);
    workVal -= val2*100;
    
    int val3 = workVal;
    serial.writeByte((uint8_t)val1);
    serial.writeByte((uint8_t)val2);
    serial.writeByte((uint8_t)val3);
    
}

void SerialHandler::update()
{
    if(!isonline)return ;
    if (serial.getNumBytesAvailable()>0)
	{
		uint8 b;
		try{
			
            b = serial.readByte();
            
		} catch(SerialTimeoutExc e) {
			console() << "timeout" << endl;
		}
		console() <<"input byte"<< (int)b << endl;
		if(b==4)
        {
        
            console() <<"positioning done" << endl;
        
        }
       
        else if(b==5)
        {
            console() <<"homing place done"<< (int)b << endl;
            
            for(int i=0;i<axisData.size();i++)
            {
              // axisData[i]->targetStep =0;
               // axisData[i]->currentStep =0;
                //axisData[i]->isDirty =true;
                axisData[i]->setHome();
            }
            setPositions();
        }
        else if(b==6){
            console() <<"homing  done" << endl;
            
                    }
        isDone =true;
		//serial.flush();
	}
    
    
    
}