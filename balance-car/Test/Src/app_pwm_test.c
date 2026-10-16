#include "delay.h"
#include "app_usart2.h"
#include "app_pwm.h"
#include "app_encoder.h"
#include "app_bat.h"
#include "task.h"
#include "app_calibrator.h"

void App_PWM_Test(void)
{
	App_Calibrator_Init();
	App_USART2_Init();
	App_Bat_Init();
	App_PWM_Init();
	App_PWM_Cmd(ENABLE);
	App_Encoder_Init();
	
//	App_PWM_Set_L(30);
//	App_PWM_Set_R(30);
//	Delay(1000);
//	
//	App_PWM_Set_L(60);
//	App_PWM_Set_R(60);
//	Delay(1000);
//	
//	App_PWM_Set_L(90);
//	App_PWM_Set_R(90);
//	Delay(1000);
	
	// 正转3s
	uint32_t expire = GetTick() + 3000;
	
	while(GetTick() < expire)
	{
		App_Bat_Proc();
		
		PERIODIC_START(PWM_TEST, 10)
		
		float bat = App_Bat_Get();
		
		App_PWM_Set_L(3 / bat * 100.0f);
		App_PWM_Set_R(3 / bat * 100.0f);
		
		My_USART_Printf(&huart2, "%.3f,%.3f\n", App_Encoder_GetSpeed_L(), App_Encoder_GetSpeed_R());
		PERIODIC_END
	}
	
	// 反转3s
	expire = GetTick() + 3000;
	
	while(GetTick() < expire)
	{
		App_Bat_Proc();
		
		PERIODIC_START(PWM_TEST, 10)
		
		float bat = App_Bat_Get();
		
		App_PWM_Set_L(-3 / bat * 100.0f);
		App_PWM_Set_R(-3 / bat * 100.0f);
		
		My_USART_Printf(&huart2, "%.3f,%.3f\n", App_Encoder_GetSpeed_L(), App_Encoder_GetSpeed_R());
		PERIODIC_END
	}
	
	App_PWM_Cmd(DISABLE);
	
	while(1){}
}

//
// @简介：测量电机的响应曲线。
//  设电机的响应函数为 Phi(s) = K/(Ts+1)
//  r(t) = 3(t)
//  c(∞) = 290 r/min
//  ts = 0.457s
//  由T=3ts，得 T = 0.5/3 = 0.152s
//  K = c(∞) / r = 300 / 80 = 3.75
//  K = 0.012954
//  因此，
//  Phi(s) = 3.75/(0.167s+1)

void App_Motor_Test_MotorResponseFunction(void)
{
	App_PWM_Init();
	App_Encoder_Init();
	App_PWM_Cmd(ENABLE);
	App_USART2_Init();
	
	// #1. 电机静止，保持2s
	Delay(2000);
	
	// #2. 给电机加3V电压，采集其速度曲线
	App_PWM_Set_L(80.0);
//	App_Motor_SetU_R(3.0);
	
	uint64_t start = GetTick();
	
	while(GetTick() < start + 3000) // 测量3s的值
	{
		uint32_t t = GetTick() - start;
		
		float speed_l = App_Encoder_GetSpeed_L();
//		float speed_r = App_Motor_Get_n_R();
		
		My_USART_Printf(&huart2, "%.3f,%.3f\n", t*1.0e-3f, speed_l);
		
		Delay(2);
	}
	
	// #3. 停止电机
	App_PWM_Set_L(0);
//	App_Motor_SetU_R(0);
	
	while(1){}
}

// 
// 测量电机的K值曲线（电机具有非线性特性，K值会随输入电压变化）
// 
void App_Motor_Test_K(void)
{
	App_PWM_Init();
	App_Encoder_Init();
	App_PWM_Cmd(ENABLE);
	App_USART2_Init();
	

	// #1. 电机静止，保持2s
	App_PWM_Set_L(-100);
	
	Delay(2000);
	
	// #2. 给电机加电压并保持0.5s
	for(int16_t i=-100;i<=100;i++)
	{
		App_PWM_Set_L(i);
		
		uint32_t start = GetTick();
		
		Delay(500);
		
		float speed_l = App_Encoder_GetSpeed_L();
		
		My_USART_Printf(&huart2, "%d,%.3f\n", i,speed_l);
	}
	
	App_PWM_Set_L(0);
	
	while(1){}
}
