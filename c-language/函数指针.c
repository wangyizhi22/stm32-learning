#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void method1();
int method2(int num1, int num2);


int main()
{
    void (*p1)() = method1;
    int (*p2)(int , int) = method2;
    p1();
    int num = p2(1, 2);
    printf("%d\n", num);
    return 0;
}

void method1()
{
    printf("method1\n");
}

int method2(int num1, int num2)
{
    return num1 + num2;
}