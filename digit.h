#ifndef _digit_h_
#define _digit_h_

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void digit_setup(void);
extern void digit_set(int value);
extern void digit_set_raw(uint8_t value);

#ifdef __cplusplus
};
#endif

#endif
