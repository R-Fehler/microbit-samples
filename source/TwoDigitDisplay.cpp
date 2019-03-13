#include "TwoDigitDisplay.h"

#define ROWS 5
#define ALPHA 255
TwoDigitDisplay::TwoDigitDisplay() {
  // uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);
  //   for (int i = 0; i < 10; i++) {
  //     imgs_ze[i] = MicroBitImage(5, 5, ze[i]);
  //     imgs_ei[i] = MicroBitImage(5, 5, ei[i]);
  //   }
}
MicroBitImage TwoDigitDisplay::createImage(int value) {
  // 5x5 led matrix
  uint8_t buffer[25] = {0};
  if (value > 99) {
	  //leds on for the third
	  uBit.io.P1.setDigitalValue(1);
	  if (value > 199) {
		  uBit.io.P8.setDigitalValue(1);
	  }
  
	  
  }
  if (value < 200) {
	  uBit.io.P8.setDigitalValue(0);
	  if (value < 100) {
	  uBit.io.P1.setDigitalValue(0);

	  }

  }
  value = value % 100;
  uint8_t first = value / 10;   // first digit
  uint8_t second = value % 10;  // second digit

  for (size_t i = 0; i < ROWS; i++)  // i==Row
  {
    if (bin_digits_first[first][i] & 0b10000) {
      buffer[i * 5] = ALPHA;
    }
    if (bin_digits_first[first][i] & 0b01000) {
      buffer[i * 5 + 1] = ALPHA;
    }
    if (bin_digits_second[second][i] & 0b00010) {
      buffer[i * 5 + 3] = ALPHA;
    }

    if (bin_digits_second[second][i] & 0b00001) {
      buffer[i * 5 + 4] = ALPHA;
    }
  }

  // uBit.display.print(ergebnis);
  return MicroBitImage(5, 5, buffer);
}
TwoDigitDisplay::~TwoDigitDisplay() {}
