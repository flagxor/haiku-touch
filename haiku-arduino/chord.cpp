#include "chord.h"
#include "captouch.h"

#define CALIBRATE_STEPS 200
#define DEBOUNCE_COUNT 5

static uint8_t tally[CAPTOUCH_BUTTONS];
static uint8_t count[CAPTOUCH_BUTTONS];
static long state;
static long stroke;
static uint8_t keys_down;

void chord_calibrate(void) {
  int i, j;
  
  keys_down = 0;
  stroke = 0;
  state = 0;
  for(i = 0; i < CAPTOUCH_BUTTONS; i++) {
    tally[i] = 5;
    count[i] = 0;
  }
  for (j = 0; j < CALIBRATE_STEPS; j++) {
    uint8_t keys[CAPTOUCH_BUTTONS];
    captouch_read(keys);
    for (i = 0; i < CAPTOUCH_BUTTONS; i++) {
      if (keys[i] > tally[i]) {
        tally[i] = keys[i];
      }
    }
  }
}

void chord_dump(void) {
  for (int i = 0; i < CAPTOUCH_BUTTONS; i++) {
    Serial.print(i);
    Serial.print(":");
    Serial.print(tally[i]);
    Serial.print(" ");
  }
  Serial.println();
}

uint8_t chord_keys_down(void) {
  return keys_down;
}

long chord_read(void) {
  uint8_t keys[CAPTOUCH_BUTTONS];
  int i;
  long ret;

  captouch_read(keys);
  for (i = 0; i < CAPTOUCH_BUTTONS; i++) {
    uint8_t val = keys[i] > tally[i];
    if (val != ((state & (1L<<i)) != 0L)) {
      if (count[i] >= DEBOUNCE_COUNT) {
        count[i] = 0;
        continue;
      }
      count[i]++;
      if (count[i] == DEBOUNCE_COUNT) {
        if (val) {
          state |= (1L<<i);
        } else {
          state &= ~(1L<<i);
        }
        if (val) {
          stroke |= (1L<<i);
          keys_down++;
        } else {
          keys_down--;
        }
      }
    }
  }
  if (keys_down > 0) {
    return 0;
  } else {
    ret = stroke;
    stroke = 0;
    return ret;
  }
}
