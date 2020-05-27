//
// Created by Richard on 23.04.2020.
//

#ifndef MICROBIT_SAMPLES_UTILITY_H
#define MICROBIT_SAMPLES_UTILITY_H

#include "ManagedString.h"
#include "MicroBit.h"
#include <vector>
#include <cstdio>
extern MicroBit uBit;

template<typename ... Args>
ManagedString myFormat(const ManagedString format, Args ... args) {
    size_t size = snprintf(nullptr, 0, format.toCharArray(), args ...) + 1; // Extra space for '\0'
    if (size <= 0) { return ManagedString(""); }
    std::vector<char> buf(size + 1);
    snprintf(&buf[0], buf.size(), format.toCharArray(), args ...);
    return ManagedString(buf.data());
}


size_t inline printFloat(double number, uint8_t digits) {
    size_t n = 0;

    if (isnan(number)) return uBit.serial.printf("nan");
    if (isinf(number)) return uBit.serial.printf("inf");
    if (number > 4294967040.0) return uBit.serial.printf("ovf"); // constant determined empirically
    if (number <-4294967040.0) return uBit.serial.printf("ovf"); // constant determined empirically

    // Handle negative numbers
    if (number < 0.0) {
        n += uBit.serial.printf("-");
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i) {
        rounding /= 10.0;
    }

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long) number;
    double remainder = number - (double) int_part;
    n += uBit.serial.printf("%lu", int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0) {
        n += uBit.serial.printf(".");
    }

    // Extract digits from the remainder one at a time
    while (digits-- > 0) {
        remainder *= 10.0;
        int toPrint = int(remainder);
        n += uBit.serial.printf("%d", toPrint);
        remainder -= toPrint;
    }

    return n;
}


void inline printMemoryAndStop() {
    int blockSize = 64; int i = 1;
    uBit.serial.printf("Checking memory with blocksize %d char ...\n", blockSize);
    while (true) {
        char *p = (char *) malloc(blockSize);
        if (p == NULL) break; // this actually never happens, get a panic(20) outOfMemory first
        uBit.serial.printf("%d + %d/16 K\n", i/16, i%16);
        ++i;
    }
}

void inline displayLeftArrow() {
    MicroBitImage arrow("0,0,255,0,0\n0,255,0,0,0\n255,255,255,255,255\n0,255,0,0,0\n0,0,255,0,0\n");
    uBit.display.print(arrow, 0, 0, 0);
}

void inline displayTick() {
    MicroBitImage tick("0,0,0,0,0\n0,0,0,0,255\n0,0,0,255,0\n255,0,255,0,0\n0,255,0,0,0\n");
    uBit.display.print(tick, 0, 0, 0);
}


//template <typename Out>
//void split(const std::string &s, char delim, Out result) {
//    std::istringstream iss(s);
//    std::string item;
//    while (std::getline(iss, item, delim)) {
//        *result++ = item;
//    }
//}
//
//std::vector<std::string> split(const std::string &s, char delim) {
//    std::vector<std::string> elems;
//    split(s, delim, std::back_inserter(elems));
//    return elems;
//}

#endif //MICROBIT_SAMPLES_UTILITY_H
