#include "MicroBit.h"
#include "MicroBitFile.h"
#include "MicroBitFileSystem.h"
extern MicroBit uBit;
void readToSerial(ManagedString fn);
int settingsReadNumber(ManagedString name);
void appendLine(ManagedString fn, ManagedString t);
