#include "pins_arduino.h" // Arduino pre-1.0 needs this

#define CALIBRATE_STEPS 200
#define BUTTONS 26
#define BUTTON_START 24
#define DEBOUNCE_COUNT 5

char button_name[] = {
  'A',
  ' ',
  'O',
  ' ',
  'E',
  ' ',
  '#',
  'U',
  'P',
  'T',
  'f',  // final
  'H',
  'l',  // final
  'p',  // final
  'd',  // final
  't',  // final
  'K',
  'S',
  'R',
  'W',
  'r',  // final
  '*',
  'g',  // final
  'b',  // final
  'z',  // final
  's',  // final
};

uint8_t tally[BUTTONS];
int count[BUTTONS];
uint8_t state[BUTTONS];
uint8_t stroke[BUTTONS];
int stroke_count = 0;
int last_tm;

static void readCapacitivePins(int start, int count, uint8_t* out) {
  int i;
  
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  for (i = 0; i < count; i++) {
    port = portOutputRegister(digitalPinToPort(start + i));
    ddr = portModeRegister(digitalPinToPort(start + i));
    bitmask = digitalPinToBitMask(start + i);
    pin = portInputRegister(digitalPinToPort(start + i));
  }
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delayMicroseconds(500);
  uint8_t SREG_old = SREG; //back up the AVR Status Register
  for (i = 0; i < count; i++) {
    port = portOutputRegister(digitalPinToPort(start + i));
    ddr = portModeRegister(digitalPinToPort(start + i));
    bitmask = digitalPinToBitMask(start + i);
    pin = portInputRegister(digitalPinToPort(start + i));

    // Prevent the timer IRQ from disturbing our measurement
    noInterrupts();

    // Make the pin an input with the internal pull-up on
    *ddr &= ~(bitmask);
    *port |= bitmask;

    // Now see how long the pin to get pulled up. This manual unrolling of the loop
    // decreases the number of hardware cycles between each read of the pin,
    // thus increasing sensitivity.
    uint8_t cycles = 17;
    if (*pin & bitmask) { cycles =  0;}
    else if (*pin & bitmask) { cycles =  1;}
    else if (*pin & bitmask) { cycles =  2;}
    else if (*pin & bitmask) { cycles =  3;}
    else if (*pin & bitmask) { cycles =  4;}
    else if (*pin & bitmask) { cycles =  5;}
    else if (*pin & bitmask) { cycles =  6;}
    else if (*pin & bitmask) { cycles =  7;}
    else if (*pin & bitmask) { cycles =  8;}
    else if (*pin & bitmask) { cycles =  9;}
    else if (*pin & bitmask) { cycles = 10;}
    else if (*pin & bitmask) { cycles = 11;}
    else if (*pin & bitmask) { cycles = 12;}
    else if (*pin & bitmask) { cycles = 13;}
    else if (*pin & bitmask) { cycles = 14;}
    else if (*pin & bitmask) { cycles = 15;}
    else if (*pin & bitmask) { cycles = 16;}

    // End of timing-critical section; turn interrupts back on if they were on before, or leave them off if they were off before
    SREG = SREG_old;
  
    // Discharge the pin again by setting it low and output
    //  It's important to leave the pins low if you want to 
    //  be able to touch more than 1 sensor at a time - if
    //  the sensor is left pulled high, when you touch
    //  two sensors, your body will transfer the charge between
    //  sensors.
    *port &= ~(bitmask);
    *ddr  |= bitmask;

    out[i] = cycles;
  }
}

static void setup_digit(void) {
  int i;
  for (i = 0; i < 9; i++) {
    pinMode(A0 + i, OUTPUT);
  }
  digitalWrite(A8, HIGH);
}

static void write_digit(uint8_t value) {
  int i;
  for (i = 0; i < 8; i++) {
    digitalWrite(A0 + i, value & 1 ? LOW : HIGH);
    value >>= 1;
  }
}

static void setup_pixels(void) {
  int i;
  for (i = 0; i < 12; i++) {
    pinMode(i + 2, OUTPUT);
    digitalWrite(i + 2, LOW);
  }
}

static uint8_t pixel_row[] = { 13, 10, 12, 5, 11, 3, 2 };
static uint8_t pixel_column[] = { 8, 9, 4, 7, 6 };

static void write_pixels(const uint8_t* pix) {
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

static uint8_t letters[] = {
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

static uint8_t im[1024];

static char message[] = "The quick brown fox jumps over the lazy dog.";

static void calibrate() {
  int i, j;
  
  stroke_count = 0;
  for(i = 0; i < BUTTONS; i++) {
    if (button_name[i] == ' ') continue;
    tally[i] = 0;
    count[i] = 0;
    state[i] = 0;
    stroke[i] = 0;
    pinMode(i + BUTTON_START, OUTPUT);
  }
  for (j = 0; j < CALIBRATE_STEPS; j++) {
    uint8_t a[BUTTONS];
    readCapacitivePins(BUTTON_START, BUTTONS, a);
    for (i = 0; i < BUTTONS; i++) {
      if (button_name[i] == ' ') continue;
      if (a[i] > tally[i]) {
        tally[i] = a[i];
      }
    }
  }
  Serial.println("ready");  
}

void setup() {
  setup_digit();
  setup_pixels();
  Serial.begin(9600);
  int i, j;

  for (i = 0; i < sizeof(im); i++) {
    if (i % 6 == 5) {
      im[i] = 0;
    } else {
      uint8_t ch = message[(i / 6) % sizeof(message)] - 32;
      im[i] = letters[ch * 5 + (i % 6)];
    }
  }

  calibrate();
}

int base = 0;
int last = 0;

void loop() {
  int i;
  uint8_t a[BUTTONS];
  readCapacitivePins(BUTTON_START, BUTTONS, a);
  for (i = 0; i < BUTTONS; i++) {
    if (button_name[i] == ' ') continue;
    uint8_t val = a[i] > tally[i];
    if (val != state[i]) {
      if (count[i] < DEBOUNCE_COUNT) {
        count[i]++;
        if (count[i] == DEBOUNCE_COUNT) {
          state[i] = val;
          if (val) {
            stroke[i] = 1;
            stroke_count++;
          } else {
            stroke_count--;
            if (stroke_count == 0) {
              char str[BUTTONS + 1];
              int pos = 0;
              for (int k = 0; k < BUTTONS; k++) {
                if (button_name[k] == ' ') continue;
                if (stroke[k] == 0) continue;
                stroke[k] = 0;
                str[pos++] = button_name[k];
              }
              str[pos] = 0;
              Serial.println(str);
              if (strcmp(str, "#*") == 0) {
                calibrate();
              }
            }
          }
        }
      } else {
        count[i] = 0;
      }
    }
  }

  write_digit(digits[stroke_count]);
  write_pixels(im + base);
  int t = millis();
  if (t - last > 100) {
    last = t;
    base++;
  }
  if (base >= sizeof(im) - 5) base = 0;
}

