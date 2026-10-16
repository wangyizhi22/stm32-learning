#include <stdio.h>

int main()
{
    //for循环的语法格式：
    //for(初始化表达式;循环条件;循环后操作)
    //循环条件为真时，执行循环体，循环后操作执行完毕后，继续判断循环条件是否为真，若为真，则继续执行循环体，若为假，则结束循环
    //for循环的初始化表达式只会执行一次，循环后操作每次循环都会执行
    //for循环的循环条件可以省略不写，
    //若省略不写，则循环条件默认为真，for循环会一直执行
    //for循环的初始化表达式和循环后操作可以省略不写，若省略不写，则需要在循环体内手动控制循环条件，否则会造成死循环
    for (int i = 1; i <= 5; i++)
    {
        printf("%d\n", i);
    }
    for (int i = 5; i >= 1; i--)
    {
        printf("%d\n",i);
    }
    
    
    
    int sum = 0;//  累加
    for (int i = 1; i <= 5; i++)
    {
        sum += i;
    }
    printf("sum = %d\n", sum);
//变量的生命周期：只在所属大括号内有效，出了大括号就失效了
//如果想每次循环一个新变量，则需要在循环体内定义变量


    //获取1-100之间的所有偶数的和
    int evenSum = 0;
    for (int i = 2; i <= 100; i += 2)
    {
        evenSum += i;
    }
    printf("1-100之间的所有偶数的和为: %d\n", evenSum);


    //统计思想
    //录入两个数字表示范围，统计范围内既能被6整除又能被8整除的数
    int start, end;
    printf("请输入两个数字表示范围: ");
    scanf("%d %d", &start, &end);
    int min = start < end ? start : end;
    int max = start > end ? start : end;
    int count = 0;
    for (int i = min; i <= max; i++)
    {
        if (i % 6 == 0 && i % 8 == 0)
        {
            count++;
        }
    }
    printf("范围内既能被6整除又能被8整除的数有: %d个\n", count);

    return 0;
}