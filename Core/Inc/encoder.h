#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#define my_times 4
#define PPR 11
#define Ratio 30

extern float speed_left,speed_right;


#include "stm32f1xx_hal.h"
#include "main.h"

int Read_Encoder(TIM_HandleTypeDef *htim);
void Read_Speed();

#endif /* INC_ENCODER_H_ */
