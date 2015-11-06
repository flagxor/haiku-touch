#ifndef _chord_h_
#define _chord_h_

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void chord_calibrate(void);
extern uint8_t chord_keys_down(void);
extern long chord_read(void);

#ifdef __cplusplus
};
#endif

#endif
