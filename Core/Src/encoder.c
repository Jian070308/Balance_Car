#include "encoder.h"

uint32_t sys_tick=0;
extern uint32_t uwtick;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
float speed_left,speed_right;

int Read_Encoder(TIM_HandleTypeDef *htim){
	int coder = 0;
	coder = (short)__HAL_TIM_GET_COUNTER(htim);       //转换类型后才能实现显示负数
	__HAL_TIM_SET_COUNTER(htim,0);
	return coder;
}

void Read_Speed(){
	if(uwtick-sys_tick<10){
		return;
	}
	sys_tick=uwtick;
	speed_left = (Read_Encoder(&htim2)*f*60)/(my_times*PPR*Ratio);
	speed_right = -(Read_Encoder(&htim4)*f*60)/(my_times*PPR*Ratio);
}


















