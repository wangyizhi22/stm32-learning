#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main()
{
    srand(time(NULL));//设置种子，种子不变结果不变 //时间永远在变
    int num = rand() % 100 + 1;
    int guess;
    
    while (1)

    {
        printf("请输入一个数字\n");
        scanf("%d",&guess);
        if(num > guess)
        {
            printf("小了");

        }
        else if(num < guess)
        {
            printf("大了");

        }
        else
        {
            printf("nb\n");
            break;

        }
    }
    
        
    
    
    return 0;
}