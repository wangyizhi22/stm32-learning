#include <stdio.h>

int sum(int m1, int m2,int m3)
{
    int sum = m1 + m2 + m3;
    printf("sum = %d\n", sum);
    return sum;
}








int main()
{
    int f1 = sum(10, 20, 15);
    int f2 = sum(20, 30, 17);
    int f3 = sum(19, 17, 20);
    int f4 = sum(23, 21, 19);
    if(f1 > f2 && f1 > f3 && f1 > f4)
    {
        printf("f1 is the largest\n");
    }
    else if(f2 > f1 && f2 > f3 && f2 > f4)
    {
        printf("f2 is the largest\n");
    }
    else if(f3 > f1 && f3 > f2 && f3 > f4)
    {
        printf("f3 is the largest\n");
    }
    else
    {
        printf("f4 is the largest\n");
    }
    return 0;
}
        