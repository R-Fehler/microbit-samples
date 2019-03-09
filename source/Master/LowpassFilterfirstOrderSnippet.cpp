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
Sample3D accel;
Sample3D accelbuff;
uint32_t buffer;

uint32_t lowpass;
/////////////////
int alpha = 10000;  // tiefpass filter daempfungsfaktor
///////////////////// Gravitation muss kalibriert werden
//
//   lowpass += alpha * (buffer-lowpass);
uint32_t force(const Sample3D& sample) {
	return (uint32_t)sample.x*(uint32_t)sample.x + (uint32_t)sample.y*(uint32_t)sample.y + (uint32_t)sample.z*(uint32_t)sample.z;
}
uint32_t maxcomp(uint32_t a, uint32_t b) {
return (a <= b) ? b : a;
}
int main() {
  // Initialise the micro:bit runtime.
  uBit.init();
  uBit.accelerometer.setRange(4);
  uBit.accelerometer.setPeriod(20);
  uBit.serial.printf("Range;%d\t Period;%d\t", uBit.accelerometer.getRange(),
                     uBit.accelerometer.getPeriod());

  const int sizefilter = 20;
  uint32_t filterbuf[sizefilter] = { 0 };
  uint32_t i=0;
  uint32_t sum = 0;
  uint32_t res = 0;
  uint32_t max = 0;
  int multi = 4;
  int divide = 5;
  uint32_t threshold = 1300000;
  bool rep = false;
  int repcnt = 0;
  //max times 5/4;
  while (true)
  {
	  
	  accel = uBit.accelerometer.getSample();
	  filterbuf[i%sizefilter] = force(accel);
	  sum = 0;
	  for (size_t j = 0; j < sizeof(filterbuf)/sizeof(uint32_t); j++)
	  {
		  sum += filterbuf[j];
	  }
	  //accelbuff = uBit.accelerometer.getSample();
	  //buffer = force(accelbuff);
	  
	 // lowpass += (buffer - lowpass) / alpha;
	  res = (sum / sizefilter);
	  max = maxcomp(max, res);
	  //uBit.serial.printf("%d\n", max);
	  threshold = maxcomp(threshold,(max * multi / divide));
	  //uBit.serial.printf("threshold: %d\n", threshold);
	  //uBit.serial.printf("res:       %d\n", res);
	  if (res > threshold) {
		  if (rep == false) {
			  rep = true;
			  repcnt++;
			  uBit.serial.printf("REEEEEP: %d\n", repcnt);
		  }

	  }
	  if (res < threshold) {
		  if (rep == true) {
			  rep = false;
		  }
	  }
	  uBit.sleep(40);
	  i++;
  }
  
 /* while (1) {
    buffer = uBit.accelerometer.getSample();
	lowpass.z += (buffer.z - lowpass.z) / alpha;
	lowpass.x += (buffer.x - lowpass.x) / alpha;
	lowpass.y += (buffer.y - lowpass.y) / alpha;
    uBit.serial.printf("%d\n", lowpass.dSquared(zero));
    uBit.sleep(1);
  }
*/
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
