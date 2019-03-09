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
    #include "MicroBitFile.h"
    //#include "MicroBitFileSystem.h"
    MicroBit uBit;

    int main()
    {
        // Initialise the micro:bit runtime.
        uBit.init();
        ManagedString delimiters("\n\r\t");
        uBit.serial.send("Wie soll die Datei genannt werden? (1 Zeichen)\n");
        ManagedString fName(uBit.serial.readUntil(delimiters));
        ManagedString debug("debug");
    //    uBit.addSystemComponent(file);
        // Insert your code here!
        uBit.display.scroll("!");
        ManagedString str("Hallo das ist super!");
        uBit.serial.send(fName+"\n");
        MicroBitFile file("Test.txt",CREATE);
    //   int test = file.append(debug);
        
        int fd = file.getHandle(); // return 0 wenn MICROBIT_OK
        uBit.serial.send(fd);
        uBit.serial.send("____fileHandle\n");
    //    uBit.serial.send(test);
        uBit.serial.send("______\n");

        uBit.serial.send("FilePosition:");
        uBit.serial.send(file.getPosition());

        int i=0;
        int acc_len=0;
        ManagedString delimit("\n");
        ManagedString tab("\t");
        
        while (i<50){
        int x_acc = uBit.accelerometer.getX();
        ManagedString x_acc_str(x_acc);
        ManagedString i_str(i);
        uBit.serial.send("Sensordaten:\t"); 
        uBit.serial.send(i);
        uBit.serial.send("___");
        uBit.serial.send(x_acc);
        uBit.serial.send("\n");
        acc_len = acc_len + file.append(i_str);
        acc_len = acc_len + file.append(tab);
        acc_len = acc_len + file.append(x_acc_str);
        acc_len = acc_len + file.append(delimit);
        i = i+1;
        uBit.serial.send("FilePosition:");
        uBit.serial.send(file.getPosition());
        uBit.serial.send("\n");
        uBit.serial.send("Laenge der geschriebenen Datei:");
        uBit.serial.send(acc_len);
        uBit.serial.send(delimit);
        uBit.sleep(1);
        
        }

        int FilePosition_end = file.getPosition(); //merkt sich die Position nach dem letzten Schreibvorgang (zb wenn nach restart eine zweite Datei geschrieben wird)
        uBit.serial.send("FileHandle: ");
        uBit.serial.send(file.getHandle());
        uBit.serial.send("\n)");
        file.setPosition(0);
    //    uBit.serial.send(acc_len);
        ManagedString readString(file.read(FilePosition_end));
        uBit.serial.send(readString);
        
        file.close();

        uBit.serial.send(file.getHandle());
        uBit.serial.send("\n)");

        // If main exits, there may still be other fibers running or registered event handlers etc.
        // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
        // sit in the idle task forever, in a power efficient sleep.
        release_fiber();
    }

