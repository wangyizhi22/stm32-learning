#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* method();


int main()
{
    //野指针：未分配的内存地址
    int a = 10;
    int* p1 = &a;
    printf("%p\n",p1);
    printf("%d\n",*p1);


    int* p2 = p1 + 10;
     printf("%p\n",p2);
     printf("%d\n",*p2);


     int* p3 = method();
     printf("拖时间");
     printf("拖时间");
     printf("拖时间");
     printf("%p\n",p3);
     printf("%d\n",*p3);


    return 0;
}

int* method()
{
    int num = 10;
    int* p = &num;
    return p;
}