#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32f1xx_hal.h"
#include <stdlib.h>
#include "main.h"

void Motor_Start(int8_t moto1,int8_t moto2);
void Limit(int8_t *motor1,int8_t *motor2);

#endif /* INC_MOTOR_H_ */
