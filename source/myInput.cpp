//
// Created by Richard on 28.04.2020.
//
#include "myInput.h"


int16_t inputBuff=0;
bool inputterm= false;
uint8_t goback=0;
bool terminator= false;
int pause_setting=PAUSEMULTIPLIKATOR;

int myInput::input() {
    int ret = 0;

    while (true) {
        ret = ret + inputBuff;
        inputBuff = 0;
        // uBit.display.printAsync(ret);
        uBit.display.printAsync(TwoDigitDisplay::createImage(ret));
        uBit.sleep(INPUTLAG);
        //    uBit.display.clear();
        if (inputterm == 1) {
            inputterm = 0;
            return ret;
        }
    }


}

int myInput::input(int previous_val) {
    int ret = previous_val;
    int i = 0;
    while (true) {
        if (inputBuff == 0 and i != 0) {
            /*if (goback != 0) {
                return ret;
            }*/
            if (inputterm == 1) {
                inputterm = 0;
                return ret;
            }

            if (i > DISPLAYTIME) {
                uBit.display.clear();
            }
        }
        else {
            i = 0;
            ret = ret + inputBuff;
            ret = abs(ret);
            inputBuff = 0;
            uBit.display.printAsync(TwoDigitDisplay::createImage(ret));
        }
        uBit.sleep(INPUTLAG);
        i++;
    }
}

int myInput::input(const char *message) {
    int ret = 0;
    uBit.display.print(message);
    while (0 == uBit.buttonA.isPressed() and 0 == uBit.buttonB.isPressed()) {
        uBit.sleep(INPUTLAG);
    }

    int i = 0;
    while ((true)) {
        if (inputBuff == 0 and i != 0) {
            if (goback != 0) {
                return ret;
            }
            if (inputterm == 1) {
                inputterm = 0;
                return ret;
            }

            if (i > DISPLAYTIME) {
                uBit.display.clear();
            }
        }
        else {
            i = 0;
            ret = ret + inputBuff;
            ret = abs(ret);
            inputBuff = 0;
            uBit.display.printAsync(TwoDigitDisplay::createImage(ret));
        }
        uBit.sleep(INPUTLAG);
        i++;
    }
}


int myInput::input(char message) {
    int ret = 0;
    uBit.display.printChar(message);
    while (0 == uBit.buttonA.isPressed() and 0 == uBit.buttonB.isPressed()) {
        uBit.sleep(INPUTLAG);
    }

    while (true) {
        ret = ret + inputBuff;
        inputBuff = 0;
        // uBit.display.printAsync(ret);
        uBit.display.printAsync(TwoDigitDisplay::createImage(ret));
        uBit.sleep(INPUTLAG);
        //    uBit.display.clear();
        if (inputterm == 1) {
            inputterm = 0;
            return ret;
        }
    }
}
/**
 * wartet time/10 sekunden auf Input, sonst previous_val
 * @param time
 * @param previous_val
 * @return new inputvalue or previous_val
 */
int myInput::input(int time, int previous_val) {
    int ret = previous_val;
    int i = 0;
    while ((i < time)) {
        if (inputBuff == 0 and i != 0) {
            if (goback != 0) {
                return ret;
            }
            if (inputterm == 1) {
                inputterm = 0;
                return ret;
            }

            if (i > DISPLAYTIME) {
                uBit.display.clear();
            }
        }
        else {
            i = 0;
            ret = ret + inputBuff;
            ret = abs(ret);
            // uBit.display.printAsync(ret);
            inputBuff = 0;
            uBit.display.printAsync(TwoDigitDisplay::createImage(ret));
        }
        uBit.sleep(INPUTLAG);
        //if (goback != 0) {
        //	return ret;
        //}
        //if (inputterm == 1) {
        //	inputterm = 0;
        //	return ret;
        //}
        i++;
    }
    //    uBit.display.clear();

    uBit.io.P1.setDigitalValue(0);
    uBit.io.P8.setDigitalValue(0);

    return ret;
}

void myInput::inputButton(MicroBitEvent e) {
    inputBuff = 0;
    if (e.source == MICROBIT_ID_BUTTON_A) {
        if (e.value == MICROBIT_BUTTON_EVT_CLICK)
            inputBuff = inputBuff + INCREMENT_A;
        if (e.value == MICROBIT_BUTTON_EVT_HOLD) {
            inputBuff = inputBuff - INCREMENT_A;
            uBit.sleep(300);
            while (uBit.buttonA.isPressed()) {

                uBit.sleep(150);
                inputBuff = inputBuff - INCREMENT_A;
            }
        }
    }
    if (e.source == MICROBIT_ID_BUTTON_B) {
        if (e.value == MICROBIT_BUTTON_EVT_CLICK)
            inputBuff = inputBuff + INCREMENT_B;
        if (e.value == MICROBIT_BUTTON_EVT_HOLD) {

            inputBuff = inputBuff - INCREMENT_B;
            uBit.sleep(300);
            while (uBit.buttonB.isPressed()) {
                uBit.sleep(150);
                inputBuff = inputBuff - INCREMENT_B;
            }
        }
    }

}
void myInput::input_done(MicroBitEvent e) {
    if(e.value==MICROBIT_BUTTON_EVT_CLICK){
    uBit.display.printCharAsync('>', FASTSCROLLTIME);
    inputterm = 1;
    }
    if(e.value==MICROBIT_BUTTON_EVT_LONG_CLICK){
        inputterm = 1;
        terminator = 1;
        uBit.display.printCharAsync('X', FASTSCROLLTIME);
    }
}

void myInput::go_back(MicroBitEvent e) {
    goback = 2;
    uBit.display.printCharAsync('<', FASTSCROLLTIME);
}

myInput::~myInput() {
    uBit.io.P0.eventOn(MICROBIT_PIN_EVENT_NONE);
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY,
                           this,&myInput::inputButton);
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY,
                           this,&myInput::inputButton);

    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_AB, MICROBIT_EVT_ANY,
                           this,&myInput::input_done);
    uBit.messageBus.ignore(MICROBIT_ID_IO_P0, MICROBIT_BUTTON_EVT_CLICK,
                           this,&myInput::go_back);
    uBit.sleep(20);
}

myInput::myInput() {
    	uBit.io.P0.eventOn(MICROBIT_PIN_EVENT_ON_TOUCH);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY,
                           this,&myInput::inputButton);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY,
                           this,&myInput::inputButton);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_EVT_ANY,
                           this,&myInput::input_done);
    uBit.messageBus.listen(MICROBIT_ID_IO_P0, MICROBIT_BUTTON_EVT_CLICK,
                           this,&myInput::go_back);

}

//myInput &myInput::getInstance() {
//    static myInput instance;
//    return instance;
//}

