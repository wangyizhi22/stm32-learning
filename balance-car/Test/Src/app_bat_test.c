#include "app_bat.h"
#include "task.h"
#include "app_usart2.h"

void App_Bat_Test(void)
{
	App_USART2_Init();
	App_Bat_Init();
	
	while(1)
	{
		float volt = App_Bat_Get();
		
		My_USART_Printf(&huart2, "%.3f\n", volt);
		
		Delay(5);
	}
}
