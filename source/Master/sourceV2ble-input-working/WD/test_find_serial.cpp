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
ManagedString buffer;
ManagedString delimiter("\n");
void fiberfkt() {
  buffer = uBit.serial.readUntil(delimiter);

  fiber_sleep(1000);
}
void test(MicroBitEvent e) {
  ManagedString buf;
  uBit.serial.printf("buf");
  uBit.display.scroll("t");
  // buf = uBit.serial.readUntil(ManagedString("\n"));
}
int main() {
  // Initialise the micro:bit runtime.
  uBit.init();
  uBit.messageBus.listen(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_DELIM_MATCH,
                         test);

  uBit.serial.eventOn(delimiter);
  uBit.sleep(100);
  ManagedString test = uBit.serial.readUntil(ManagedString("\n"));
  uBit.serial.send(test);
  // Insert your code here!

  // uBit.display.scroll("!!");
  create_fiber(fiberfkt);
  while (1) {
    uBit.sleep(1000);
  }
  // If main exits, there may still be other fibers running or registered
  // event handlers etc. Simply release this fiber, which will mean we enter
  // the scheduler. Worse case, we then sit in the idle task forever, in a
  // power efficient sleep.
  release_fiber();
}