#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32f1xx_hal.h"
#include <stdlib.h>
#include "main.h"

void Motor_Start(int motor1,int motor2);
void Limit(int *motor1,int *motor2);

#endif /* INC_MOTOR_H_ */
