#ifndef INC_STANDARD_PID_H_
#define INC_STANDARD_PID_H_

#include "main.h"

/* 注意：双环控制的时候，外环PID参数调节幅度不要太大，这对于整个曲线的影响很大 */

/* 定义位置环（内环）PID参数相关宏 */
#define  A_KP      84               /* P参数 */        //300-600
#define  A_KI      0                /* I参数 */
#define  A_KD      0.6              /* D参数 */		   //1-5

/* 定义速度环（外环）PID参数相关宏 */
#define  S_KP      -0.015f             /* P参数 */		   //0.01-0.08
#define  S_KI      -0.00008f             /* I参数 */
#define  S_KD      0.00f             /* D参数 */

/*     定义机械中值    */
#define  MED_ANGLE      0.5f

/*     电机死区补偿值    */
#define  COMPENSATION   100


/* PID结构体 */
typedef struct
{
    __IO float  SetPoint;            /* 目标值 */
    __IO float  ActualValue;         /* 期望输出值 */
    __IO float  SumError;            /* 误差累计 */
    __IO float  Proportion;          /* 比例常数 P */
    __IO float  Integral;            /* 积分常数 I */
    __IO float  Derivative;          /* 微分常数 D */
    __IO float  Error;               /* Error[1] */
    __IO float  ErrorLowout;         /* ErrorLowout[1] */
    __IO float  LastErrorLowout;     /* ErrorLowout[-1] */
} PID_SPEED;

extern PID_SPEED  g_speed_pid;     /* 速度环PID参数结构体 */

typedef struct
{
	__IO float  SetPoint;			 /* 目标角度值 */
	__IO float  RealPoint;			 /* 真实角度值 */
	__IO float  Gyro_X;			 	 /* 真实角速度 */
    __IO float  Proportion;          /* 比例常数 P */
    __IO float  Integral;            /* 积分常数 I */
    __IO float  Derivative;          /* 微分常数 D */
    __IO int    ActualValue;         /* 期望输出值 */

}PID_ANGLE;

extern PID_ANGLE  g_angle_pid;  /* 位置环PID参数结构体 */

/* 电机参数结构体 */
typedef struct
{
  float speed_left;       /*左电机实际速度*/
  float speed_right; 	  /*右电机实际速度*/
  float speed_car;        /*小车速度*/
  float MedAngle;         /*机械中值*/
  uint8_t state;          /*电机状态*/
  int compensation;       /*死区补偿*/
  int  pwm_left;          /*左电机比较值 */
  int  pwm_right;         /*右电机比较值 */

} Motor_DATA;

extern Motor_DATA  g_motor_data;  /*电机参数变量*/

extern uint8_t stop_flag;
extern float pitch, roll, yaw;				//角度值
extern short gyro_x,gyro_y,gyro_z;          //角速度

void pid_init();
int Vertical();
float Speed();
void Control();

#endif /* INC_STANDARD_PID_H_ */
