#ifndef REPCOUNT
#define REPCOUNT
#include "Microbit.h"
#include "TwoDigitDisplay.h"
#include "MicroBitUARTService.h"
#define DPTIMEUNTILFIRSTREP 300
#define MAX_FILTER_REDUCTION_CONSTANT 3
//#define  ACCELOREMETERSERIALOUTPUT
extern MicroBit uBit;
extern MicroBitUARTService *uart;
uint32_t force(const Sample3D& sample);
uint32_t maxcomp(uint32_t a, uint32_t b);

void initrepcount();
int repcount(int & input, int & inputterm, int & goback, int & noreps_time_till_next, uint32_t threshold = 1300000);
#endif // !REPCOUNT
