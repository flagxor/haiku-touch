#ifndef _pixels_h_
#define _pixels_h_

#include "Arduino.h"

extern void pixels_setup(void);
extern void pixels_refresh(const uint8_t* pix);
extern void pixels_scroll(const char* message, int len, int scroll);

#endif
