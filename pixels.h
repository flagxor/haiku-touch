#ifndef _pixels_h_
#define _pixels_h_

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void pixels_setup(void);
extern void pixels_refresh(const uint8_t* pix);
extern void pixels_scroll(const char* message, int len, int scroll);

#ifdef __cplusplus
};
#endif

#endif
