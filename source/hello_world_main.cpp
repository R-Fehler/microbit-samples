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
#include "bmp/bmp280.h"
MicroBit uBit;
void delay_ms(uint32_t period_ms);
int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
void print_rslt(const char api_name[], int8_t rslt);


int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
/*!
 *  @brief Example shows basic setup of sensor which includes following
 *      Initialization of the interface.
 *      performing the sensor initialization.
 */



//
        int8_t rslt;
//        char data[1];
//        int adress=0x76;
//       rslt= uBit.i2c.read(2*adress,data,1);
//       printf("Adress: %d \t rslt: %d \t data= %d\n",adress,rslt,data);
//       uBit.sleep(15);

        uBit.sleep(1000);

        struct bmp280_dev bmp;

        /* Map the delay function pointer with the function responsible for implementing the delay */
        bmp.delay_ms = delay_ms;

        /* Assign device I2C address based on the status of SDO pin (GND for PRIMARY(0x76) & VDD for SECONDARY(0x77)) */
        bmp.dev_id = BMP280_I2C_ADDR_PRIM*2;

        /* Select the interface mode as I2C */
        bmp.intf = BMP280_I2C_INTF;

        /* Map the I2C read & write function pointer with the functions responsible for I2C bus transfer */
        bmp.read = i2c_reg_read;
        bmp.write = i2c_reg_write;

        /* To enable SPI interface: comment the above 4 lines and uncomment the below 4 lines */

        /*
         * bmp.dev_id = 0;
         * bmp.read = spi_reg_read;
         * bmp.write = spi_reg_write;
         * bmp.intf = BMP280_SPI_INTF;
         */
        rslt = bmp280_init(&bmp);
        print_rslt(" bmp280_init status", rslt);


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
        uBit.sleep(period_ms);
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
        int rslt=-1;
        for (int i = 0; i < length; ++i) {
            rslt=uBit.i2c.writeRegister(i2c_addr,reg_addr+(i*8),reg_data[i]);//unklar ob reg_addr auch inkrementiert werden muss
        }
        rslt=-rslt;
        /* Implement the I2C write routine according to the target machine. */
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
    int rslt=-1;

            rslt=uBit.i2c.readRegister(i2c_addr,reg_addr,reg_data,length);
            uBit.serial.printf("uBit.i2c.readreg reslt: %d", rslt);
            rslt=-rslt;
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
        if (rslt != BMP280_OK)
        {
            uBit.serial.printf("%s\t", api_name);
            if (rslt == BMP280_E_NULL_PTR)
            {
                uBit.serial.printf("Error [%d] : Null pointer error\r\n", rslt);
            }
            else if (rslt == BMP280_E_COMM_FAIL)
            {
                uBit.serial.printf("Error [%d] : Bus communication failed\r\n", rslt);
            }
            else if (rslt == BMP280_E_IMPLAUS_TEMP)
            {
                uBit.serial.printf("Error [%d] : Invalid Temperature\r\n", rslt);
            }
            else if (rslt == BMP280_E_DEV_NOT_FOUND)
            {
                uBit.serial.printf("Error [%d] : Device not found\r\n", rslt);
            }
            else
            {
                /* For more error codes refer "*_defs.h" */
                uBit.serial.printf("Error [%d] : Unknown error code\r\n", rslt);
            }
        }
    }




