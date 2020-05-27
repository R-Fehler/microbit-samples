//
// Created by Richard on 23.04.2020.
//

#ifndef MICROBIT_SAMPLES_TRAINING_H
#define MICROBIT_SAMPLES_TRAINING_H


#include "ManagedString.h"
#include "MicroBitEvent.h"
#include "myUtils.h"
#include <vector>
#include "Exercise.h"
#include "MicroBitUARTService.h"
extern MicroBitUARTService *uart;


class Training {
public:
    std::vector<Exercise> exercises;
    int customDate;
    ManagedString dateString;
    ManagedString name;
    void inputLoopOverExercises();
    void onExercisesChanged(MicroBitEvent e);
    void onRemoveExercise(MicroBitEvent e);
    void onSetName(MicroBitEvent e);
    void onSetDate(MicroBitEvent e);
};


#endif //MICROBIT_SAMPLES_TRAINING_H
