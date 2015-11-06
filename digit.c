#include "digit.h"

void digit_setup(void) {
  int i;
  for (i = 0; i < 9; i++) {
    pinMode(A0 + i, OUTPUT);
  }
  digitalWrite(A8, HIGH);
}

void digit_set_raw(uint8_t value) {
  int i;
  for (i = 0; i < 8; i++) {
    digitalWrite(A0 + i, value & 1 ? LOW : HIGH);
    value >>= 1;
  }
}

static uint8_t digits[] = {
  0xe7,  // 0
  0x44,  // 1
  0xd3,  // 2
  0xd6,  // 3
  0x74,  // 4
  0xb6,  // 5
  0xb7,  // 6
  0xe4,  // 7
  0xf7,  // 8
  0xf6,  // 9
  0xf5,  // a
  0x37,  // b
  0xa3,  // c
  0x57,  // d
  0xb3,  // e
  0xb1,  // f
  0xa7,  // g
  0x75,  // h
  0x47,  // j
  0x23,  // l
  0x17,  // o
  0xf1,  // p
  0x11,  // r
  0x31,  // t
  0x67,  // u
  0x76,  // y
};

void digit_set(int value) {
  digit_set_raw(digits[value]);
}
