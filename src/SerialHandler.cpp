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
            console() << "Device: " << deviceIt->getName() << endl;
        }
	}
    
}
bool SerialHandler::sendHoming()
{
    if(!isDone)return false;
    if(!isonline)return true;
    
   // isDone =false;
    
    
    serial.writeByte(0x00);
    
    
    
    
    
    
    serial.writeByte(0xff);
    return true;
}
bool SerialHandler::sendPositions()
{
    if(!isDone)return false;
    if(!isonline)return true;
    
    //isDone =false;
    
    serial.writeByte(0x01);
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
		
		
        console() <<"input"<< (int)b << endl;
        isDone =true;
		//serial.flush();
	}
    
    
    
}