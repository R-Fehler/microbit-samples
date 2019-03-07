
#include "MicroBit.h"
#include "MicroBitFile.h"
#include "MyFiles.h"
#define EOT 4
#define SERBUF 32
MicroBit uBit;

int main() {
  uBit.init();
  MicroBitFileSystem::defaultFileSystem->createDirectory("log/");
  ManagedString filename = "filename";
  ManagedString payload;
  char recv[32];

  // // uBit.display.scroll("HELLO ");
  // uBit.serial.printf("size char %d, size ui8:%d", sizeof(char),
  //                    sizeof(uint8_t));
  // char buf[32];
  // int i = 0;
  // int c;

  // // do {
  // //   c = uBit.serial.getc();
  // //   buf[i] = c;
  // //   i++;
  // // } while (c != (int)'\n');

  // while ((c = uBit.serial.getc()) != (int)'\n') {
  //   buf[i] = c;
  //   i++;
  // }

  // buf[i] = '\0';
  // ManagedString input(buf);

  // uBit.serial.printf("input");
  // uBit.serial.printf(input.toCharArray());

  MicroBitFile myFile(filename, CREATE);
  myFile.close();
  while (1) {
    serial_to_file(filename);
    // myFile.append(input);
    // Insert your code here!
    // uBit.display.scroll("WORLD! :)");
    // myFile.flush();
    // myFile.setPosition(0);
    // myFile.close();  // muss closed werden damit readToSerial fnkt.
    int read = 0;
    /**
     * @brief liest die Datei in 32byte happen ein und schreibt sie in happen
     * auf dem serial port.
     *
     */
    uBit.serial.printf("recvd:");

    readToSerial(filename);
    // while ((read = myFile.read(recv, sizeof(recv) * sizeof(char))) > 0) {
    //   uBit.serial.send((uint8_t*)recv, read * sizeof(char), SYNC_SPINWAIT);
    // }
    uBit.serial.printf("recvd");
  }

  // If main exits, there may still be other fibers running or registered event
  // handlers etc. Simply release this fiber, which will mean we enter the
  // scheduler. Worse case, we then sit in the idle task forever, in a power
  // efficient sleep.
  release_fiber();
}
