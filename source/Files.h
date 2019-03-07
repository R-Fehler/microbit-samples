#ifndef MYFILES
#define MYFILES

#include "MicroBit.h"
#include "MicroBitFile.h"
#include "MicroBitFileSystem.h"
#include "MicroBitUARTService.h"

#define SERBUF 32
#define ESCAPE_SEQ '\n'  //'\004'
extern MicroBit uBit;
extern MicroBitUARTService *uart;
void readToSerial(ManagedString fn);
int settingsReadNumber(ManagedString name);
void appendLine(ManagedString fn, ManagedString t);
void serial_to_file(ManagedString fn);
void readtoBLE(ManagedString fn);

#endif  // !MYFILES