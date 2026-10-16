#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    char str[100];
    printf("请输入你女朋友的名字：\n");
    scanf("%s", str);//str不需要加&符号，因为str是一个数组名，它本身就是一个指向数组首元素的指针
    printf("你女朋友的名字是：%s\n", str);
    return 0;
}