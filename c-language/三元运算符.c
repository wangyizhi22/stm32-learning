#include <stdio.h>

int main()
{
    int a = 10;
    int b = 20;
    int c = 30;
    int ab = a > b ? a : b;
    int abc = ab > c ? ab : c;
    printf("abc = %d\n", abc);
    //样例：关系表达式？表达式1 ： 表达式2
    //首先计算关系表达式的值，如果为真，则计算表达式1的值，否则计算表达式2的值
    return 0;
}