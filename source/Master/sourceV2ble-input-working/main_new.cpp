#include "Files.h"
#include "MicroBit.h"
#include "MicroBitUARTService.h"
#include "Set.h"
#include "TwoDigitDisplay.h"
#define INCREMENT_A 10
#define INCREMENT_B 1

MicroBit uBit;
MicroBitUARTService *uart;

TwoDigitDisplay twodigit;
ManagedString filename;
Set meinsatz;
ManagedString delimiter("\n");
ManagedString setting;
int inputBuff = 0;
int terminator = 0;
int connected = 0;

void onConnected(MicroBitEvent) {
  uBit.display.scroll("C");

  connected = 1;
}

void onDisconnected(MicroBitEvent) {
  uBit.display.scroll("D");
  connected = 0;
}

void printfile(MicroBitEvent e) { readToSerial(ManagedString(filename)); }
void sendfile(MicroBitEvent e) { readtoBLE(filename); }

int int_from_serial() {
  return atoi(uBit.serial.readUntil(delimiter).toCharArray());
}

ManagedString string_from_serial() { return uBit.serial.readUntil(delimiter); }
void inputButton(MicroBitEvent e) {
  inputBuff = 0;
  if (e.source == MICROBIT_ID_BUTTON_A) {
    if (e.value == MICROBIT_BUTTON_EVT_CLICK)
      inputBuff = inputBuff + INCREMENT_A;
    if (e.value == MICROBIT_BUTTON_EVT_HOLD)
      inputBuff = inputBuff - INCREMENT_A;
  }
  if (e.source == MICROBIT_ID_BUTTON_B) {
    if (e.value == MICROBIT_BUTTON_EVT_CLICK)
      inputBuff = inputBuff + INCREMENT_B;
    if (e.value == MICROBIT_BUTTON_EVT_HOLD)
      inputBuff = inputBuff - INCREMENT_B;
  }
}

ManagedString string_from_ble() { return uart->readUntil(delimiter); }

int input() {
  int ret = 0;

  while (0 == uBit.buttonAB.isPressed() and 0 == uBit.io.P2.isTouched()) {
    ret = ret + inputBuff;
    inputBuff = 0;
    // uBit.display.printAsync(ret);
    uBit.display.printAsync(twodigit.createImage(ret));
    uBit.sleep(60);
    //    uBit.display.clear();
  }

  return ret;
}
int input(ManagedString message) {
  int ret = 0;
  uBit.display.scrollAsync(message);
  while (0 == uBit.buttonA.isPressed() and 0 == uBit.buttonB.isPressed()) {
    uBit.sleep(100);
  }

  while (0 == uBit.buttonAB.isPressed() and 0 == uBit.io.P2.isTouched()) {
    ret = ret + inputBuff;
    inputBuff = 0;
    // uBit.display.printAsync(ret);
    uBit.display.printAsync(twodigit.createImage(ret));
    uBit.sleep(60);
    //    uBit.display.clear();
  }

  return ret;
}
int input(const char *message) {
  int ret = 0;
  uBit.display.scrollAsync(message);
  while (0 == uBit.buttonA.isPressed() and 0 == uBit.buttonB.isPressed()) {
    uBit.sleep(100);
  }

  while (0 == uBit.buttonAB.isPressed() and 0 == uBit.io.P2.isTouched()) {
    ret = ret + inputBuff;
    inputBuff = 0;
    // uBit.display.printAsync(ret);
    uBit.display.printAsync(twodigit.createImage(ret));
    uBit.sleep(60);
    //    uBit.display.clear();
  }
  uBit.display.clear();
  uBit.sleep(200);
  return ret;
}

void serial_input(MicroBitEvent e) {
  ManagedString buffer;
  buffer = string_from_serial();
  uBit.serial.printf("input: %s", buffer.toCharArray());
  // meinsatz.set_name(buffer);
}

void terminate(MicroBitEvent e) { terminator = 1; }
int main() {
  // Initialise the micro:bit runtime.
  // delimiter = "\n";
  // TODO:
  /*
  automatisches zählen der reps
  (Beschleunigungswerte speichern/senden)
  Key Values speichern für Trainingsmittelkatalog
  */
  uBit.init();
  filename = "log.csv";  // file to store the training log
  uint8_t *stor;  // storing the setting if microbit has started , Warning
                  // uninitialized
  *stor = 1;      // dereferenced ptr. inhalt ist 1
  setting = "fileexist";  // name des settings,storage key value pair
  uBit.serial.eventOn(delimiter);

  uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED,
                         onConnected);  // entscheide ob eingabe über ble
  uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED,
                         onDisconnected);
  uart = new MicroBitUARTService(*uBit.ble, 32, 32);  // ble service
  uBit.display.scroll("UART");                        // info

  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK,
                         inputButton);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK,
                         inputButton);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_LONG_CLICK,
                         terminate);
  uBit.messageBus.listen(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_DELIM_MATCH,
                         serial_input);
  if (NULL == uBit.storage.get(setting)) {
    uBit.storage.put(setting, stor, sizeof(stor));
    appendLine(filename, ManagedString("date,name,id,weight,reps"));
  };  // prueft ob der microbit zum ersten mal in das file schreibt.
  // dann wird die header dafür geschrieben. (csv format header)
  uBit.display.scroll(ManagedString("BLE!"));
  uBit.sleep(2000);  // warten auf Verbindung mit Smartphone
  // uBit.serial.send(
  //     ManagedString("gebe die ID, das Gewicht, die Reps, ein! via "
  //                   "buttonsaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
  // uBit.serial.send("datum:");

  // wenn Verbunden lese das Datum ein (von Handy App)
  if (connected == 1) {
    uart->send(ManagedString("Datum: 3x T,M,J newline"));
    meinsatz.set_date(atoi(string_from_ble().toCharArray()),
                      atoi(string_from_ble().toCharArray()),
                      atoi(string_from_ble().toCharArray()));
    uart->send(ManagedString("ID, gewicht, reps"));
  }
  // ansonsten über Hand eingabe
  else {
    meinsatz.set_date(input("T"), input("M"), input("J"));
  }
  // uBit.serial.printf("gebe datum ein");

  // meinsatz.set_date(int_from_serial(), int_from_serial(),
  // int_from_serial());

  // automatischer / manueller Input der Sets
  while (1) {
    uBit.display.scroll(ManagedString("!!"));
    meinsatz.set_ID(input());
    uBit.sleep(500);

    meinsatz.set_weight(input());
    uBit.sleep(500);

    meinsatz.set_reps(input());
    uBit.sleep(500);
    uBit.display.clear();
    uBit.serial.printf(
        "gebe name ein! halte AB gedrückt \n um das training zu beenden");
    uBit.sleep(100);
    // meinsatz.set_name(string_from_serial());

    meinsatz.write_to_file(filename);  // schreibe den Satz ins file
    uBit.sleep(500);
    // prüfe ob Set Loop verlassen werden soll.
    if (terminator == 1) {
      terminator = 0;
      break;
    }
  }
  uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, inputButton);
  uBit.messageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY, inputButton);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK,
                         printfile);  // gebe file via serial aus
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK,
                         sendfile);  // gebe via BLE aus
  readtoBLE(filename);               // gebe via BLE aus
  uBit.serial.send("hello");

  uBit.sleep(10000);
  // Insert your code here!

  // If main exits, there may still be other fibers running or registered
  // event handlers etc. Simply release this fiber, which will mean we enter
  // the scheduler. Worse case, we then sit in the idle task forever, in a
  // power efficient sleep.
  release_fiber();
}
