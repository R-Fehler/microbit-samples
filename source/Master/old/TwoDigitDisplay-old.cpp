#include "TwoDigitDisplay.h"
#include "MicroBitImage.h"

TwoDigitDisplay::TwoDigitDisplay() {
  // uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);
  //   for (int i = 0; i < 10; i++) {
  //     imgs_ze[i] = MicroBitImage(5, 5, ze[i]);
  //     imgs_ei[i] = MicroBitImage(5, 5, ei[i]);
  //   }
}
MicroBitImage TwoDigitDisplay::createImage(int wert) {
  MicroBitImage ergebnis(5, 5);
  ergebnis.paste(MicroBitImage(5, 5, ze[wert / 10 % 10]), 0, 0, 1);
  ergebnis.paste(MicroBitImage(5, 5, ei[wert % 10]), 0, 0, 1);
  // uBit.display.print(ergebnis);
  return ergebnis;
}
TwoDigitDisplay::~TwoDigitDisplay() {}
