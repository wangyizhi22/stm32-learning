#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    int arr1[] ={1,2,3,4,5};
    int arr2[] = { 6,7,8,9,10 };
    int arr3[] = { 11,12,13,14,15 };
    int* arr[3] = { arr1,arr2,arr3 };
    int** p = arr;

   

  // 遍历
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printf("%d ", *(*p + j));
        }
        printf("\n");
        p++;
    }
    return 0;
}