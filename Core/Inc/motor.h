#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"

//预留一点余量
#define PWM_MAX 7100
#define PWM_MIN -7100

void Motor_Start(int moto1,int moto2);
void Limit(int *motor1,int *motor2);

#endif /* INC_MOTOR_H_ */
