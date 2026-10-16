#include <stdio.h>

int main()
{
    int num;
    int sum = 0;
    printf("请输入一个1000以内的数字：");
    scanf("%d", &num);
    while (num != 0)
    {
        int temp = num % 10;
        sum += temp;
        num /= 10;

    }
    if (sum == 15)
    {
        printf("这个数字的各位数字之和为15\n");
    }
    else
    {
        printf("这个数字的各位数字之和不为15\n");
    }
    return 0;
}
        