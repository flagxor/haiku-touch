#include "pins_arduino.h"
#include "chord.h"
#include "digit.h"
#include "pixels.h"
#include "steno.h"

static char message[64] = "boot";
static int scroll = 0;
static int last_tm = 0;

void setup() {
  Serial.begin(9600);
  digit_setup();
  pixels_setup();
  chord_calibrate();
  strcpy(message, "ok");
}

void update(void) {
  int tm = millis();
  if (tm - last_tm > 70) {
    last_tm = tm;
    scroll++;
  }
  if (scroll >= strlen(message) * 6) {
    scroll = 0;
  }
}

void loop() {
  long ch = chord_read();
  if (ch) {
    steno_to_string(ch, message);
    Serial.println(message);
    if (strcmp(message, "#*") == 0) {
      delay(200);
      chord_calibrate();
      strcpy(message, "ok");
      Serial.println(message);
    } else if (strcmp(message, "*P") == 0) {
      chord_dump();
    }
  }
  digit_set(chord_keys_down());
  pixels_scroll(message, strlen(message), scroll);
  update();
}
