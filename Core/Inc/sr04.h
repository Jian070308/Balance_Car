#ifndef INC_SR04_H_
#define INC_SR04_H_

#include "stm32f1xx_hal.h"
#include "pid.h"

void Get_Distance();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* INC_SR04_H_ */
