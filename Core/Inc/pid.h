#ifndef INC_PID_H_
#define INC_PID_H_

#include "stm32f1xx_hal.h"
#include "math.h"
#include "encoder.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu6050.h"
#include "motor.h"

void Control();

#endif /* INC_PID_H_ */
