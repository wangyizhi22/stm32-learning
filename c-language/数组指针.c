#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int main()
{
    int arr[] = {10, 20, 30, 40, 50};
    int len = sizeof(arr) / sizeof(int);
    int *p = arr;//数组指针，即是数组的首地址
    printf("%d", *p);
    printf("%d", *(p + 1));//访问数组的第二个元素
    printf("%d\n", *(p + 2));//访问数组的第三个元素

    //利用指针遍历数组
    for (int i = 0; i < len; i++)
    {
        printf("%d\n", *(p + i));
    }







    //数组指针的细节
    printf("%p\n",arr);//数组指针arr参与地址运算，返回数组首元素的地址
    printf("%p\n",&arr);//数组指针arr参与qu取地址运算，返回数组首元素的地址
    printf("%p\n",&arr[0]);//数组指针arr参与取地址运算，返回数组首元素的地址
    //sizeof(arr)返回数组的长度，sizeof(int)返回int的长度
    printf("%d\n",sizeof(arr));  //4*5 步长：数据类型*数组长度
    printf("%d\n",sizeof(int));  //4




    return 0;
}

