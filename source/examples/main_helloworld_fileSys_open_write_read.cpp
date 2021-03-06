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
//#include "MicroBitFile.h"
#include "MicroBitFileSystem.h"
MicroBit uBit;

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    ManagedString fName("Test.txt");
    ManagedString debug("debug");

//    uBit.addSystemComponent(file);
    // Insert your code here!
    uBit.display.scroll(":)");
    ManagedString str("Hallo das ist super!");
    uBit.serial.send(fName);
    //MicroBitFile file(fName,READ_AND_WRITE);
    
    uBit.serial.send(debug);
    uBit.display.scroll("k1");

    MicroBitFileSystem f;
    int fd = f.open("test.txt", MB_WRITE|MB_CREAT);
    if(fd<0){
    uBit.serial.send("file open error");

    }
    unsigned char buf[5] = {'H','a','l','l','o'};
    if(f.write(fd,buf, 5) != 5){
    uBit.serial.send("error writing");
    }
    uBit.display.scroll("l2");
//    int app_return = f.append("Hallo",5);
    uBit.display.scroll("m3");
    
    uBit.serial.send(" open_return_code\n");
    uBit.serial.send(fd);
    uBit.serial.send("\n");
    
    f.seek(fd, -5, MB_SEEK_END);
    
    unsigned char read_buff[5];
    int read_return = f.read(fd,read_buff,5);
    uBit.serial.send("read_return:"+ read_return);
    uBit.serial.send("READINHALT:\n"); 
    unsigned char testArr[5] = {'t','e','s','t','i'} ;
    for (int i=0; i<5;i++){
    uBit.serial.sendChar(read_buff[i]);

    }
    f.close(fd);

    

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}

