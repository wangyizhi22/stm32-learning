#include <stdio.h>

int main()
{
    int a = 10;
    printf("%p\n",&a);

    printf("--------------");
    int arr[] = {1,2,3};
    printf("%p\n",&arr);//首地址
    printf("%p\n",&arr[0]);
    printf("%p\n",&arr[1]);
    printf("%p\n",&arr[2]);


    
    return 0;
}
        