/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t display_buf[20];
uint8_t rx_buf[10];
uint8_t res=0;
uint8_t a=1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart3){

		debug_handle(&res);

		HAL_UART_Receive_IT(huart, &res, 1);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  Project_Init();

#if DEBUG_ENABLE                            /* 开启调试 */

  debug_send_motorcode(DC_MOTOR);         /* 上传电机类型（直流有刷电机） */
  debug_send_motorstate(RUN_STATE);       /* 上传电机状态（空闲） */

  /* 同步数据PID参数到上位机 ，无论同步哪一组数据，目标值地址只能是外环PID的 */
  debug_send_initdata(TYPE_PID1, (float *)(&g_speed_pid.SetPoint), A_KP, A_KI, A_KD);  /* 位置环PID参数（PID1）*/
  debug_send_initdata(TYPE_PID2, (float *)(&g_speed_pid.SetPoint), S_KP, S_KI, S_KD);  /* 速度环PID参数（PID2）*/

#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(a_pid_flag==1){
		  Control();
		  a_pid_flag=0;
	  }

#if DEBUG_ENABLE

        /* 接收PID助手设置的直立环PID参数 */
        debug_receive_pid(TYPE_PID1, (float *)&g_angle_pid.Proportion,(float *)&g_angle_pid.Integral, (float *)&g_angle_pid.Derivative);

        /* 接收PID助手设置的速度环PID参数 */
        debug_receive_pid(TYPE_PID2, (float *)&g_speed_pid.Proportion, (float *)&g_speed_pid.Integral, (float *)&g_speed_pid.Derivative);

#endif


      /*读取数据*/
      mpu_dmp_get_data(&pitch, &roll, &yaw);

      Get_Distance();

      //显示数据
      OLED_PRINT();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Project_Init(){
      HAL_Delay(20);
      OLED_Init();

      MPU_Init();
      mpu_dmp_init();

      pid_init();
      debug_init();

      //确保初始化完成后再开启中断
      HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
      HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
      __HAL_TIM_SET_COUNTER(&htim2,0);
      HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
      __HAL_TIM_SET_COUNTER(&htim4,0);

      HAL_UART_Receive_IT(&huart3, &res, 1);



}

void OLED_PRINT(){
      OLED_NewFrame();

      PRINT(roll,1);
      PRINT(g_motor_data.speed_left,17);
      PRINT(g_motor_data.speed_left,33);
      PRINT(g_speed_pid.ActualValue,48);

      OLED_ShowFrame();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
