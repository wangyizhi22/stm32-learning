#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    for(int i = 2; i <= 100; i++)    // 从 2 开始，1 不是质数
    {
        int count = 0;
        for (int j = 2; j * j <= i; j++)  
        {
            if(i % j == 0)
            {
                count++;
                break;
            }
        }

        if(count == 0)
        {
            printf("%d 是一个质数\n", i);
        }
    }
}