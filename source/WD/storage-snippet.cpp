#include "MicroBit.h"

MicroBit uBit;
// 21 Keys can be stored (1kbyte)
int main() {
  uBit.init();

  KeyValuePair* firstTime = uBit.storage.get("boot");

  int stored;

  if (firstTime == NULL) {
    // this is the first boot after a flash. Store a value!
    stored = 1234;
    uBit.storage.put("boot", (uint8_t*)&stored, sizeof(int));
    uBit.display.scroll("Stored!");
  } else {
    // this is not the first boot, scroll our stored value.
    memcpy(&stored, firstTime->value, sizeof(int));
    delete firstTime;
    uBit.display.scroll(stored);
  }
}
