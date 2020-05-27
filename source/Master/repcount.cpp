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
uint32_t mincomp(uint32_t a, uint32_t b) {
	return (a >= b) ? b : a;
}


void initrepcount() {
	uBit.accelerometer.setRange(4);
	uBit.accelerometer.setPeriod(20);
}
int repcount(int& input,int& inputterm,int& goback, int& noreps_time_till_next , uint32_t threshold) {
	//ToDo Calibration reps only for first set of exersice id //warmup
	int dptime = DPTIMEUNTILFIRSTREP;
	const int sizefilter = 20;
	const int max_filter_div = 4;
	const int max_sizefilter = sizefilter / max_filter_div;
	uint32_t threshold_start = threshold;
	uint32_t filterbuf[sizefilter] = { 0 };
	uint32_t i = 0;
	uint32_t sum = 0;
	uint32_t res = 0;
	uint32_t max[max_sizefilter] = { threshold };
	uint32_t maxaverage = threshold;
	uint32_t sum_max = 0;
	//TODO : Testen ob die ganzen uint auf dem heap alocated werden und nicht gefreed
	int multi = 9;
	int divide = 10;
	const int noreps_threshold = 1200000;
	const int noreps_dp = 250;
	int norepcnt = 0;
	bool rep = false;
	int repcnt = 0;
	//max times 5/4;
	uBit.display.printAsync(twodigit.createImage(repcnt));
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
#ifdef  ACCELOREMETERSERIALOUTPUT
		uBit.serial.printf("%d\n", res);
#endif
		max[i%max_sizefilter] = maxcomp(max[i%max_sizefilter], res);
		//uBit.serial.printf("%d\n", max);
		sum_max = 0;
		for (size_t j = 0; j < sizeof(max) / sizeof(uint32_t); j++)
		{
			sum_max += max[j];
		}
		maxaverage = (sum_max / max_sizefilter);
		if (i%(max_sizefilter*max_filter_div*MAX_FILTER_REDUCTION_CONSTANT)==0) {
			max[i%max_sizefilter] = threshold_start;
		}



		threshold = maxcomp(threshold, (maxaverage * multi / divide));
	/*	uBit.serial.printf("maxaverage: %d\n", maxaverage);
		uBit.serial.printf("res:        %d\n", res);
		uBit.serial.printf("threshold:  %d\n", threshold);
*/
		//uBit.serial.printf("threshold: %d\n", threshold);
		//uBit.serial.printf("res:       %d\n", res);
		if (input != 0) {
			repcnt = repcnt + (input);
			repcnt = abs(repcnt);
			uBit.serial.printf("rpcnt:%d", repcnt);
			uBit.display.printAsync(twodigit.createImage(repcnt));
			norepcnt = 0;
			input = 0;
            uart->send(ManagedString(repcnt));//TODO stattdessen evt()
        }
		if (res > threshold) {
			if (rep == false) {
				rep = true;
				repcnt++;
				uBit.serial.printf("\t\t REEEEEP");
				uBit.display.printAsync(twodigit.createImage(repcnt));
				uart->send(ManagedString(repcnt));
				norepcnt = 0;
			}

		}
		if (res < threshold) {
			if (res<noreps_threshold) {
				norepcnt++;
			}
			
			if (rep == true) {
				rep = false;
			}
		}
		uBit.sleep(20);
		if (norepcnt > noreps_dp) {

			uBit.display.clear();
		}
			

		if (goback != 0) {
			return repcnt;
			uBit.io.P1.setDigitalValue(0);
			uBit.io.P8.setDigitalValue(0);
		}
		if (norepcnt >= noreps_time_till_next or 1 == inputterm) {
			inputterm = 0;
			uBit.io.P1.setDigitalValue(0);
			uBit.io.P8.setDigitalValue(0);
			if (norepcnt >= noreps_time_till_next) {

			uBit.display.printChar('|');
			for (size_t i = 0; i < 10; i++) //TODO Magic 10 entfernen
			{

			uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_90);
			uBit.sleep(500);
			uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_0);
			uBit.sleep(500);
			}
			uBit.display.clear();
			}
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
