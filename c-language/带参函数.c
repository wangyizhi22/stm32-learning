#include <stdio.h>

void sum(int num1, int num2)
{
    int result = num1 + num2;
    printf("sum = %d\n", result);
}




void dayin(int num)
{
    for (int i = 1; i <= num; i++)
    {
        printf("阿伟giegie\n");
    }
    
}



int main()
{
    sum(5, 10);// 形式参数必须和实际参数类型一致，个数一致
    dayin(5);
    return 0;
}
        