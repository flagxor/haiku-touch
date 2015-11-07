#ifndef _captouch_h_
#define _captouch_h_

#include "Arduino.h"

#define CAPTOUCH_BUTTONS 23

extern void captouch_setup(void);
extern void captouch_read(uint8_t *out);

#endif
