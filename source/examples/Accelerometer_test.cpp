/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "MicroBit.h"
#include <math.h>
MicroBit uBit;
//
// Scales the given value that is in the -1024 to 1024 range
// int a value between 0 and 4.
//
ManagedString delimiter("\r");

void RxFull(MicroBitEvent){
    uBit.display.print("F");
	ManagedString rxbuff(uBit.serial.getRxBufferSize());
	uBit.serial.send("Alte Buffer Größe:"+rxbuff);
	//uBit.serial.readUntil(delimiter);
	uBit.serial.clearRxBuffer();
	uBit.serial.setRxBufferSize(uBit.serial.getRxBufferSize()+16);
	uBit.display.clear();
	
		
    }
void Hallo(MicroBitEvent){
    uBit.display.scroll("HALLO");
    
}  
	
    
int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

        MicroBitPin P0(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_ALL);

        uBit.messageBus.listen(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_RX_FULL, RxFull);
    //    uBit.messageBus.listen(MICROBIT_ID_IO_P0, MICROBIT_BUTTON_EVT_CLICK, Hallo);
    //
    // Periodically read the accelerometer x and y values, and plot a 
    // scaled version of this ont the display. 
    //
    while(1)
    {   
		uBit.accelerometer.setRange(8);
        int raw_x = uBit.accelerometer.getX();
        double acc_x = double(raw_x);
        double acc_y = double(uBit.accelerometer.getY());
        double acc_z = double(uBit.accelerometer.getZ());

        long double  acc_sum = sqrt(acc_x*acc_x + acc_y*acc_y + acc_z*acc_z);
        int     acc_sum_round = int(round(acc_sum));
        int acc_x_round = int (acc_x);

        ManagedString s(acc_sum_round);
        ManagedString s5(raw_x);
        // ManagedString s2("Beschleunigung:");
         ManagedString s3("\n");
        // ManagedString s4("ECHO");
        uBit.serial.send("Summe:"+s+s3+"X:"+s5+s3);
 //       uBit.serial.send(s);
 //       uBit.serial.send(s3);
 //       uBit.display.image.clear();
 //       uBit.display.scroll(s);
            if(P0.isTouched()){
            uBit.display.print("if");

            }

			uBit.sleep(50);
		}
       
        
        // ManagedString r (uBit.serial.readUntil(delimiter,ASYNC));
        // uBit.serial.send(s4+r);
        
    }


