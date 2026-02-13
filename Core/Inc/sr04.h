#ifndef INC_SR04_H_
#define INC_SR04_H_

#include "main.h"

extern float distance;
extern __IO uint8_t pid_flag;

void Get_Distance();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* INC_SR04_H_ */
