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

#include "repcount.h"

Sample3D accel;
Sample3D accelbuff;
uint32_t buffer;

uint32_t lowpass;

uint32_t force(const Sample3D& sample) {
	return (uint32_t)sample.x*(uint32_t)sample.x + (uint32_t)sample.y*(uint32_t)sample.y + (uint32_t)sample.z*(uint32_t)sample.z;
}
uint32_t maxcomp(uint32_t a, uint32_t b) {
	return (a <= b) ? b : a;
}

void initrepcount() {
	uBit.accelerometer.setRange(4);
	uBit.accelerometer.setPeriod(20);
}
int repcount(int& input,int& inputterm, uint32_t threshold) {
	//ToDo Calibration reps only for first set of exersice id //warmup

	const int sizefilter = 20;
	uint32_t filterbuf[sizefilter] = { 0 };
	uint32_t i = 0;
	uint32_t sum = 0;
	uint32_t res = 0;
	uint32_t max = 0;
	int multi = 4;
	int divide = 5;
	const int noreps_threshold = 1200000;
	const int nr_noreps = 750;
	int norepcnt = 0;
	bool rep = false;
	int repcnt = 0;
	//max times 5/4;
	while (true)
	{

		accel = uBit.accelerometer.getSample();
		filterbuf[i%sizefilter] = force(accel);
		sum = 0;
		for (size_t j = 0; j < sizeof(filterbuf) / sizeof(uint32_t); j++)
		{
			sum += filterbuf[j];
		}
		//accelbuff = uBit.accelerometer.getSample();
		//buffer = force(accelbuff);

	   // lowpass += (buffer - lowpass) / alpha;
		res = (sum / sizefilter);
		max = maxcomp(max, res);
		//uBit.serial.printf("%d\n", max);
		threshold = maxcomp(threshold, (max * multi / divide));
		//uBit.serial.printf("threshold: %d\n", threshold);
		//uBit.serial.printf("res:       %d\n", res);
		if (input != 0) {
			repcnt = repcnt + (input);
			uBit.serial.printf("rpcnt:%d", repcnt);
			uBit.display.printAsync(twodigit.createImage(repcnt));
			input = 0;
		}
		if (res > threshold) {
			if (rep == false) {
				rep = true;
				repcnt++;
				uBit.serial.printf("REEEEEP: %d\n", repcnt);
				uBit.display.printAsync(twodigit.createImage(repcnt));
				
			}

		}
		if (res < threshold) {
			if (repcnt >= 1 and res<noreps_threshold) {
				norepcnt++;
			}
			
			if (rep == true) {
				rep = false;
			}
		}
		uBit.sleep(20);
		if (norepcnt >= nr_noreps and 1 == inputterm) {
			inputterm = 0;
			return repcnt;
		}
		i++;

	}
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
