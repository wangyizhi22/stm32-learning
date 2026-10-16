#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int arrprintf(int arr[], int len)
{
    for(int i = 0;i < len;i++)
    {
        printf("%d",arr[i]);

    }
    printf("\n");
    return 0;
}

int main()
{
    int arr[5] = {0};     
    int len = sizeof(arr) / sizeof(int); 

    
    for(int i = 0; i < len; i++)              // for循环用分号分隔，i初始化为0，去掉末尾分号
    {
        printf("请输入第%d个数字",i + 1);
        scanf("%d",&arr[i]);
    }

    int q = 0;
    int j = len - 1;
    
        while(q < j)
        {
            int temp = arr[q];
            arr[q] = arr[j];
            arr[j] = temp;

            q++;    // 头往前走一步
            j--;    // 尾往后退一步
        }
    




    
    arrprintf(arr, len);




    return 0;
}