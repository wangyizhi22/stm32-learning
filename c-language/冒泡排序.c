#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main()
{
    int arr[] = {3,5,2,4,1};     
    int len = sizeof(arr) / sizeof(int); 
    

    for(int i = 0; i< len;i++)
    {
        for(int j = 0; j < len - 1 - i;j++)
    {
        if(arr[j] > arr[j+1])
        {
            int temp = arr[j];
            arr[j] = arr[j+1];
            arr[j+1] = temp;
        }
    }
    }

    for(int j = 0; j < len ;j++)
    {
        printf("%d\n",arr[j]);
    }

    


    return 0;
}