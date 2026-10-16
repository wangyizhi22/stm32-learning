#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 1. 函数声明（末尾必须有分号）
void qiuzuidazuixiao(int arr[], int len, int* max, int* min);

int main()
{
    int arr[] = {1, 2, 3, 4, 5};     
    int len = sizeof(arr) / sizeof(int); 
    
    // 2. 定义普通int变量，并初始化为数组第一个元素
    int max = arr[0];
    int min = arr[0];
    
    // 3. 函数调用（不要写void，arr不要加[]，传变量的地址）
    qiuzuidazuixiao(arr, len, &max, &min);
    
    // 4. 输出结果
    printf("数组最大值为: %d\n", max);
    printf("数组最小值为: %d\n", min);

    return 0;
}

// 5. 函数定义
void qiuzuidazuixiao(int arr[], int len, int* max, int* min)
{
    // 先初始化为第一个元素
    *max = arr[0];
    *min = arr[0];
    
    for(int i = 1; i < len; i++)  // 从1开始即可，0已经用过了
    {
        if(arr[i] > *max)
        {
            *max = arr[i];   // 把更大的值赋给max
        }
        if(arr[i] < *min)
        {
            *min = arr[i];   // 把更小的值赋给min
        }
    }
}