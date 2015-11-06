#include "steno.h"
#include "captouch.h"

static char button_name[] = "#STKPWHRAO*EUFRPBLGTSDZ";
#if 0
static char button_name[] = {
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
#endif

void steno_to_string(long ch, char *out) {
  int i;
  int pos;

  pos = 0;
  for (i = 0; i < CAPTOUCH_BUTTONS; i++) {
    if (ch & 1) {
      out[pos++] = button_name[i];
    }
  }
  out[pos] = '\0';
}
