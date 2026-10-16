#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    for (int i = 1; i <= 100; i++)
    
    {
        if(i % 3 == 0 && i % 5 == 0)
        {
            printf("%d\n", i);
            break;
        }
        
    }









    //continue语句：跳过本次循环，继续下一次循环
    for (int i = 1; i <= 5; i++)
    {
        if (i == 3)
        {
            continue;//跳过第三个循环
        }
        printf("在吃第%d个苹果\n", i);
    }
    return 0;
}