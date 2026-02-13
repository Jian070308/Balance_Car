#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#define f 100
#define my_times 4
#define PPR 11
#define Ratio 30

#define GET_SPEED_FOR_PID 1

#include "main.h"

int Read_Encoder(TIM_HandleTypeDef *htim);
void Read_Speed();

#endif /* INC_ENCODER_H_ */
