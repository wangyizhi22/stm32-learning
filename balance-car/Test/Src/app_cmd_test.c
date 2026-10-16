#include "app_cmd_test.h"
#include "app_cmd.h"
#include "app_usart2.h"

void App_Cmd_Test(void)
{
	App_Cmd_Init();
	App_USART2_Init();
	
	while(1)
	{
		App_Cmd_Proc();
	}
}
