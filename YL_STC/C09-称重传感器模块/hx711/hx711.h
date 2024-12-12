#ifndef __HX711_H__
#define __HX711_H__

#include "main.h"

#define hx711_DAT	P21
#define hx711_SCLK_LOW  P20=0
#define hx711_SCLK_HIGH P20=1

unsigned long ReadCount(void);

#endif





