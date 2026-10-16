#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main()
{
    srand(time(NULL));//设置种子，种子不变结果不变 //时间永远在变
    for(int i = 0; i <= 50; i++)
    {
        int num = rand() % 100 + 1;//获取随机数//取任意范围的随机数
        //用于生成任意范围的随机数
        //1.把这个范围变成包头不包尾，包左不包右的8- 50
        //2.拿着尾巴减开头 50 - 8 = 42
        //3.修改代码
        
        printf("随机数是：%d\n", num);
    }
    
    return 0;
}