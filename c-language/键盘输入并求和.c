#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    // int num1;
    // int num2;
    // printf("请录入第一个数字：\n");
    // scanf("%d", &num1);
    // printf("请录入第二个数字：\n");
    // scanf("%d", &num2);
    // int total;
    // total = num1 + num2;
    // printf("两个数字的和是：%d\n", total);
    int num1;
    int num2;
    printf("请录入两个数字：\n");
    scanf("%d %d", &num1, &num2);
    int total;
    printf("两个数字的和是：%d\n", num1 + num2);
    return 0;
}