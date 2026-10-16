#include <stdio.h>

int main()
{
    int num;
    int rev = 0;
    int original;
    printf("请输入一个数字");
    scanf("%d", &num);
    original = num;
    while(num > 0)
    {
        rev = rev * 10 + num % 10;
        num = num / 10;
    }
    if(rev == original)
    {
        printf("是回文数\n");
    }
    else
    {
        printf("不是回文数\n");
    }
    printf("rev = %d\n", rev);
    return 0;
}