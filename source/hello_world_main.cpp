/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "MicroBit.h"
#include "mbed.h"
#include "bmp3/bmp3.h"
#include "bmp3/self-test/bmp3_selftest.h"
#include "myUtils.h"
#include <algorithm>
#include <numeric>
#include <deque>
#include <set>
#include "peaks/peaks.h"
#include "TwoDigitDisplay.h"
MicroBit uBit;
void delay_ms(uint32_t period_ms);
int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
void print_rslt(const char api_name[], int8_t rslt);

int8_t get_sensor_data(struct bmp3_dev *dev)
{
    int8_t rslt;
    /* Variable used to select the sensor component */
    uint8_t sensor_comp;
    /* Variable used to store the compensated data */
    struct bmp3_data data;

    /* Sensor component selection */
    sensor_comp = BMP3_PRESS | BMP3_TEMP;
    /* Temperature and Pressure data are read and stored in the bmp3_data instance */
    rslt = bmp3_get_sensor_data(sensor_comp, &data, dev);

/* Print the temperature and pressure data */
// uBit.serial.printf("Temperature in deg celsius\t Pressure in Pascal\t\n");
#ifdef BMP3_DOUBLE_PRECISION_COMPENSATION
    uBit.serial.printf("%0.2f\t\t %0.2f\t\t\n", data.temperature, data.pressure);
#else
    /* for fixed point the compensated temperature and pressure output has a multiplication factor of 100 */
    uBit.serial.printf("%d\n", data.pressure);
#endif

    return rslt;
}

int8_t set_normal_mode(struct bmp3_dev *dev)
{
    int8_t rslt;
    /* Used to select the settings user needs to change */
    uint16_t settings_sel;

    /* Select the pressure and temperature sensor to be enabled */
    dev->settings.press_en = BMP3_ENABLE;
    dev->settings.temp_en = BMP3_ENABLE;
    /* Select the output data rate and oversampling settings for pressure and temperature */
    dev->settings.odr_filter.press_os = BMP3_OVERSAMPLING_32X;
    dev->settings.odr_filter.temp_os = BMP3_OVERSAMPLING_4X;
    dev->settings.odr_filter.odr = BMP3_ODR_6_25_HZ;
    dev->settings.odr_filter.iir_filter = BMP3_IIR_FILTER_COEFF_127;
    /* Assign the settings which needs to be set in the sensor */
    settings_sel = BMP3_PRESS_EN_SEL | BMP3_TEMP_EN_SEL | BMP3_PRESS_OS_SEL | BMP3_TEMP_OS_SEL | BMP3_ODR_SEL;
    rslt = bmp3_set_sensor_settings(settings_sel, dev);

    /* Set the power mode to normal mode */
    dev->settings.op_mode = BMP3_NORMAL_MODE;
    rslt = bmp3_set_op_mode(dev);

    return rslt;
}
int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_180);

    //Entwurf der Klasse
    /**
     * bmpdev
     *conf
     * ucompdata
     * pres64
     * double pres
     * 
     * in constr.
     * 
     **/
    uint32_t pres32, pres64;
    double pres;

    struct bmp3_dev dev;
    int8_t rslt = BMP3_OK;

    dev.dev_id = BMP3_I2C_ADDR_PRIM * 2;
    dev.intf = BMP3_I2C_INTF;
    dev.read = i2c_reg_read;
    dev.write = i2c_reg_write;
    dev.delay_ms = delay_ms;

    rslt = bmp3_init(&dev);
    // print_rslt("init status", rslt);
    rslt = set_normal_mode(&dev);
    //    print_rslt("set normal mode status",rslt);
    struct bmp3_data data;
    //Buffer for 10 seconds with 6.25Hz
    uint8_t sensor_comp = BMP3_PRESS | BMP3_TEMP;
    rslt = bmp3_get_sensor_data(sensor_comp, &data, &dev);
    uBit.sleep(160);
    rslt = bmp3_get_sensor_data(sensor_comp, &data, &dev);
    uBit.sleep(160);
    rslt = bmp3_get_sensor_data(sensor_comp, &data, &dev);

    std::deque<int64_t> filteredSensorData(60,data.pressure);
    std::deque<int64_t> negativeFilteredSensorData(filteredSensorData.size(),-data.pressure);

    const int filtersize = 6;
    std::deque<int64_t> filterBuffer(filtersize,data.pressure);


    int64_t newSensorDataFiltered = data.pressure;
    int64_t negativeNewSDF =-data.pressure;
    std::set<int64_t> uniquePeakValues;

    int64_t meanValue=data.pressure;
    int64_t maxValue=data.pressure;
    int64_t minValue=data.pressure;
    int decidingDelta=280;
    char decision='n';//no decision ; u for up eg deadlift, d for down eg squat
    int counter = 0;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true)
    {
        rslt = bmp3_get_sensor_data(sensor_comp, &data, &dev);
        filterBuffer.pop_front();
        filterBuffer.push_back((int64_t)data.pressure);

        newSensorDataFiltered = std::accumulate(filterBuffer.begin(), filterBuffer.end(), (int64_t) 0) / filtersize;
        negativeNewSDF=-newSensorDataFiltered;
        filteredSensorData.pop_front();
        filteredSensorData.push_back(newSensorDataFiltered);
        negativeFilteredSensorData.pop_front();
        negativeFilteredSensorData.push_back(negativeNewSDF);
        std::vector<int> indexOfPeaksInFilteredSD;
        std::vector<int> indexOfPeaksInNegFilteredSD;


        meanValue=std::accumulate(filteredSensorData.begin(),filteredSensorData.end(),(int64_t)0)/filteredSensorData.size();
        maxValue=*max_element(filteredSensorData.begin(),filteredSensorData.end());
        minValue=*min_element(filteredSensorData.begin(),filteredSensorData.end());
        //TODO Filter index that are too close to each other. eg smaller than 5 samples keep the first
        if(((abs(maxValue-meanValue)>decidingDelta && abs(minValue-meanValue)<decidingDelta) && decision=='n') or decision=='d'){
            decision='d';

            Peaks::findPeaks({filteredSensorData.begin(), filteredSensorData.end()}, indexOfPeaksInFilteredSD, 0);
            for (int index : indexOfPeaksInFilteredSD) {
                if (index > 10 && index < filteredSensorData.size() - 1) { // must be > 5 like in peaks.cpp minDistanceOfPeaks

                    auto ret = uniquePeakValues.insert(filteredSensorData[index]);

                }
            }

        }
        else if(((abs(maxValue-meanValue)<decidingDelta && abs(minValue-meanValue)>decidingDelta) && decision=='n') or decision=='u'){
            decision='u';
        Peaks::findPeaks({negativeFilteredSensorData.begin(), negativeFilteredSensorData.end()}, indexOfPeaksInNegFilteredSD, 0);

                for (int index : indexOfPeaksInNegFilteredSD) {
                    if (index > 10 && index < filteredSensorData.size() - 1) {

                        auto ret = uniquePeakValues.insert(filteredSensorData[index]);

                    }
                }
        }

        if(abs(maxValue-meanValue)<decidingDelta && abs(minValue - meanValue)<decidingDelta){
            decision='n';
        }


        uBit.serial.printf("%d\n", newSensorDataFiltered);
        uBit.display.printCharAsync(decision);
        int reps = static_cast<int>(uniquePeakValues.size());
        uBit.display.clear();
        uBit.display.print(TwoDigitDisplay::createImage(reps));
        uBit.sleep(140);
        counter++;
    }
#pragma clang diagnostic pop
    //    while (true)
    //    {
    //
    //
    //    /* Sensor component selection */
    //    sensor_comp = BMP3_PRESS | BMP3_TEMP;
    //    /* Temperature and Pressure data are read and stored in the bmp3_data instance */
    //    rslt = bmp3_get_sensor_data(sensor_comp, &data, dev);
    //    databuffer[i%filtersize]=data.pressure;
    //    sumOverBuffer=std::accumulate(begin(databuffer),end(databuffer),0);
    //    movingAveragePressure=sumOverBuffer/filtersize;
    //    const auto [min, max]=std::minmax_element(begin(databuffer),end(databuffer));
    //    delta=*max-*min;
    //    globalDeltaMax=std::max(delta,globalDeltaMax);
    //    uint64_t thresholdDelta=8*globalDeltaMax/10;
    //    if(delta>thresholdDelta && repIsGoing==false){
    //        repIsGoing=true;
    //    }

    // get_sensor_data(&dev);
    uBit.sleep(160);

    //TODO Microbit Runtime Driver with idletick function.
    // Insert your code here!
    uBit.display.scroll("HELLO WORLD! :)");

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
    return 0;
}

/*!
 *  @brief Function that creates a mandatory delay required in some of the APIs such as "bmg250_soft_reset",
 *      "bmg250_set_foc", "bmg250_perform_self_test"  and so on.
 *
 *  @param[in] period_ms  : the required wait time in milliseconds.
 *  @return void.
 *
 */
void delay_ms(uint32_t period_ms)
{
    wait_ms(period_ms);
    /* Implement the delay routine according to the target machine */
}

/*!
 *  @brief Function for writing the sensor's registers through I2C bus.
 *
 *  @param[in] i2c_addr : sensor I2C address.
 *  @param[in] reg_addr : Register address.
 *  @param[in] reg_data : Pointer to the data buffer whose value is to be written.
 *  @param[in] length   : No of bytes to write.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
    //        //TODO Test if it works with "real" burst write

    //        build the complete byte array to write over i2c
    //      bmp burst write reg_data looks like: data,..optional.. addr, data, addr....
    uint8_t command[length + 1];
    command[0] = reg_addr;
    for (int i = 0; i < length; i++)
    {
        command[1 + i] = reg_data[i];
    }
    int rslt = -1;
    // write as single 2 byte i2c commands, not burst write.
    for (int j = 0; j <= length / 2; ++j)
    {
        rslt = uBit.i2c.writeRegister(i2c_addr, command[j * 2], command[j * 2 + 1]);
    }
    //real burst write

    //        rslt=uBit.i2c.write(i2c_addr, reinterpret_cast<const char *>(command), length + 1);

    rslt = -rslt;
    return rslt;
}

/*!
 *  @brief Function for reading the sensor's registers through I2C bus.
 *
 *  @param[in] i2c_addr : Sensor I2C address.
 *  @param[in] reg_addr : Register address.
 *  @param[out] reg_data    : Pointer to the data buffer to store the read data.
 *  @param[in] length   : No of bytes to read.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
    int rslt = -1;

    rslt = uBit.i2c.readRegister(i2c_addr, reg_addr, reg_data, length);
    rslt = -rslt;
    /* Implement the I2C read routine according to the target machine. */
    return rslt;
}

/*!
 *  @brief Function for writing the sensor's registers through SPI bus.
 *
 *  @param[in] cs           : Chip select to enable the sensor.
 *  @param[in] reg_addr     : Register address.
 *  @param[in] reg_data : Pointer to the data buffer whose data has to be written.
 *  @param[in] length       : No of bytes to write.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{

    /* Implement the SPI write routine according to the target machine. */
    return -1;
}

/*!
 *  @brief Function for reading the sensor's registers through SPI bus.
 *
 *  @param[in] cs       : Chip select to enable the sensor.
 *  @param[in] reg_addr : Register address.
 *  @param[out] reg_data    : Pointer to the data buffer to store the read data.
 *  @param[in] length   : No of bytes to read.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{

    /* Implement the SPI read routine according to the target machine. */
    return -1;
}

/*!
 *  @brief Prints the execution status of the APIs.
 *
 *  @param[in] api_name : name of the API whose execution status has to be printed.
 *  @param[in] rslt     : error code returned by the API whose execution status has to be printed.
 *
 *  @return void.
 */
void print_rslt(const char api_name[], int8_t rslt)
{
    if (rslt != BMP3_OK)
    {
        uBit.serial.printf("%s\t", api_name);
        if (rslt == BMP3_E_NULL_PTR)
        {
            uBit.serial.printf("Error [%d] : Null pointer error\r\n", rslt);
        }
        else if (rslt == BMP3_E_COMM_FAIL)
        {
            uBit.serial.printf("Error [%d] : Bus communication failed\r\n", rslt);
        }
        else if (rslt == BMP3_IMPLAUSIBLE_TEMPERATURE)
        {
            uBit.serial.printf("Error [%d] : Invalid Temperature\r\n", rslt);
        }
        else if (rslt == BMP3_E_DEV_NOT_FOUND)
        {
            uBit.serial.printf("Error [%d] : Device not found\r\n", rslt);
        }
        else
        {
            /* For more error codes refer "*_defs.h" */
            uBit.serial.printf("Error [%d] : Unknown error code\r\n", rslt);
        }
    }
    else
    {
        uBit.serial.printf("%s\t OK!", api_name);
    }
}
