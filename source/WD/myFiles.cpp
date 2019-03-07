#include "myFiles.h"
#include "MicroBit.h"
#include "MicroBitFile.h"
#include "MicroBitFileSystem.h"
void readToSerial(ManagedString fn) {
  MicroBitFile f(fn);
  char buf[32];
  int read = 0;
  while ((read = f.read(buf, sizeof(buf) * sizeof(char))) > 0) {
    uBit.serial.send((uint8_t*)buf, read * sizeof(char), SYNC_SPINWAIT);
  }
  f.close();
}

int settingsReadNumber(ManagedString name) {
  MicroBitFile f(name, MB_READ);
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
  f.append("\n");  // oder \r\n
  f.close();
}