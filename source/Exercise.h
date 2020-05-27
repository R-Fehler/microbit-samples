//
// Created by Richard on 23.04.2020.
//

#ifndef MICROBIT_SAMPLES_EXERCISE_H
#define MICROBIT_SAMPLES_EXERCISE_H
#include <vector>
#include "ManagedString.h"
#include "ExerciseSet.h"
#include "myUtils.h"
#include <regex>

class Exercise {


public:
    ManagedString name;
    uint16_t custom_id;
    std::vector<ExerciseSet> sets;

    Exercise();

    ManagedString toString();
    void fromString(const ManagedString& s);

};

#endif //MICROBIT_SAMPLES_EXERCISE_H
