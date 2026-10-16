#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int erfenchazhao(int arr[],int len,int num)
{
    int min = 0;
    int max = len - 1;

    while(min <= max)
    {
        int mid = (min + max) / 2;
        if (arr[mid] < num)
        {
            min = mid + 1;
        }
        else if (arr[mid] > num)
        {
            max = mid - 1;

        }
        else{
            return mid;

        }
        
    }
    return -1;
}



int main()
{
    int arr[] = {1,2,3,4,5,6,7,8};     
    int len = sizeof(arr) / sizeof(int); 
    
    int num = 7;
    int index = erfenchazhao(arr, len ,num);
    printf("%d\n",index);

    return 0;
}