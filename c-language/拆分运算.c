#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    printf("请输入一个三位数字：\n");
    int num;
    scanf("%d", &num);
    int bai = num / 100 % 10;
    int shi = num / 10 % 10;
    int ge = num % 10;
    printf("百位数是：%d\n", bai);
    printf("十位数是：%d\n", shi);
    printf("个位数是：%d\n", ge);
    return 0;
}