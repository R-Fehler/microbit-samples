
#include "MicroBit.h"

/*
 * Temperature Alarm
 * Uses the Bluetooth Event Service to inform registered clients whenever the temperature falls below xx or rises above yy
 */
MicroBit uBit;

uint16_t state      = 0; // 0=ok, 1=alarming because it's cold, 2=alarming because it's hot
uint16_t ok         = 0;
uint16_t temp_alarm = 9000;
uint16_t set_lower  = 9001;
uint16_t set_upper  = 9002;
uint16_t cold       = 1;
uint16_t hot        = 2;
uint8_t  upper      = 24;// evt.value ist eh byte[2]
uint8_t  lower      = 19;
int reading_count   = 0;

void onSetLower(MicroBitEvent e)
{
    lower = e.value;
    uBit.display.scroll("L=");
    uBit.display.scrollAsync(lower);
}

void onSetUpper(MicroBitEvent e)
{
    upper = e.value;
    uBit.display.scroll("U=");
    uBit.display.scrollAsync(upper);
}

void onTemperatureReading(MicroBitEvent e)
{
    int temperature = uBit.thermometer.getTemperature();
    reading_count++;
    if (reading_count == 10) {
        uBit.display.scrollAsync(temperature);
        reading_count = 0;
    }
    if (temperature > upper && state == ok) {
        MicroBitEvent evt(temp_alarm, hot);
        state = hot;
        return;
    }
    if (temperature < lower && state == ok) {
        MicroBitEvent evt(temp_alarm, cold);
        state = cold;
        return;
    }
    if (temperature >= lower && temperature <= upper && state != ok) {
        MicroBitEvent evt(temp_alarm, ok);
        state = ok;
    }
}

int main()
{
    uBit.init();
    uBit.display.scrollAsync("TEMP ALARM");
    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);
    // listen for client events which set the upper and lower temperature limits
    uBit.messageBus.listen(set_lower, MICROBIT_EVT_ANY, onSetLower);
    uBit.messageBus.listen(set_upper, MICROBIT_EVT_ANY, onSetUpper);
    // listen to the temperature sensor
    uBit.messageBus.listen(MICROBIT_ID_THERMOMETER, MICROBIT_THERMOMETER_EVT_UPDATE, onTemperatureReading);
    release_fiber();
}
