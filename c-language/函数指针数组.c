#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int add(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
int divv(int a, int b);

int main()
{
    int (*arr[4])(int, int) = { add, sub, mul, divv };//创建函数指针数组
    //特性：只有返回值类型一致才能放进同一个指针数组中
    int a, b;
    int choice;
    printf("请输入两个数字：");
    scanf("%d %d", &a, &b);
    printf("请选择运算方式1加 2减 3乘 4除:\n");
    scanf("%d", &choice);
    
    // 2. 边界检查
    if (choice < 1 || choice > 4) {
        printf("选择无效！\n");
        return 1;
    }
    
    // 3. 通过函数指针数组调用函数
    int result = arr[choice - 1](a, b);
    printf("结果：%d\n", result);
    return 0;
}

int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

int mul(int a, int b)
{
    return a * b;
}

int divv(int a, int b)
{
    return a / b;
}