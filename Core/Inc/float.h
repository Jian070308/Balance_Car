#ifndef INC_FLOAT_H_
#define INC_FLOAT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "stm32f1xx_hal.h"

void py_f2s4printf(char * stra, float x, uint8_t flen);
void my_sprintf(char *result, const char *prefix, float num, uint8_t flen);

#endif /* INC_FLOAT_H_ */
