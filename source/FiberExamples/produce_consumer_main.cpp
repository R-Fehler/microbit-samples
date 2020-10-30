#include "MicroBit.h"
#define UNUSED(x) (void)(x)

MicroBit uBit;


const int DATA_ID = MICROBIT_ID_NOTIFY+1; // last defined eventId is MICROBIT_ID_NOTIFY==1023 in MicroBitComponent.h 
const int NEW_DATA = 1;

// struct for the data
struct DATA_STRUCT {
    int count;
    char c;
};

struct DATA_STRUCT data; // the global to update and access between fibers

const int debugBufferSize = 62;
char debugBuffer[debugBufferSize+2]; // allow 1 for terminating null and 1 for newline
int debugBufferIdx = 0; // next location to write to leave first char for \n
int dumpInterval = 5000;

void debugBufferInit() {
    debugBuffer[0] = '\n';
    debugBuffer[1] = '\0'; // terminate buffer
    debugBufferIdx = 1;;
}

void debugChar(char c) {
    if (debugBufferIdx>=debugBufferSize) {
        return; // buffer full
    }
    debugBuffer[debugBufferIdx] = c;
    debugBufferIdx++;
    debugBuffer[debugBufferIdx] = '\0'; // terminate buffer
}

// must call this from a fiber!!
void sendDebugBuffer() {
    int size = debugBufferIdx;
    debugBufferIdx = debugBufferSize+1; // block any further additions until write is finished
    // add newline
    debugBuffer[size] = '\n';
    size++;
    debugBuffer[size] = '\0'; // terminate buffer    
    int rtn = uBit.serial.send((uint8_t *)debugBuffer,size);
    while (rtn == MICROBIT_SERIAL_IN_USE) {
        // try again later, wait for TX buffer empty
        fiber_wait_for_event(MICROBIT_ID_NOTIFY, MICROBIT_SERIAL_EVT_TX_EMPTY);
        rtn = uBit.serial.send((uint8_t *)debugBuffer,size);
    }
    debugBufferInit(); // clear buffer and put in leading \n
}

void producer() {
    while(1) { // loop for ever
        MicroBitEvent(DATA_ID,NEW_DATA);
        uBit.sleep(20); // wait a short while
        data.c = (char)('A'+data.count); // next char in sequence
        debugChar(data.c);
        data.count++;
        if (data.count>=26) {
            data.count = 0;
        }
    }
}

void consumer(MicroBitEvent evt) {
    UNUSED(evt); // keep Netbeans from complaining about unreferenced evt 
    struct DATA_STRUCT localData;
    localData.c = data.c;
    localData.count = data.count;
    uBit.serial.sendChar(localData.c);
    uBit.display.printChar(localData.c,1000);
    uBit.serial.send(localData.count);
    uBit.display.print(localData.count);
}

void debugOut(void *intervalPtr) {
    int interval = *((int*)intervalPtr);
    while(1) {
        uBit.sleep(interval); // every 5 sec
        sendDebugBuffer();
    }
}

int main() {
    debugBufferInit();
    uBit.init();
    uBit.display.print("Start"); // will pause here a little while while showing Start
    // gives you time to open the Serial terminal if you need to   
    // initialize the data
    data.count = 0;
    data.c = 'A';
    
    // register consumer to listen to produce
    uBit.messageBus.listen(DATA_ID,NEW_DATA,consumer);
    create_fiber(producer); //
    create_fiber(debugOut,(void*)&dumpInterval); // send debug every 5 sec
    release_fiber(); // finished with setup, release the fibers!!
}



