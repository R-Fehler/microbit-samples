#include "MicroBit.h"
#define UNUSED(x) (void)(x)

MicroBit uBit;
const int MY_WAKEUP_EVENT_ID = MICROBIT_ID_NOTIFY + 10; 
// MICROBIT_ID_NOTIFY is the last event Id used by system
const int MY_WAKEUP_EVENT_VALUE = 1;

void loopUntilSent(ManagedString str) {
    int rtn = uBit.serial.send(str);
    while(rtn == MICROBIT_SERIAL_IN_USE) {
       uBit.sleep(0); // let other tasks run
       rtn = uBit.serial.send(str); 
    }
}
void task_1() {
    loopUntilSent("Started task_1, put it to sleep.\n");
    fiber_wait_for_event(MY_WAKEUP_EVENT_ID,MY_WAKEUP_EVENT_VALUE);
    loopUntilSent("task_1 woken up.\n");
}
void task_2() {
    loopUntilSent("Started task_2, put it to sleep.\n");
    fiber_wait_for_event(MY_WAKEUP_EVENT_ID,MY_WAKEUP_EVENT_VALUE);
    loopUntilSent("task_2 woken up.\n");
}
void task_3() {
    loopUntilSent("Started task_3, put it to sleep.\n");
    fiber_wait_for_event(MY_WAKEUP_EVENT_ID,MY_WAKEUP_EVENT_VALUE);
    loopUntilSent("task_3 woken up.\n");
}

void fireMyEvent(MicroBitEvent evt) {
    UNUSED(evt);
    // this method listens for a ButtonA click and fires event to wake up all the waiting tasks
    // could also have just use fiber_wait_for_event(MICROBIT_ID_BUTTON_A,MICROBIT_BUTTON_EVT_CLICK) 
    // directly in task_1,etc and not have this method at all
    loopUntilSent("Button A clicked, send my event to wake up all the tasks\n");
    MicroBitEvent(MY_WAKEUP_EVENT_ID,MY_WAKEUP_EVENT_VALUE); // fire wake up event
}

int main() {
    uBit.init();
    uBit.display.print("Start"); // will pause here a little while while showing Start
    // gives you time to open the Serial terminal if you need to   
    
    // register listen for ButtonA click
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A,MICROBIT_BUTTON_EVT_CLICK,fireMyEvent);
    create_fiber(task_1); // create tasks
    create_fiber(task_2); //
    create_fiber(task_3); //
    release_fiber(); // finished with setup, release the fibers!!
}



