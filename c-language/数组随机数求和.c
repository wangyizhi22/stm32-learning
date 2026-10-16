#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int contains(int arr[], int len,int num)
{
    for(int i = 0;i < len;i++)
    {
        if (arr[i] == num )
        return 1;

    }
    return 0;
}

int main()
{
    int arr[10] = {0};     
    int len = sizeof(arr) / sizeof(int); 
    srand(time(NULL));

    
    for(int i = 0; i < len; )              // for循环用分号分隔，i初始化为0，去掉末尾分号
    {
        int num = rand() % 10 + 1;
         int flag = contains(arr,len,num);
         if(!flag)
         {
            arr[i] = num;
            i++;
         }
    }
    
    int sum = 0;
    for(int i = 0; i < len; i++)
    {
        sum = sum + arr[i];
    }
    // 打印10个不重复数字
    for(int i = 0; i < len; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("%d\n",sum);

    float avg = (float)sum / len;
    printf("平均数 = %.2f\n", avg);




    return 0;
}