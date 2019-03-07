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
class TwoDigitDisplay {
 public:
  const uint8_t ze[10][25]{
      {250, 250, 0,   0,   0, 250, 250, 0,   0,   0, 250, 250, 0,
       0,   0,   250, 250, 0, 0,   0,   250, 250, 0, 0,   0},  // 0
      {0, 250, 0, 0,   0, 0, 250, 0, 0,   0, 0, 250, 0,
       0, 0,   0, 250, 0, 0, 0,   0, 250, 0, 0, 0},  // 250
      {250, 250, 0,   0, 0, 0, 250, 0,   0,   0, 250, 250, 0,
       0,   0,   250, 0, 0, 0, 0,   250, 250, 0, 0,   0},  // 2
      {250, 250, 0, 0,   0, 0, 250, 0,   0,   0, 250, 250, 0,
       0,   0,   0, 250, 0, 0, 0,   250, 250, 0, 0,   0},  // 3
      {250, 0, 0, 0,   0, 250, 0, 0, 0,   0, 250, 250, 0,
       0,   0, 0, 250, 0, 0,   0, 0, 250, 0, 0,   0},  // 4
      {250, 250, 0, 0,   0, 250, 0, 0,   0,   0, 250, 250, 0,
       0,   0,   0, 250, 0, 0,   0, 250, 250, 0, 0,   0},  // 5
      {250, 0, 0,   0,   0, 250, 0, 0,   0,   0, 250, 250, 0,
       0,   0, 250, 250, 0, 0,   0, 250, 250, 0, 0,   0},  // 6
      {250, 250, 0, 0,   0, 0, 250, 0, 0,   0, 0, 250, 0,
       0,   0,   0, 250, 0, 0, 0,   0, 250, 0, 0, 0},  // 7
      {250, 250, 0,   0,   0, 250, 250, 0,   0,   0, 0, 0, 0,
       0,   0,   250, 250, 0, 0,   0,   250, 250, 0, 0, 0},  // 8
      {250, 250, 0, 0,   0, 250, 250, 0, 0,   0, 250, 250, 0,
       0,   0,   0, 250, 0, 0,   0,   0, 250, 0, 0,   0},  // 9
  };

  const uint8_t ei[10][25]{
      {
          0,   0,   0, 250, 250, 0,   0,   0, 250, 250, 0,   0,   0,
          250, 250, 0, 0,   0,   250, 250, 0, 0,   0,   250, 250,
      },  // 0
      {
          0, 0,   0, 0, 250, 0, 0,   0, 0, 250, 0, 0,   0,
          0, 250, 0, 0, 0,   0, 250, 0, 0, 0,   0, 250,
      },  // 01
      {0,   0,   0, 250, 250, 0,   0, 0, 0, 250, 0,   0,  0,
       250, 250, 0, 0,   0,   250, 0, 0, 0, 0,   250, 250},  // 2
      {
          0,   0,   0, 250, 250, 0, 0,   0, 0, 250, 0,   0,   0,
          250, 250, 0, 0,   0,   0, 250, 0, 0, 0,   250, 250,
      },  // 3
      {0,   0,   0, 250, 0, 0, 0,   0, 250, 0, 0, 0,  0,
       250, 250, 0, 0,   0, 0, 250, 0, 0,   0, 0, 250},  // 4
      {0,   0,   0, 250, 250, 0, 0,   0, 250, 0, 0,   0,  0,
       250, 250, 0, 0,   0,   0, 250, 0, 0,   0, 250, 250},  // 5
      {0,   0,   0, 250, 0, 0,   0,   0, 250, 0, 0,   0,  0,
       250, 250, 0, 0,   0, 250, 250, 0, 0,   0, 250, 250},  // 6
      {0, 0,   0, 250, 250, 0, 0,   0, 0, 250, 0, 0,  0,
       0, 250, 0, 0,   0,   0, 250, 0, 0, 0,   0, 250},  // 7
      {
          0, 0, 0, 250, 250, 0,   0,   0, 250, 250, 0,   0,   0,
          0, 0, 0, 0,   0,   250, 250, 0, 0,   0,   250, 250,
      },  // 8
      {
          0,   0,   0, 250, 250, 0, 0,   0, 250, 250, 0, 0,   0,
          250, 250, 0, 0,   0,   0, 250, 0, 0,   0,   0, 250,
      },  // 9
  };

  MicroBitImage createImage(int wert);
  TwoDigitDisplay();
  ~TwoDigitDisplay();

 private:
};

// void TwoDigitDisplay::printTwoDigit(int wert,MicroBitImage*
// imgs_ze,MicroBitImage* imgs_ei);

TwoDigitDisplay::TwoDigitDisplay() {
  // uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);
  //   for (int i = 0; i < 10; i++) {
  //     imgs_ze[i] = MicroBitImage(5, 5, ze[i]);
  //     imgs_ei[i] = MicroBitImage(5, 5, ei[i]);
  //   }
}
MicroBitImage TwoDigitDisplay::createImage(int wert) {
  MicroBitImage ergebnis(5, 5);
  ergebnis.paste(MicroBitImage(5, 5, ze[wert / 10 % 10]), 0, 0, 1);
  ergebnis.paste(MicroBitImage(5, 5, ei[wert % 10]), 0, 0, 1);
  // uBit.display.print(ergebnis);
  return ergebnis;
}
TwoDigitDisplay::~TwoDigitDisplay() {}

int main() {
  // Initialise the micro:bit runtime.
  uBit.init();
  uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);

  TwoDigitDisplay twod;
  uBit.display.print(MicroBitImage(twod.createImage(12)));
  uBit.sleep(1000);
  uBit.display.clear();
  // Insert your code here!
  uBit.display.scroll("HELLO WORLD! :)");

  // If main exits, there may still be other fibers running or registered event
  // handlers etc. Simply release this fiber, which will mean we enter the
  // scheduler. Worse case, we then sit in the idle task forever, in a power
  // efficient sleep.
  release_fiber();
}
