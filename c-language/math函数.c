#include <stdio.h>
#include <math.h>

int main()
{
    double result = pow(2, 1.2);
    printf("2的1.2次方是：%.1f\n", result);


    double sqrt_result = sqrt(16);
    printf("16的平方根是：%.2f\n", sqrt_result);


    double ceil_result = ceil(3.7);
    printf("3.7向上取整是：%.0f\n", ceil_result);

    double floor_result = floor(3.7);
    printf("3.7向下取整是：%.0f\n", floor_result);


    double abs_result = abs(-5);
    printf("-5的绝对值是：%.0f\n", abs_result); 


    


    return 0;
}