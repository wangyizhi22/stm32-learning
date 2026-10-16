#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main()
{
    int arr[] = {3,5,2,4,1};     
    int len = sizeof(arr) / sizeof(int); 
    

    for(int i = 0; i< len - 1;i++)
    {
        for(int j = i + 1; j < len ;j++)
    {
        if(arr[i] > arr[j])
        {
            int temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
        }
    }
    }

    for(int i = 0; i < len ;i++)
    {
        printf("%d\n",arr[i]);
    }

    


    return 0;
}