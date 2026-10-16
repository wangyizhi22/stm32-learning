#include <stdio.h>

int main()
{
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            printf("%d*%d=%d \t", j, i, i * j);//\t表示制表符，输出对齐
        }
        printf("\n");
    }


    printf("name\t\tage\t\n");
    printf("zhangsan\t20\n");
    return 0;
}
        