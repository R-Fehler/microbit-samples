#include "MicroBit.h"

MicroBit uBit;
uint8_t connected = 0;
//BLE Connections Handlers are called on the BLE Events of Connection / Disconnection and Display User Feedback
void onConnected (MicroBitEvent)
{
    connected = 1;
    uBit.display.printAsync("C");
}

void onDisconnected(MicroBitEvent)
{
    connected = 0;
    uBit.display.printAsync("D");

}
void onButton(MicroBitEvent)
{
    uBit.serial.printf("Hallo %d",10);
}

void led(MicroBitEvent)
{
    uBit.display.print("X");
}
int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    // Display a start-up message
    uBit.display.scrollAsync("DATA");
    uBit.messageBus.listen(MICROBIT_ID_BLE,MICROBIT_BLE_EVT_CONNECTED,onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE,MICROBIT_BLE_EVT_DISCONNECTED,onDisconnected);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A,MICROBIT_BUTTON_EVT_CLICK,onButton);
    uBit.messageBus.listen(MICROBIT_ID_SERIAL,MICROBIT_SERIAL_EVT_HEAD_MATCH,led);
    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
    new MicroBitTemperatureService(*uBit.ble,uBit.thermometer);

    
    // Application code will go here and in functions outside of main()
    
    
    
    
    
    // end of application code in main()    
    
    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
