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
Sample3D speed;
Sample3D position;
int const buffersize = 10;
Sample3D buffer;
Sample3D calib;
Sample3D lowpass;
/////////////////
int alpha = 500;  // tiefpass filter daempfungsfaktor
///////////////////// Gravitation muss kalibriert werden
int index;
//
//   lowpass += alpha * (buffer-lowpass);

int main() {
  // Initialise the micro:bit runtime.
  uBit.init();
  uBit.accelerometer.setRange(4);
  uBit.accelerometer.setPeriod(1);
  uBit.serial.printf("Range;%d\t Period;%d\t", uBit.accelerometer.getRange(),
                     uBit.accelerometer.getPeriod());
  lowpass = uBit.accelerometer.getSample();
  while (1) {
    buffer = uBit.accelerometer.getSample();
    lowpass.z += (buffer.z - lowpass.z) / alpha;
    uBit.serial.printf("%d\n", lowpass.z);
    uBit.sleep(1);
  }

  ////diese impl. ist bullshit. lerne Mathe bzw regelungstechnik!
  /// tiefpassgleichung umgestellt:
  //   y += alpha * (x-y); mit y output und x input

  // for (int i = 0; i < buffersize; i++) {
  //   buffer[i] = Sample3D();
  // }
  // while (1) {
  //   index = 0;
  //   // if (index % 100 == 0) {
  //   //   calib.x = uBit.accelerometer.getX();
  //   //   calib.y = uBit.accelerometer.getY();
  //   //   calib.z = uBit.accelerometer.getZ();
  //   // }
  //   buffer[index % buffersize] = uBit.accelerometer.getSample();
  //   // speed.x = speed.x + buffer[index % 10].x;
  //   // speed.y = speed.y + buffer[index % 10].y;
  //   // speed.z = speed.z + buffer[index % 10].z;
  //   if
  //     for (int i = 0; i < buffersize; i++) {
  //       lowpass = lowpass + buffer[buffersize];
  //     }

  //   uBit.serial.printf("%d\t %d\t %d\t \n", lowpass.x / buffersize,
  //                      lowpass.y / buffersize, lowpass.z / buffersize);
  //   uBit.sleep(1);
  //   index++;
}
//
// Periodically read the accelerometer x and y values, and plot a
// scaled version of this ont the display.
//
