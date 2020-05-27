//
// Created by Richard on 28.04.2020.
//

#ifndef MICROBIT_SAMPLES_MYINPUT_H
#define MICROBIT_SAMPLES_MYINPUT_H

#include "TwoDigitDisplay.h"
#include "MicroBit.h"
#include "TwoDigitDisplay.h"
#include "myUtils.h"

#define INCREMENT_A 10
#define INCREMENT_B 1
#define INPUTLAG 70
#define LONGTEXTDELAY 65
#define WAITFORINPUTTIME 400
#define FASTSCROLLTIME 40
#define DISPLAYTIME 50
#define PAUSEMULTIPLIKATOR 2562

extern int16_t inputBuff;
extern bool inputterm;
extern uint8_t goback;
extern bool terminator;
extern int pause_setting;

class myInput{
public:

//     static myInput& getInstance();
     int input();

     int input(int previous_val);

     int input(const char *message);

     int input(char message);

     int input(int time, int previous_val);

     void inputButton(MicroBitEvent e);

    ~myInput();

    myInput();
private:

    void input_done(MicroBitEvent);

    void terminate_loop(MicroBitEvent e);

    void go_back(MicroBitEvent e);
};


#endif //MICROBIT_SAMPLES_MYINPUT_H
