#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * 冒泡排序
 * 思路：每一轮把当前范围内最大的数「冒」到末尾。
 *   外层 i —— 控制轮数，最多 len 轮
 *   内层 j —— 在 [0, len-1-i) 范围内两两比较，前面比后面大就交换
 * 为什么上界是 len-1-i：末尾 i 个元素已经排好了，不用再比。
 */
int main()
{
    int arr[] = {3, 5, 2, 4, 1};
    int len = sizeof(arr) / sizeof(int);

    // 先打印原数组，方便和排序后对照
    printf("排序前: ");
    for (int j = 0; j < len; j++)
    {
        printf("%d ", arr[j]);
    }
    printf("\n");

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len - 1 - i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    printf("排序后: ");
    for (int j = 0; j < len; j++)
    {
        printf("%d ", arr[j]);
    }
    printf("\n");

    return 0;
}
