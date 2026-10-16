/*************************************************************

简介：使用串口将左右电机的速度发送到电脑，并实时显示两条

测试方法：

使用USB转TTL模块连接单片机的串口2
USART2_Tx (PA2) - USB_TTL_Rx
STM32_GND       - USB_TTL_GND

电脑端打开VOFA+，选择波形图，并显示I0和I1两个通道
I0 - 左编码器速度
I1 - 右编码器速度

*************************************************************/

#include "app_encoder.h"
#include "task.h"
#include "app_usart2.h"
#include "app_pwm.h"
#include "app_calibrator.h"

void App_Encoder_Test(void)
{
	App_Calibrator_Init();
	App_USART2_Init();
	App_Encoder_Init();
	
	while(1)
	{
		float vl = App_Encoder_GetSpeed_L();
		float vr = App_Encoder_GetSpeed_R();
		int32_t encoder_l = App_Encoder_GetPos_L();
		int32_t encoder_r = App_Encoder_GetPos_R();
		
		My_USART_Printf(&huart2, "%.3f,%.3f,%d,%d\n", vl, vr,encoder_l, encoder_r);
		
		Delay(2);
	}
}
