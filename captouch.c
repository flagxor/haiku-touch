#include "captouch.h"

static uint8_t buttons[CAPTOUCH_BUTTONS] = {
  24, 26, 28, 30, 31, 32, 33, 34, 35, 36, 37,
  38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 
};

void captouch_setup(void) {
  for (int i = 0; i < CAPTOUCH_BUTTONS; i++) {
    pinMode(buttons[i], OUTPUT);
  }
}

void captouch_read(uint8_t *out) {
  int i;
  
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  for (i = 0; i < CAPTOUCH_BUTTONS; i++) {
    port = portOutputRegister(digitalPinToPort(buttons[i]));
    ddr = portModeRegister(digitalPinToPort(buttons[i]));
    bitmask = digitalPinToBitMask(buttons[i]);
    // Discharge the pin first by setting it low and output
    *port &= ~(bitmask);
    *ddr  |= bitmask;
  }

  delayMicroseconds(500);

  volatile uint8_t* pin;
  uint8_t SREG_old = SREG; // back up the AVR Status Register
  for (i = 0; i < CAPTOUCH_BUTTONS; i++) {
    port = portOutputRegister(digitalPinToPort(buttons[i]));
    ddr = portModeRegister(digitalPinToPort(buttons[i]));
    bitmask = digitalPinToBitMask(buttons[i]);
    pin = portInputRegister(digitalPinToPort(buttons[i]));

    // Prevent the timer IRQ from disturbing our measurement
    noInterrupts();

    // Make the pin an input with the internal pull-up on
    *ddr &= ~(bitmask);
    *port |= bitmask;

    // Now see how long the pin to get pulled up. This manual unrolling of the
    // loop decreases the number of hardware cycles between each read of the
    // pin, thus increasing sensitivity.
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

    // End of timing-critical section; turn interrupts back on if they were
    // on before, or leave them off if they were off before
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
