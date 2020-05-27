//
// Created by Richard on 28.04.2020.
//


#include "ExerciseSet.h"

ExerciseSet::ExerciseSet(int w, int r) {
    weight=w;
    reps=r;
    isDone=false;
}

ExerciseSet::ExerciseSet() : weight(20),reps(0),isDone(false){
}





void ExerciseSet::cntReps(myInput& input, uint32_t threshold) {

    uBit.accelerometer.setRange(4);
    uBit.accelerometer.setPeriod(20);


    //ToDo Calibration reps only for first set of exersice id //warmup
        int dptime = DPTIMEUNTILFIRSTREP;
        const int sizefilter = 20;
        const int max_filter_div = 4;
        const int max_sizefilter = sizefilter / max_filter_div;
        uint32_t threshold_start = threshold;
        uint32_t filterbuf[sizefilter] = { 0 };
        uint32_t i = 0;
        uint32_t sum = 0;
        uint32_t res = 0;
        uint32_t max[max_sizefilter] = { threshold };
        uint32_t maxaverage;
        uint32_t sum_max = 0;
        //TODO : Testen ob die ganzen uint auf dem heap alocated werden und nicht gefreed
        int multi = 9;
        int divide = 10;
        const int noreps_threshold = 1200000;
        const int noreps_dp = 250;
        int norepcnt = 0;
        bool rep = false;
        int repcnt = 0;

    //max times 5/4;
        uBit.display.printAsync(TwoDigitDisplay::createImage(repcnt));
        while (true)
        {

            accel = uBit.accelerometer.getSample();

            filterbuf[i%sizefilter] = myForce(accel);
            sum = 0;
            for (size_t j = 0; j < sizeof(filterbuf) / sizeof(uint32_t); j++)
            {
                sum += filterbuf[j];
            }
            //accelbuff = uBit.accelerometer.getSample();
            //buffer = force(accelbuff);

            // lowpass += (buffer - lowpass) / alpha;
            res = (sum / sizefilter);
#ifdef  ACCELOREMETERSERIALOUTPUT
            uBit.serial.printf("%d\n", res);
#endif
            max[i%max_sizefilter] = maxC(max[i % max_sizefilter], res);
            //uBit.serial.printf("%d\n", max);
            sum_max = 0;
            for (size_t j = 0; j < sizeof(max) / sizeof(uint32_t); j++)
            {
                sum_max += max[j];
            }
            maxaverage = (sum_max / max_sizefilter);
            if (i%(max_sizefilter*max_filter_div*MAX_FILTER_REDUCTION_CONSTANT)==0) {
                max[i%max_sizefilter] = threshold_start;
            }



            threshold = maxC(threshold, (maxaverage * multi / divide));
            /*	uBit.serial.printf("maxaverage: %d\n", maxaverage);
                uBit.serial.printf("res:        %d\n", res);
                uBit.serial.printf("threshold:  %d\n", threshold);
        */
            //uBit.serial.printf("threshold: %d\n", threshold);
            //uBit.serial.printf("res:       %d\n", res);
            if (inputBuff != 0) {
                repcnt = repcnt + (inputBuff);
                repcnt = abs(repcnt);
                uBit.serial.printf("rpcnt:%d", repcnt);
                uBit.display.printAsync(TwoDigitDisplay::createImage(repcnt));
                norepcnt = 0;
                inputBuff = 0;
//                uart->send(ManagedString(repcnt));//TODO stattdessen evt()
            }
            if (res > threshold) {
                if (rep == false) {
                    rep = true;
                    repcnt++;
                    uBit.serial.printf("\t\t REEEEEP");
                    uBit.display.printAsync(TwoDigitDisplay::createImage(repcnt));
//                    uart->send(ManagedString(repcnt));
                    norepcnt = 0;
                }

            }
            if (res < threshold) {
                if (res<noreps_threshold) {
                    norepcnt++;
                }

                if (rep == true) {
                    rep = false;
                }
            }
            uBit.sleep(20);
            if (norepcnt > noreps_dp) {

                uBit.display.clear();
            }


            if (goback != 0) {
                return;
                uBit.io.P1.setDigitalValue(0);
                uBit.io.P8.setDigitalValue(0);
            }
            if (norepcnt >= pause_setting or 1 == inputterm) {
                inputterm = 0;
                uBit.io.P1.setDigitalValue(0);
                uBit.io.P8.setDigitalValue(0);
                if (norepcnt >= pause_setting) {

                    uBit.display.printChar('|');
                    for (size_t j = 0; j < 10; j++) //TODO Magic 10 entfernen
                    {

                        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_90);
                        uBit.sleep(500);
                        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_0);
                        uBit.sleep(500);
                    }
                    uBit.display.clear();
                }
                reps= repcnt;
                isDone=true;
                return;
            }
            i++;


        }




    }



