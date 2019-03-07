#if !defined(TWODIGITDISPLAY_H)
#define TWODIGITDISPLAY_H
#include "MicroBitImage.h"
class TwoDigitDisplay {
 private:
  const uint8_t bin_digits_second[10][5]{
      {
          0b00011,
          0b00011,
          0b00011,
          0b00011,
          0b00011,
      },
      {
          0b00001,
          0b00001,
          0b00001,
          0b00001,
          0b00001,
      },
      {
          0b00011,
          0b00001,
          0b00011,
          0b00010,
          0b00011,
      },
      {
          0b00011,
          0b00001,
          0b00011,
          0b00001,
          0b00011,
      },
      {
          0b00010,
          0b00010,
          0b00011,
          0b00001,
          0b00001,
      },
      {
          0b00011,
          0b00010,
          0b00011,
          0b00001,
          0b00011,
      },
      {
          0b00010,
          0b00010,
          0b00011,
          0b00011,
          0b00011,
      },
      {
          0b00011,
          0b00001,
          0b00001,
          0b00001,
          0b00001,
      },
      {
          0b00011,
          0b00011,
          0b00000,
          0b00011,
          0b00011,
      },
      {
          0b00011,
          0b00011,
          0b00011,
          0b00001,
          0b00001,
      },
  };

  const uint8_t bin_digits_first[10][5]{
      {
          0b11000,
          0b11000,
          0b11000,
          0b11000,
          0b11000,
      },
      {
          0b10000,
          0b10000,
          0b10000,
          0b10000,
          0b10000,
      },
      {
          0b11000,
          0b01000,
          0b11000,
          0b10000,
          0b11000,
      },
      {
          0b11000,
          0b01000,
          0b11000,
          0b01000,
          0b11000,
      },
      {
          0b10000,
          0b10000,
          0b11000,
          0b01000,
          0b01000,
      },
      {
          0b11000,
          0b10000,
          0b11000,
          0b01000,
          0b11000,
      },
      {
          0b10000,
          0b10000,
          0b11000,
          0b11000,
          0b11000,
      },
      {
          0b11000,
          0b01000,
          0b01000,
          0b01000,
          0b01000,
      },
      {
          0b11000,
          0b11000,
          0b00000,
          0b11000,
          0b11000,
      },
      {
          0b11000,
          0b11000,
          0b11000,
          0b01000,
          0b01000,
      },
  };

 public:
  MicroBitImage createImage(int value);
  TwoDigitDisplay();
  ~TwoDigitDisplay();
};
#endif