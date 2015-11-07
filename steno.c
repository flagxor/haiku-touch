#include "steno.h"
#include "captouch.h"

static char button_name[] = "#STKPWHRAO*EUFRPBLGTSDZ";

void steno_to_string(long ch, char *out) {
  int i;
  int pos;

  pos = 0;
  for (i = 0; i < CAPTOUCH_BUTTONS; i++) {
    if (ch & 1) {
      out[pos++] = button_name[i];
    }
    ch >>= 1;
  }
  out[pos] = '\0';
}
