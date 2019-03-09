#include "Files.h"
#include "MicroBit.h"
#include "MicroBitFile.h"
#include "MicroBitFileSystem.h"

void readToSerial(ManagedString fn) {
  MicroBitFile f(fn);
  char buf[32];
  int read = 0;
  while ((read = f.read(buf, sizeof(buf) * sizeof(char))) > 0) {
    uBit.serial.send((uint8_t *)buf, read * sizeof(char), SYNC_SPINWAIT);
  }
  f.close();
}
void readtoBLE(ManagedString fn) {
  MicroBitFile f(fn);
  char buf[8];
  int read = 0;
  while ((read = f.read(buf, sizeof(buf) * sizeof(char))) > 0) {
    // uBit.serial.send((uint8_t*)buf, read * sizeof(char), SYNC_SPINWAIT);
    uart->send((uint8_t *)buf, read * sizeof(char));
  }
  f.close();
}
int settingsReadNumber(ManagedString name) {
  MicroBitFileSystem::defaultFileSystem->createDirectory("settings");
  MicroBitFile f("settings/" + name, MB_READ);
  if (!f.isValid()) return -1;
  ManagedString v;
  ManagedString buff;
  do {
    buff = f.read(32);
    v = v + buff;
  } while (buff.length() > 0);
  return atoi(v.toCharArray());
}

void appendLine(ManagedString fn, ManagedString t) {
  MicroBitFile f(fn);
  f.append(t);
  f.append("\r\n");  // oder \n
  f.close();
}

void serial_to_file(ManagedString fn) {
  uBit.serial.printf("Bitte nur soviele chars auf einmal: %d ",
                     uBit.serial.getRxBufferSize());
  MicroBitFile f(fn);
  char buf[SERBUF];
  // int read = 0;
  // while ((read = f.read(buf, sizeof(buf) * sizeof(char))) > 0) {
  //   uBit.serial.send((uint8_t*)buf, read * sizeof(char), SYNC_SPINWAIT);
  // }
  char buffc[1];
  int i = 0;
  int c;
  while ((c = uBit.serial.read()) != ESCAPE_SEQ) {
    buffc[0] = c;
    // i++;
    // if (i != 0) {
    //   if (i % SERBUF == 0) {
    //     f.append(buf, sizeof(buf) * sizeof(char));
    //     i = 0;
    //   }
    // }
    f.append(buffc, sizeof(char) * sizeof(buffc));
  }
  f.close();
}