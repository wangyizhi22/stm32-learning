#include <stdio.h>

int main()
{
    short a = 100;
    short b = 20;
    //隐士转换为int
    short sum = a + b;
    //这里又转换回来了
    printf("%zu\n", sizeof(sum));
    
    return 0;
}