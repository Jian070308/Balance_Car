#include "standard_PID.h"

PID_ANGLE  	    g_angle_pid;                /* 直立环PID参数结构体 */
PID_SPEED  		g_speed_pid;                /* 速度环PID参数结构体 */
Motor_DATA      g_motor_data;			 	/*   电机参数结构体   */
uint8_t stop_flag=0;
float pitch, roll, yaw;				//角度值
short gyro_x,gyro_y,gyro_z;         //角速度


void pid_init()
{
    /* 初始化位置环PID参数 */
	g_angle_pid.SetPoint = 0.0;       /* 目标值 */
	g_angle_pid.ActualValue = 0.0;    /* 期望输出值值 */
	g_angle_pid.RealPoint = 0.0;      /* 目标值 */
	g_angle_pid.Gyro_X = 0.0;         /* 目标值 */
	g_angle_pid.Proportion = A_KP;    /* 比例常数 Proportional */
	g_angle_pid.Integral = A_KI;      /* 积分常数 Proportional */
	g_angle_pid.Derivative = A_KD;    /* 微分常数 Derivative   */

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

    g_motor_data.MedAngle=MED_ANGLE;
    g_motor_data.compensation=COMPENSATION;
}

//直立环PD控制器
//输入：期望角度 真实角度 角速度
int Vertical(){
	g_angle_pid.RealPoint=roll;
	g_angle_pid.Gyro_X=gyro_x;
	g_angle_pid.ActualValue = g_angle_pid.Proportion*(g_angle_pid.RealPoint - g_angle_pid.SetPoint)
									+ g_angle_pid.Derivative * g_angle_pid.Gyro_X;
	return g_angle_pid.ActualValue;
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
	float temp =g_speed_pid.Proportion*g_speed_pid.ErrorLowout+g_speed_pid.Integral*g_speed_pid.SumError;

	g_speed_pid.ActualValue=((temp>20)?20:(temp<-20?-20:temp));   //限制角度在-30到30

	return g_speed_pid.ActualValue;
}

void Control(){
	//读取电机和陀螺仪数据
	Read_Speed();

	mpu_dmp_get_data(&pitch, &roll, &yaw);
	MPU_Get_Gyroscope(&gyro_x, &gyro_y, &gyro_z);

	//外环周期为内环周期的五倍
	if(s_pid_flag==1){
		//外环速度环-->输出值传给内环
		g_angle_pid.SetPoint=Speed()+g_motor_data.MedAngle;
		s_pid_flag=0;
	}

	//接收外环目标值，并直接输出占空比
	g_motor_data.pwm_left=Vertical();

	//暂未加入转向环

	//死区补偿
	if(g_motor_data.pwm_left>0){
		g_motor_data.pwm_left+=g_motor_data.compensation;
	}else if(g_motor_data.pwm_left<0){
		g_motor_data.pwm_left-=g_motor_data.compensation;
	}

	g_motor_data.pwm_right=g_motor_data.pwm_left;

	Limit(&g_motor_data.pwm_left,&g_motor_data.pwm_right);
	Motor_Start(g_motor_data.pwm_left,g_motor_data.pwm_right);

#if DEBUG_ENABLE  /* 发送基本参数*/

	debug_send_wave_data( 1 ,roll);            					 /* 选择通道1，发送实际角度（波形显示）*/
	debug_send_wave_data( 2 ,g_motor_data.MedAngle);             /* 选择通道2，发送机械中值（波形显示）*/
    debug_send_wave_data( 3 ,g_motor_data.speed_car);            /* 选择通道3，发送实际速度（波形显示）*/
    debug_send_wave_data( 4 ,g_speed_pid.SetPoint);              /* 选择通道4，发送目标速度（波形显示）*/

#endif
}
