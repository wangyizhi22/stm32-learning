#include <stdio.h>

int main()
{
    //eg:+=
    //把左右先相加，后赋值给左边，对右边无任何影响
    //还有-= *= /= %=
    int a = 10;
    int b = 20;
    a *= b;
    printf("%d\n",a);
    printf("%d\n",b);
    return 0;
}