#include "app_mpu6050_test.h"
#include "app_mpu6050.h"
#include "main.h"
#include "app_usart2.h"
#include "task.h"
#include "app_calibrator.h"

void App_MPU6050_Test(void)
{
	App_Calibrator_Init();
	App_USART2_Init(); // 串口2初始化
	App_MPU6050_Init(); // MPU6050初始化
	
	while(1)
	{
		App_MPU6050_Proc();
		
		PERIODIC_START(PRINT, 1)
		
		float ax = App_MPU6050_GetAccelX();
		float ay = App_MPU6050_GetAccelY();
		float az = App_MPU6050_GetAccelZ();
		
		float gx = App_MPU6050_GetGyroX();
		float gy = App_MPU6050_GetGyroY();
		float gz = App_MPU6050_GetGyroZ();
		
		float temp = App_MPU6050_GetTemperature();
		
		float yaw = App_MPU6050_GetYaw();
		float roll = App_MPU6050_GetRoll();
		float pitch = App_MPU6050_GetPitch();
		
		My_USART_Printf(&huart2, 
		                "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n",
		                 ax,  ay,  az,  temp,gx,  gy,  gz,  yaw, pitch, roll);
		PERIODIC_END;
	}
}
