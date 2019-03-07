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


/**IDEE: jede Woche oder Tag eine eigene File.csv für die Uebersichtlichkeit. Import etwas unangenehm dafür kompakte Dateien.
 * oder Woche ist Directory und Tag ist file.csv.
 * IDEE: Datum ueber increment alle 24h. uBit wacht aus sleep und erhoeht die Zahl. so oder so aehnlich.
 * 
 * CSV FORMAT: date, id([i]), load[j], reps[j] index: gut mit Schleifen. zb print Befehl.
 * CSV FORMAT: Export CSVs von anderen Logging Apps vergleichen. Evtl den Standard übernehmen
 * 
 * 
 * EXPORT: Serial Input Befehl: zb load oder l über Serial empfangen: sendet ganze csv via serial. danach zb Befehl remove via Serial. entfernt csv im FileSystem.
 *
 * TODOs: 
-Datum, RealTimeClock oÄ. sodass das Datum automatisch angepasst wird. Plan B: Datum via BLE über Smartphone beziehen. und ansonsten von Hand Eingeben.
-log.csv soll auf Befehl vom USB Serial und/oder BLE UART gesendet werden. Dazu muss eine Abfrage implementiert werden.

 *
 * 
 *  
 */



#include "MicroBit.h"
#include "MicroBitFile.h"
MicroBit uBit;
/**wake from sleep or similiar. IDEE: ResetButton zerstoert nicht den Flash aber startet die Main() neu. 
 */

// buffer Value for User Input. 
#define INCREMENT_A    10
#define INCREMENT_B    1
int inputBuff = 0;

unsigned long startTimer;
unsigned long lastFreefallTime;
unsigned long exitLimit =10000;// 10 sekunden maximale Pause zwischen Reps
ManagedString delimiters("\n\r\t");
  
ManagedString dot (".");
ManagedString semi(";");

MicroBitFile myFile_creation   ("log.csv",CREATE);
//myFile_creation.close();

void powerOn(MicroBitEvent e){


}

/** sends the logged File via Serial UART (or Bluetooth UART) should start on a command from the Serial Monitor for example "s" messageBus needs to listen to it.s 

 */
void sendFile(){
    //  int FilePosition_end = myFile_loop.getPosition();
    //     myFile_loop.setPosition(0);
    //     ManagedString readString(myFile_loop.read(FilePosition_end));
    //     uBit.serial.send(readString);
        }



/**sets the Accelerometer up. setup accRange,accPeriod. 
 * @code
 * setup(8,2); // sets the Range to 8g and the sample Period to ~2 ms or 500Hz. (actually 400Hz)
 * 
 *  
 * 
 */
void setup(int accRange, int accPeriod){
uBit.accelerometer.setRange(accRange);
uBit.accelerometer.setPeriod(accPeriod);
}
/** User can input via Buttons a Number eg. exercise ID or exercise Load.
 * 
 */

// int addOnButton(value_to_set){
//     int input=0;

//     if (uBit.ButtonA.isPressed())
//         input=input+10;
//     if (uBit.ButtonB.isPressed())
//         input=input+1;

// return input;
// }

void onButtonPressed(MicroBitEvent e){
inputBuff = 0;
if (e.source == MICROBIT_ID_BUTTON_A){
        if (e.value == MICROBIT_BUTTON_EVT_CLICK)
        inputBuff = inputBuff + INCREMENT_A;
        if (e.value == MICROBIT_BUTTON_EVT_HOLD)
        inputBuff = inputBuff - INCREMENT_A;
    }
if (e.source == MICROBIT_ID_BUTTON_B){
        if (e.value == MICROBIT_BUTTON_EVT_CLICK)
        inputBuff = inputBuff + INCREMENT_B;
        if (e.value == MICROBIT_BUTTON_EVT_HOLD)
        inputBuff = inputBuff - INCREMENT_B;
    }

}

/**input function via Button [A,B]: [+-INCREMENT_A, +-INCREMENT_B]. Returns via Multibutton AB or touching P2 pressed.
 *  
 */
int input(char arg){
    int ret = 0;
    

    if(arg=='r'){
    lastFreefallTime = uBit.systemTime();
    while (0==uBit.buttonAB.isPressed() and (lastFreefallTime - startTimer) < exitLimit){
        ret = ret + inputBuff;
        inputBuff = 0;
        uBit.display.printAsync(ret,250);
        uBit.sleep(40);
    //    uBit.display.clear();
    }
    }

    if (arg=='l'){
    while (0==uBit.buttonAB.isPressed() and 0==uBit.io.P2.isTouched()){
        ret = ret + inputBuff;
        inputBuff = 0;
        uBit.display.printAsync(ret,250);
        uBit.sleep(40);
    //    uBit.display.clear();
    }
    }
    return ret;
}

/** counts the repetitions of the exercise an returns an int after time t of no accelerometer change (~1g)
 * 
 */
void onFreeFall(MicroBitEvent e){

inputBuff = inputBuff + 1;
lastFreefallTime = uBit.systemTime();
uBit.serial.send("FREEFALL");
}



 
// int arraySize = 10;
// int acc[4][arraySize];

//     while(exit_timer<exitLimit){
//         for(int i=0;i<arraySize;i++){
        
//         acc[0][i]=uBit.accelerometer.getX();
//         acc[1][i]=uBit.accelerometer.getY();
//         acc[2][i]=uBit.accelerometer.getZ();
//         acc[3][i]=acc[0][i]*acc[0][i]+acc[1][i]*acc[1][i]+acc[2][i]*acc[2][i];

//         for(int j=0;j<=3;j++){
//             uBit.serial.printf("acc %d  ",j);
//             uBit.serial.send(acc[j][i]);
//             uBit.serial.printf("\n");
//             uBit.serial.printf("Time: %d", uBit.systemTime());
//             }
        

//         }    


/**saves the data to flash and closes or flushes the file. 
 */ 
void save(){


}

/**loads the file with the with the correct date written in the variable /struct date.
 */
void load(){

}




/**show the Number of Reps on the Screen. Mode: Dots/ Number.
 */
void screenReps(int reps, int Mode){
}
/** Funktion um eine Eingabeaufforderung an den serial zu senden und die Antwort in einem ManagedString zu übergeben.
 */
ManagedString serialEingabe(ManagedString question){
    uBit.serial.send(question);
    ManagedString answer(uBit.serial.readUntil(delimiters));
    return answer;
}

// ManagedString getDate(){
//     ManagedString year  (serialEingabe(ManagedString("year?\n")));
//     ManagedString month (serialEingabe(ManagedString("month?\n")));
//     ManagedString day   (serialEingabe(ManagedString("day?\n")));
//     ManagedString date_total (year+semi+month+semi+day+semi);
// return date_total;
// 
// }
int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();


    // Insert your code here!
    uBit.display.scroll("!");
    setup(8,1);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A,MICROBIT_EVT_ANY,onButtonPressed);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B,MICROBIT_EVT_ANY,onButtonPressed);
    uBit.messageBus.listen(MICROBIT_ID_GESTURE,MICROBIT_ACCELEROMETER_EVT_6G,onFreeFall);
  //  uBit.messageBus.listen(MICROBIT_ID_SERIAL,MICROBIT_EVT_ANY,sendFile);
    uBit.display.scroll("L:");
    
    ManagedString format ("year;month;day;ID;load;reps\n");
    myFile_creation.append(format);
    myFile_creation.close();
    ManagedString year  (serialEingabe(ManagedString("year?\n")));
    ManagedString month (serialEingabe(ManagedString("month?\n")));
    ManagedString day   (serialEingabe(ManagedString("day?\n")));

    uBit.serial.send(year+dot+month+dot+day);
    
    int last;
    int reps;
    int ID;

                //TODO: onFreeFall soll nur "listen" wenn man inn der Reps Schleife ist.
                //TODO: FileWrite und Read implementieren. siehe andere Programme

    while(1){
        MicroBitFile myFile_loop   ("log.csv",CREATE);
        ID = input('l');
        uBit.serial.printf("ID: %d  \n",ID);
        uBit.sleep(1000);
        last = input('l');  
        uBit.serial.printf("LAST: %d    \n",last);
        uBit.sleep(1000);
        startTimer=uBit.systemTime();
        reps = input('r');
        uBit.sleep(1000);
        uBit.serial.printf("Reps: %d    \n",reps);
        myFile_loop.append(year+semi+month+semi+day+semi); 
        myFile_loop.append(ManagedString(ID)+semi+ManagedString(last)+semi+ManagedString(reps)+ManagedString("\n"));
        
        if(uBit.serial.isReadable()==1) //stürzt ab 
        {
        

        if(serialEingabe("log eingeben")==ManagedString("log")){
        int FilePosition_end = myFile_loop.getPosition();// Datei über Serial senden als Eventhandler auf folgendes Evt: * Will generate an event with the ID: MICROBIT_ID_SERIAL and the value MICROBIT_SERIAL_EVT_DELIM_MATCH.
        myFile_loop.setPosition(0);
        ManagedString readString(myFile_loop.read(FilePosition_end));
        uBit.serial.send(readString);  
        }

        myFile_loop.close();

        
    }

    
    
    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
}   

