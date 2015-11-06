#include "pixels.h"

void pixels_setup(void) {
  int i;
  for (i = 0; i < 12; i++) {
    pinMode(i + 2, OUTPUT);
    digitalWrite(i + 2, LOW);
  }
}

static uint8_t pixel_row[] = { 13, 10, 12, 5, 11, 3, 2 };
static uint8_t pixel_column[] = { 8, 9, 4, 7, 6 };

void pixels_refresh(const uint8_t* pix) {
  int i, j;

  for (j = 0; j < 5; j++) {
    digitalWrite(pixel_column[j], HIGH);
    uint8_t v;
    v = pix[j];
    for (i = 0; i < 7; i++) {
      if (v & 1) {
        digitalWrite(pixel_row[i], v&1 ? LOW : HIGH);
        delayMicroseconds(30);
        digitalWrite(pixel_row[i], HIGH);
      }
      v >>= 1;
    }
    digitalWrite(pixel_column[j], LOW);
  }
}

static uint8_t font[] = {
  0x00, 0x00, 0x00, 0x00, 0x00,  //
  
  0x00, 0x00, 0x7d, 0x00, 0x00,  // !
  0x00, 0x60, 0x00, 0x60, 0x00,  // "
  0x14, 0x7f, 0x14, 0x7f, 0x14,  // #
  0x12, 0x2a, 0x7f, 0x2a, 0x24,  // $
  0x61, 0x66, 0x08, 0x33, 0x43,  // %
  0x36, 0x49, 0x35, 0x0a, 0x11,  // &
  0x00, 0x00, 0x60, 0x00, 0x00,  // '
  0x00, 0x1c, 0x22, 0x41, 0x00,  // (
  0x00, 0x41, 0x22, 0x1c, 0x00,  // )
  0x2a, 0x1c, 0x7f, 0x1c, 0x2a,  // *
  0x08, 0x08, 0x7f, 0x08, 0x08,  // +
  0x08, 0x08, 0x08, 0x08, 0x08,  // -
  0x00, 0x01, 0x02, 0x00, 0x00,  // ,
  0x00, 0x00, 0x01, 0x00, 0x00,  // .
  0x01, 0x06, 0x08, 0x30, 0x40,  // /

  0x3e, 0x47, 0x49, 0x71, 0x3e,  // 0
  0x11, 0x21, 0x7f, 0x01, 0x01,  // 1
  0x23, 0x45, 0x49, 0x49, 0x31,  // 2
  0x22, 0x41, 0x49, 0x49, 0x36,  // 3
  0x78, 0x08, 0x08, 0x7f, 0x08,  // 4
  0x7a, 0x49, 0x49, 0x49, 0x46,  // 5
  0x3e, 0x49, 0x49, 0x49, 0x26,  // 6
  0x40, 0x40, 0x4f, 0x50, 0x60,  // 7
  0x36, 0x49, 0x49, 0x49, 0x36,  // 8
  0x32, 0x49, 0x49, 0x49, 0x3e,  // 9

  0x00, 0x00, 0x14, 0x00, 0x00,  // :
  0x00, 0x02, 0x14, 0x00, 0x00,  // ;
  0x08, 0x14, 0x22, 0x22, 0x41,  // <
  0x14, 0x14, 0x14, 0x14, 0x14,  // =
  0x41, 0x22, 0x22, 0x14, 0x08,  // >
  0x30, 0x40, 0x4d, 0x48, 0x30,  // ?
  
  0x3e, 0x41, 0x5d, 0x55, 0x3c,  // @

  0x1f, 0x28, 0x48, 0x28, 0x1f,  // A
  0x7f, 0x49, 0x49, 0x49, 0x36,  // B
  0x3e, 0x41, 0x41, 0x41, 0x22,  // C
  0x7f, 0x41, 0x41, 0x41, 0x3e,  // D
  0x7f, 0x49, 0x49, 0x49, 0x41,  // E
  0x7f, 0x48, 0x48, 0x48, 0x40,  // F
  0x3e, 0x41, 0x49, 0x49, 0x0e,  // G
  0x7f, 0x08, 0x08, 0x08, 0x7f,  // H
  0x00, 0x41, 0x7f, 0x41, 0x00,  // I
  0x06, 0x01, 0x01, 0x01, 0x7e,  // J
  0x7f, 0x08, 0x14, 0x22, 0x41,  // K
  0x7f, 0x01, 0x01, 0x01, 0x01,  // L
  0x7f, 0x20, 0x10, 0x20, 0x7f,  // M
  0x7f, 0x30, 0x08, 0x06, 0x7f,  // N
  0x3e, 0x41, 0x41, 0x41, 0x3e,  // O
  0x7f, 0x48, 0x48, 0x48, 0x30,  // P
  0x3e, 0x41, 0x45, 0x42, 0x3d,  // Q
  0x7f, 0x48, 0x4c, 0x4a, 0x31,  // R
  0x32, 0x49, 0x49, 0x49, 0x26,  // S
  0x40, 0x40, 0x7f, 0x40, 0x40,  // T
  0x7e, 0x01, 0x01, 0x01, 0x7e,  // U
  0x78, 0x06, 0x01, 0x06, 0x78,  // V
  0x7f, 0x02, 0x04, 0x02, 0x7f,  // W
  0x41, 0x36, 0x08, 0x36, 0x41,  // X
  0x40, 0x30, 0x0f, 0x30, 0x40,  // Y
  0x41, 0x47, 0x49, 0x71, 0x41,  // Z

  0x00, 0x7f, 0x41, 0x41, 0x00,  // [
  0x40, 0x30, 0x08, 0x06, 0x01,  // \
  0x00, 0x41, 0x41, 0x7f, 0x00,  // ]
  0x10, 0x20, 0x40, 0x20, 0x10,  // ^
  0x01, 0x01, 0x01, 0x01, 0x01,  // _
  0x00, 0x00, 0x40, 0x20, 0x00,  // `

  0x02, 0x15, 0x15, 0x15, 0x0f,  // a
  0x7f, 0x09, 0x09, 0x09, 0x06,  // b
  0x0e, 0x11, 0x11, 0x11, 0x11,  // c
  0x06, 0x09, 0x09, 0x09, 0x7f,  // d
  0x0e, 0x15, 0x15, 0x15, 0x0c,  // e
  0x00, 0x08, 0x3f, 0x48, 0x00,  // f
  0x09, 0x15, 0x15, 0x15, 0x0e,  // g
  0x7f, 0x08, 0x08, 0x08, 0x07,  // h
  0x00, 0x00, 0x17, 0x00, 0x00,  // i
  0x02, 0x01, 0x16, 0x00, 0x00,  // j
  0x7f, 0x04, 0x0a, 0x0a, 0x11,  // k
  0x00, 0x00, 0x7e, 0x01, 0x00,  // l
  0x1f, 0x10, 0x0f, 0x10, 0x0f,  // m
  0x1f, 0x10, 0x10, 0x10, 0x0f,  // n
  0x0e, 0x11, 0x11, 0x11, 0x0e,  // o
  0x1f, 0x14, 0x14, 0x14, 0x08,  // p
  0x08, 0x14, 0x14, 0x14, 0x1f,  // q
  0x1f, 0x10, 0x10, 0x10, 0x08,  // r
  0x08, 0x15, 0x15, 0x15, 0x02,  // s
  0x08, 0x08, 0x1e, 0x09, 0x09,  // t
  0x1e, 0x01, 0x01, 0x01, 0x1f,  // u
  0x18, 0x06, 0x01, 0x06, 0x18,  // v
  0x1e, 0x01, 0x1e, 0x01, 0x1e,  // w
  0x11, 0x0a, 0x04, 0x0a, 0x11,  // x
  0x18, 0x05, 0x05, 0x05, 0x1e,  // y
  0x11, 0x13, 0x15, 0x19, 0x11,  // z

  0x00, 0x08, 0x36, 0x41, 0x00,  // {
  0x00, 0x00, 0x77, 0x00, 0x00,  // |
  0x00, 0x41, 0x36, 0x08, 0x00,  // }
  0x08, 0x10, 0x08, 0x04, 0x08,  // ~
};

void pixels_scroll(const char* message, int len, int scroll) {
  uint8_t im[5];

  for (int i = 0; i < 5; i++) {
    int at = scroll + i;
    int pos = (at/ 6) % (len + 1);
    int pos1 = at % 6;
    if (pos1 == 5) {
      im[i] = 0;
    } else {
      if (pos == len) {
        im[i] = 0;
      } else {
        im[i] = font[(message[pos] - ' ') * 5 + pos1];
      }
    }
  }
  pixels_refresh(im);
}
