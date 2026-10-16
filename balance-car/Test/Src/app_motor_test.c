#include "app_motor_test.h"
#include "app_motor.h"
#include "app_usart2.h"
#include "task.h"
#include "qmath.h"
#include "app_bat.h"
#include "app_pwm.h"
#include "app_encoder.h"
#include "stdlib.h"
#include "app_calibrator.h"

static void USART2_Proc(void);
static void Motor_Speed_Tune_Proc(void);

void MotorSpeedTest(void)
{
	App_Calibrator_Init();
	App_Bat_Init();
	App_Motor_Init();
	App_Motor_Cmd(ENABLE);
	App_USART2_Init();
	
	while(1)
	{
		App_Motor_Proc();
		USART2_Proc();
		Motor_Speed_Tune_Proc();
		App_Bat_Proc();
	}
}

static uint8_t stage = 0; // 0 静止，1 正转
static uint32_t lastTimeChange = 0;

static void Motor_Speed_Tune_Proc(void)
{
	PERIODIC(1);
		
	switch(stage)
	{
		case 0:
			if(GetTick() - lastTimeChange > 2000)
			{
				App_Motor_SetSpeed_L(10);
				App_Motor_SetSpeed_R(20);
				stage = 1;
				lastTimeChange = GetTick();
			}
			break;
		case 1:
			if(GetTick() - lastTimeChange > 2000)
			{
				App_Motor_SetSpeed_L(0);
				App_Motor_SetSpeed_R(0);
				lastTimeChange = GetTick();
			}
			break;
	}
	
	float speed_ref = 5*qsin(GetTick()*1.0e-3f*6.28*0.2);
	App_Motor_SetSpeed_L(speed_ref);
	App_Motor_SetSpeed_R(speed_ref);

//	App_Motor_SetSpeed_L(20);
//	App_Motor_SetSpeed_R(20);
}

static void USART2_Proc(void)
{
	PERIODIC(1);
	
	float speed_l = App_Motor_GetSpeed_L();
	float speed_r = App_Motor_GetSpeed_R();
	
	My_USART_Printf(&huart2, "%.3f,%.3f\n", speed_l, speed_r);
}
