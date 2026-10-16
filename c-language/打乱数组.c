#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int main()
{
    int arr[] = {1,2,3,4,5};     
    int len = sizeof(arr) / sizeof(int); 
    srand(time(NULL));

    
    for(int i = 0; i < len; i++)              // for循环用分号分隔，i初始化为0，去掉末尾分号
    {
        int index = rand() % len;
        int temp = arr[i];
        arr[i] = arr[index];
        arr[index] = temp;
    }

    

    
    for(int i = 0; i < len; i++)
    {
        printf("%d\n",arr[i]);
    }




    return 0;
}