//#include "pid.h"
//
//#define KP_Vertical 0
//#define KD_Vertical 0
//
//#define KP_Speed 0
//#define KI_Speed 0
//
//#define KP_Turn 0
//#define KD_Turn 0
//
////数据
//int encoder_left,encoder_right;
//
//short a_x,a_y,a_z;                  //加速度
//int Vertical_out,Speed_out,Turn_out;
//int target_angle,target_speed,target_turn;
//float Med_angle;                    //平衡时角度的偏移量
//int motor1,motor2;
//
//
//extern TIM_HandleTypeDef htim2;
//extern TIM_HandleTypeDef htim4;
//
//
//int stop1=0;
//
////直立环PD控制器
////输入：期望角度 真实角度 角速度
//int Vertical(float target,float angle,float gyro_x){
//	int output = 0;
//	output = KP_Vertical*(angle - target) + KD_Vertical * gyro_x;
//	return output;
//}
//
////速度环PI控制器
////输入：期望速度 真实速度（左/右）
//int Speed(int target,int encoder_left,int encoder_right){
//	static int error_lowout_last;
//	static int encoder_s=0;
//	static float a=0.7;
//	int error,error_lowout,temp;
//
//	//1.计算偏差值
//	error = (encoder_left+encoder_right)-target;
//	//2.低通滤波
//	error_lowout=(1-a)*error+a*error_lowout_last;
//	error_lowout_last=error_lowout;
//	//3.积分
//	encoder_s+=error_lowout;
//	//4.积分限幅
//	if(encoder_s>20000) encoder_s = 20000;
//	if(encoder_s<-20000) encoder_s = -20000;
//	//5.清除积分，实现急停
//	if(stop1==1) encoder_s = 0;
//	//6.速度环
//	temp = KP_Speed*error_lowout+KI_Speed*encoder_s;
//
//	return temp;
//}
//
//
////转向环PD控制器
////输入：期望角度 角速度
//int Turn(float target,float gyro_z){
//	int temp;
//	temp = KP_Turn*target+KD_Turn*gyro_z;
//	return temp;
//}
//
//
//void Control(){
//	int PWM_out;
//
//	//读取编码器和陀螺仪数据
//	encoder_left=Read_Encoder(&htim2);
//	encoder_right=-Read_Encoder(&htim4);
//	mpu_dmp_get_data(&pitch, &roll, &yaw);
//	MPU_Get_Gyroscope(&gyro_x, &gyro_y, &gyro_z);
//	MPU_Get_Accelerometer(&a_x, &a_y, &a_z);
//
//	//将数据传入传感器
//	Speed_out=Speed(target_speed,encoder_left,encoder_right);
//	Vertical_out=Vertical(Speed_out+Med_angle,roll,gyro_x);     //具体哪个角度控制平衡需测试
//	Turn_out=Turn(target_turn,gyro_z);
//	PWM_out=Vertical_out;
//	motor1=PWM_out-Turn_out;         //差速转向 转速为差值 即2*Turn_out
//	motor2=PWM_out+Turn_out;
//	Limit(&motor1,&motor2);
//	Motor_Start(motor1,motor2);
//}
//
//
//
