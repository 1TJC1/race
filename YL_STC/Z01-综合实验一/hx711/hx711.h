#ifndef __HX711_H__
#define __HX711_H__

#include "main.h"

#define hx711_DAT	P26
#define hx711_SCLK_LOW  P25=0
#define hx711_SCLK_HIGH P25=1

unsigned long ReadCount(void);

#endif





