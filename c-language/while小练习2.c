#include <stdio.h>

int main()
{
    double h = 0.1;
    int count = 0;
    while(h <= 8844430)
    {
        h = h * 2;
        count++;

    }
    printf("h = %lf\n", h);
    printf("count = %d\n", count);



    return 0;
}