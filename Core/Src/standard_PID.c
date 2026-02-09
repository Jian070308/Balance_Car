#include "standard_PID.h"

PID_LOCATION  	g_location_pid;             /* 位置环PID参数结构体 */
PID_SPEED  		g_speed_pid;                /* 速度环PID参数结构体 */
Motor_DATA      g_motor_data;			 	/*   电机参数结构体   */
uint8_t stop_flag=0;
float pitch, roll, yaw;				//角度值
short gyro_x,gyro_y,gyro_z;         //角速度

void pid_init()
{
    /* 初始化位置环PID参数 */
    g_location_pid.SetPoint = 0.0;       /* 目标值 */
    g_location_pid.ActualValue = 0.0;    /* 期望输出值值 */
    g_location_pid.RealPoint = 0.0;      /* 目标值 */
    g_location_pid.Gyro_X = 0.0;         /* 目标值 */
    g_location_pid.Proportion = L_KP;    /* 比例常数 Proportional */
    g_location_pid.Integral = L_KI;      /* 积分常数 Proportional */
    g_location_pid.Derivative = L_KD;    /* 微分常数 Derivative   */

    /* 初始化速度环PID参数 */
    g_speed_pid.SetPoint = 0.0;          /* 目标值 */
    g_speed_pid.ActualValue = 0.0;       /* 期望输出值 */
    g_speed_pid.SumError = 0.0;          /* 积分值 */
    g_speed_pid.Error = 0.0;             /* Error[1]  */
    g_speed_pid.ErrorLowout = 0.0;       /* ErrorLowout[1]  */
    g_speed_pid.LastErrorLowout = 0.0;   /* ErrorLowout[-1] */
    g_speed_pid.Proportion = S_KP;       /* 比例常数 Proportional  */
    g_speed_pid.Integral = S_KI;         /* 积分常数 Integral      */
    g_speed_pid.Derivative = S_KD;       /* 微分常数 Integral      */
}

//直立环PD控制器
//输入：期望角度 真实角度 角速度
int Vertical(){
	g_location_pid.ActualValue = g_location_pid.Proportion*(g_location_pid.RealPoint - g_location_pid.SetPoint)
									+ g_location_pid.Derivative * g_location_pid.Gyro_X;
	return g_location_pid.ActualValue;
}

//速度环PI控制器
//输入：期望速度 真实速度（左/右）
float Speed(){
	static float a=0.7;     //滤波系数

	//1.计算偏差值
	g_speed_pid.Error = (g_motor_data.speed_left+g_motor_data.speed_right)-g_speed_pid.SetPoint;
	//2.低通滤波
	g_speed_pid.ErrorLowout=(1-a)*g_speed_pid.Error+a*g_speed_pid.LastErrorLowout;
	g_speed_pid.LastErrorLowout=g_speed_pid.ErrorLowout;
	//3.积分
	g_speed_pid.SumError+=g_speed_pid.ErrorLowout;
	//4.积分限幅
	if(g_speed_pid.SumError>20000) g_speed_pid.SumError = 20000;
	if(g_speed_pid.SumError<-20000) g_speed_pid.SumError = -20000;
	//5.清除积分，实现急停
	if(stop_flag==1) g_speed_pid.SumError = 0;
	//6.速度环
	g_speed_pid.ActualValue =g_speed_pid.Proportion*g_speed_pid.ErrorLowout+g_speed_pid.Integral*g_speed_pid.SumError;

	return g_speed_pid.ActualValue;
}

void Control(){
	//读取电机和陀螺仪数据
	Read_Speed();
	mpu_dmp_get_data(&pitch, &roll, &yaw);
	MPU_Get_Gyroscope(&gyro_x, &gyro_y, &gyro_z);

	//外环速度环-->输出值传给内环
	g_location_pid.SetPoint=Speed();

	//接收外环目标值，并直接输出占空比
	g_motor_data.pwm_left=Vertical();
	g_motor_data.pwm_right=g_motor_data.pwm_left;
	//暂未加入转向环

	Limit(&g_motor_data.pwm_left,&g_motor_data.pwm_right);
	Motor_Start(g_motor_data.pwm_left,g_motor_data.pwm_right);
}
