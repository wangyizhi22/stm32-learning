#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    int arr[] = {33, 5, 22, 44, 55, 66};      // 用花括号初始化数组
    int max = arr[0];
    int len = sizeof(arr) / sizeof(arr[0]);   // 正确计算数组长度
    
    for(int i = 0; i < len; i++)              // for循环用分号分隔，i初始化为0，去掉末尾分号
    {
        if (arr[i] > max)                     // 用数组元素和max比较
        {
            max = arr[i];                     // 更新最大值，加分号
        }
    }
    
    printf("数组中的最大值是：%d\n", max);     // 加分号
    return 0;
}