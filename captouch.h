#ifndef _captouch_h_
#define _captouch_h_

#include "Arduino.h"

#define CAPTOUCH_BUTTONS 23

#ifdef __cplusplus
extern "C" {
#endif

extern void captouch_setup(void);
extern void captouch_read(uint8_t *out);

#ifdef __cplusplus
};
#endif

#endif
