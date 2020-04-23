//
// Created by Richard on 23.04.2020.
//

#ifndef MICROBIT_SAMPLES_UTILITY_H
#define MICROBIT_SAMPLES_UTILITY_H

#include "ManagedString.h"
#include <vector>
#include <cstdlib>
#include <cstdio>

template<typename ... Args>
ManagedString myFormat(const ManagedString format,Args ... args){
    size_t size = snprintf( nullptr, 0, format.toCharArray(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ){ return ManagedString("");}
    std::vector<char> buf(size + 1);
    snprintf(&buf[0], buf.size(), format.toCharArray(), args ...);
    return ManagedString(buf.data());
}

#endif //MICROBIT_SAMPLES_UTILITY_H
