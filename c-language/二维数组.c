#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    int arr1[] = { 1,2,3};
    int arr2[] = { 6,7,8,9,10 };
    int arr3[] = { 1,2,3,4,5,6,7,8,9,10 };
    int len1 = sizeof(arr1) / sizeof(int);
    int len2 = sizeof(arr2) / sizeof(int);
    int len3 = sizeof(arr3) / sizeof(int);

    int lenArr[3] = { len1, len2, len3 };


    int* allarr[3] = { arr1, arr2, arr3 };  // ✅ 指针数组//不能直接赋值，因为数组名是数组首地址，会退化为指针

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < lenArr[i]; j++)。//二维数组
        {
            printf("%d ", allarr[i][j]);
            // allarr[i] → 拿到第 i 个 int* 指针
            // [j] → 在该指针上偏移 j 个 int，解引用
        }
        printf("\n");
    }

    return 0;
}