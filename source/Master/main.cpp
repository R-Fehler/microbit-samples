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

// Works with the Animal Vegetable Mineral guessing game in the Android 'micro:bit Blue' app which is obtainable from
// https://play.google.com/store/apps/details?id=com.bluetooth.mwoolley.microbitbledemo

#include "MicroBit.h"
#include "MicroBitUARTService.h"
#include "Training.h"
#include "sin1.h"
/**
 * Q is a binary fixed point number format where the number of fractional bits (and optionally the number of integer bits)
 * is specified. For example, a Q15 number has 15 fractional bits
 */
#define Q15 (1.0/(double)((1<<15)-1))

MicroBit uBit;
// MicroBitUARTService *uart;


 int connected = 0;

 void onBLEChanged(MicroBitEvent e)
 {if(e.value==MICROBIT_BLE_EVT_CONNECTED){
     connected = 1;
     uBit.display.scroll("C");
 }
 if(e.value==MICROBIT_BLE_EVT_DISCONNECTED){
     connected=0;
     uBit.display.scroll("D");
 }
 }

void Rotate_X_MatrixfromAlpha(int A[9],int16_t alpha){
     A[0]=1;
     A[1]=0;
     A[2]=0;
     A[3]=0;
     A[4]=cos1(alpha);
     A[5]=sin1(alpha);
     A[6]=0;
     A[7]=-sin1(alpha);
     A[8]=cos1(alpha);
 }
/**
 * Ax=y
 * @param A [in]
 * @param x [in]
 * @param y [out]
 */
void M_v_Multi(const int A[9], const int x[3], int y[3])
{
    int i0;
    for (i0 = 0; i0 < 3; i0++) {
        y[i0] = 0.0;
        y[i0] = (A[i0] * x[0] + A[i0 + 3] * x[1]) + A[i0 + 6] * x[2];
    }
}

// prüfe ob das alpha dann max oder min ist mit ddfdxx <0 bedingung
// falls min dann phasenverschiebung um pi bzw int16 size/4;
double Newton(int (*f)(int), int (*dfdx)(int), int x0) {
    int xneu = x0;
    int xalt = 0;
    int delta = 5;
    int i = 0;
    while (delta <= fabs(xneu - xalt)) {
        i++;
        xalt = xneu;
        xneu = xalt - f(xalt) / dfdx(xalt);
        if (i >= 200) {
            break;
        }
    }
    return xneu;
}

int16_t findAlpha(){

}
int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
//    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_EVT_ANY, onBLEChanged);
    // Note GATT table size increased from default in MicroBitConfig.h
    // #define MICROBIT_SD_GATT_TABLE_SIZE             0x500
//     uart= new MicroBitUARTService(*uBit.ble, 32, 32);
//    Training training;
//    Exercise e;
//    e.custom_id=14;
//    training.exercises.push_back(e);
//    e.custom_id=19;
//    training.exercises.push_back(e);
//    training.inputLoopOverExercises();


    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
