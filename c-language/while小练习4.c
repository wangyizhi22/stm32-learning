#include <stdio.h>

int main()
{
    int num = 17;
    int i = 1;
    while(i * i <= num)
    {
        if(i * i == num)
        {
            printf("%d是一个完全平方数\n", num);
            break;
        }
        i++;
    }
    printf("%d不是一个完全平方数\n", num);
    printf("算数平方根的整数部分是：%d\n", i - 1);
    return 0;
}