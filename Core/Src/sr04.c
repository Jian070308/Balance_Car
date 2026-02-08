#include "sr04.h"

extern TIM_HandleTypeDef htim3;
uint16_t count;
float distance;

void Delay_us(uint32_t nus)
{
    uint32_t Delay = nus * 168/4;
    do
    {
        __NOP();
    }
    while (Delay --);
}

void Get_Distance(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	Delay_us(12);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_2){
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_SET){
			__HAL_TIM_SET_COUNTER(&htim3,0);
			HAL_TIM_Base_Start(&htim3);
		}
		else{
			HAL_TIM_Base_Stop(&htim3);
			count=__HAL_TIM_GET_COUNTER(&htim3);
			distance=count*0.017;
		}
	}
	else if(GPIO_Pin == GPIO_PIN_5){
		Control();
	}

}


