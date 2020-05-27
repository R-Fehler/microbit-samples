#ifndef MICROBIT_SAMPLES_EXERCISE_SET_H
#define MICROBIT_SAMPLES_EXERCISE_SET_H

#include "MicroBitEvent.h"
#include "MicroBitUARTService.h"
#include "Microbit.h"
#include "TwoDigitDisplay.h"
#include "myInput.h"
#define DPTIMEUNTILFIRSTREP 300
#define MAX_FILTER_REDUCTION_CONSTANT 3
//#define  ACCELOREMETERSERIALOUTPUT
extern MicroBit uBit;
extern MicroBitUARTService *uart;

class ExerciseSet{
public:
uint16_t weight;
uint8_t reps;
bool isDone;
ExerciseSet();
ExerciseSet(int w, int r);
    void updateReps(){
//    MicroBitEvent evt(SetRepsID,reps);
    }
    void updateWeight(){
//    MicroBitEvent evt(SetWeightID,weight);
    }
    void updateIsDone(){
//    MicroBitEvent evt(SetIsDoneID,isDone);
    }
    void cntReps(myInput& input, uint32_t threshold=1300000);

private:
    Sample3D accel;
    Sample3D accelbuff;
    uint32_t buffer;

    uint32_t lowpass;




    uint32_t myForce(const Sample3D& sample) {
        return (uint32_t)sample.x*(uint32_t)sample.x + (uint32_t)sample.y*(uint32_t)sample.y + (uint32_t)sample.z*(uint32_t)sample.z;
    }

    uint32_t maxC(uint32_t a, uint32_t b) {
        return (a <= b) ? b : a;
    }

    uint32_t minC(uint32_t a, uint32_t b) {
        return (a >= b) ? b : a;
    }

};



#endif // !MICROBIT_SAMPLES_EXERCISE_SET_H