#include "MicroBit.h"
#include "ManagedPrintf.h"

MicroBit uBit;

bool lockBusy = false;
const int RESOURCE_ID = MICROBIT_ID_NOTIFY + 100;
const int RESOURCE_FREE = 4096;

void loopUntilSent(ManagedString str) {
    int rtn = uBit.serial.send(str);
    while (rtn == MICROBIT_SERIAL_IN_USE) {
        uBit.sleep(0); // let other tasks run
        rtn = uBit.serial.send(str);
    }
}

void unlock() {
    lockBusy = false;
    MicroBitEvent(RESOURCE_ID, RESOURCE_FREE); // wake next fiber, in order, that is waiting to use Serial
    uBit.sleep(0); // put this fiber on the end of the queue so other waiting fibers get a go at the resource
}

void lock() {
    lockBusy = true;
}

bool isLocked() {
    return lockBusy;
}

void useLimitedResource() {
    while (isLocked()) {
        loopUntilSent("Putting Fiber:"+ManagedPrintf::getAddress(currentFiber)+ " to sleep waiting for access the resource\n");
        fiber_wait_for_event(RESOURCE_ID, RESOURCE_FREE);
    }
    lock();
    loopUntilSent("Fiber:"+ManagedPrintf::getAddress(currentFiber)+ " has control of the resource\n");
    uBit.sleep(2000);
    unlock();
}


void runA() {
    loopUntilSent("Started FiberA:"+ManagedPrintf::getAddress(currentFiber)+"\n");
    while (1) {
        useLimitedResource();
    }
}

void runB() {
    loopUntilSent("Started FiberB:"+ManagedPrintf::getAddress(currentFiber)+"\n");
    while (1) {
        useLimitedResource();
    }
}

void runC() {
    loopUntilSent("Started FiberC:"+ManagedPrintf::getAddress(currentFiber)+"\n");
    while (1) {
        useLimitedResource();
    }
}

int main() {
    uBit.init();
    uBit.display.print("Start");
    uBit.serial.send("main() fiber:"+ManagedPrintf::getAddress(currentFiber)+"\n");
    create_fiber(runA);
    create_fiber(runB);
    create_fiber(runC);
    release_fiber(); // finished with setup, release the fibers!!
}
