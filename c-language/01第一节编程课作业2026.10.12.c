#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()
{
    int a;
    int b;
    int c;
    printf("请输入三个数字：\n");
    scanf("%d%d%d", &a, &b, &c);
    if (a > b)
    {
        if (a > c)
        {
            printf("最大的数字是%d\n", a);
        }
        else
        {
            printf("最大的数字是%d\n", c);
        }
    }
    else
    {
        if (b > c)
        {
            printf("最大的数字是%d\n", b);
        }
        else
        {
            printf("最大的数字是%d\n", c);
        }
    }
     return 0;
}




