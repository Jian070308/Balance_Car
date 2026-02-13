#include "encoder.h"

uint32_t sys_tick=0;

int Read_Encoder(TIM_HandleTypeDef *htim){
	int coder = 0;
	coder = (short)__HAL_TIM_GET_COUNTER(htim);       //转换类型后才能实现显示负数
	__HAL_TIM_SET_COUNTER(htim,0);
	return coder;
}

void Read_Speed() {
#ifdef GET_SPEED_FOR_PID

        g_motor_data.speed_left = ((float)Read_Encoder(&htim2) * f * 60) / (my_times * PPR * Ratio);
        g_motor_data.speed_right = -((float)Read_Encoder(&htim4) * f * 60) / (my_times * PPR * Ratio);
        g_motor_data.speed_car = (g_motor_data.speed_left+g_motor_data.speed_right)/2;

#else
    uint32_t current_tick = uwTick;
    uint32_t delta_time = current_tick - sys_tick; // 计算实际经过了多少毫秒

    // 只有经过时间大于等于10ms才计算（防止除以0）
    if(delta_time >= 10){
        sys_tick = current_tick; // 更新时间

        // 算出实际频率 real_f = 1000 / 经过的毫秒数
        // 比如经过了50ms，频率就是 20Hz
        float real_f = 1000.0f / (float)delta_time;

        // 把公式里的 f 换成 real_f
        g_motor_data.speed_left = ((float)Read_Encoder(&htim2) * real_f * 60) / (my_times * PPR * Ratio);
        g_motor_data.speed_right = -((float)Read_Encoder(&htim4) * real_f * 60) / (my_times * PPR * Ratio);
    }
#endif
}















