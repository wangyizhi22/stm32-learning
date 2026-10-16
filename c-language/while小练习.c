#include <stdio.h>

int main()
{
    int n;
    printf("请输入一个数字");
    scanf("%d", &n);
    while(n > 1 && n % 2 == 0)
    {
        n = n / 2;
    }

    if(n == 1)
    {
        printf("是2的幂次方\n");
    }
    else
    {
        printf("不是2的幂次方\n");
    }

    return 0;
}