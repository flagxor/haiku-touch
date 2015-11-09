#include "pins_arduino.h"
#include "chord.h"
#include "digit.h"
#include "pixels.h"
#include "steno.h"

static char message[256] = "boot";
static int message_length = 4;
static char incoming[256] = "";
static int incoming_length = 0;
static int scroll = 0;
static int last_tm = 0;

void update_message() {
  message_length = strlen(message);
  scroll = 0;
}

void set_message(const char *m) {
  strcpy(message, m);
  update_message();
}

void setup() {
  Serial.begin(9600);
  digit_setup();
  pixels_setup();
  chord_calibrate();
  set_message("ok");
}

void update(void) {
  int tm = millis();
  if (tm - last_tm > 70) {
    last_tm = tm;
    scroll++;
  }
  if (scroll >= pixels_scroll_width(message_length)) {
    scroll = 0;
  }
}

void read_output(void) {
  if (Serial.available() > 0) {
    char ch = Serial.read();
    if (ch >= ' ' && ch < 127 && incoming_length < sizeof(incoming) - 1) {
      incoming[incoming_length++] = ch;
    } else if (ch == 10) {
      incoming[incoming_length] = '\0';
      set_message(incoming);
      incoming_length = 0;
    }
  }
}

void loop() {
  read_output();
  long ch = chord_read();
  if (ch) {
    steno_to_string(ch, message);
    update_message();
    Serial.println(message);
    if (strcmp(message, "#*") == 0) {
      delay(200);
      chord_calibrate();
      set_message("ok");
    } else if (strcmp(message, "*P") == 0) {
      chord_dump();
    }
  }
  digit_set(chord_keys_down());
  pixels_scroll(message, message_length, scroll);
  update();
}
