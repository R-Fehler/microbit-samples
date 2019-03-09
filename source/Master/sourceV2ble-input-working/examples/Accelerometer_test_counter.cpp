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

MicroBit uBit;
int reps = 0;
//
// Scales the given value that is in the -1024 to 1024 range
// int a value between 0 and 4.
//
ManagedString delimiter("\r");
ManagedString countString("255");
uint8_t count_arr[] {
                          0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, };    
void counter(MicroBitEvent e){
    count_arr[reps] = 1;
    reps = reps + 1;
    uBit.display.clear();
    uBit.display.scroll(reps);
    uBit.sleep(500);
    uBit.display.clear();
 //  char string[countString.length()] = countString.toCharArray();

 //   MicroBitImage img(string);
 //   uBit.serial.printf(string);
 //   uBit.display.print(img);
    if(reps % 5){
        countString = countString + ",255";
    }
    else{
        countString = countString + ",255\n";
    }
    MicroBitImage img(5,5,count_arr);
    uBit.display.print(img);
    uBit.sleep(500);
}  
	
void Hallo2(MicroBitEvent e){
    uBit.display.print("B");
    uBit.sleep(500);
    
}   
int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
        uBit.accelerometer.setRange(8);
        uBit.display.print("hi");
        uBit.display.setBrightness(2);
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_ACCELEROMETER_EVT_6G, counter);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A,MICROBIT_BUTTON_EVT_CLICK, Hallo2);

    //
    // Periodically read the accelerometer x and y values, and plot a 
    // scaled version of this ont the display. 
    //
    while(1){
        uBit.sleep(10000);

    }
    

			
		
       
    
        // ManagedString r (uBit.serial.readUntil(delimiter,ASYNC));
        // uBit.serial.send(s4+r);
}       
    


