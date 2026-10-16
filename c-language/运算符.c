#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{




    //加减乘除模
     //直接计算
    printf("%d\n", 10 + 20);
    
    printf("%.2lf\n", 10.0 + 20.0);//.2表示保留两位小数 l表示long double类型 f表示float类型
    //  变量计算
    int num1 = 10;
    int num2 = 20;
    printf("%d\n", num1 + num2);
    //除法和取余
    printf("%d\n", 20 / 10);
    printf("%d\n", 20 % 10);
    //细节
    //1.整数计算的结果一定是整数
    printf("%d\n", 10 / 3);
    printf("%.2lf\n", 10.0 / 3.0);//小数计算的结果一定是小数
    //2.整数和小数计算的结果一定是小数
    printf("%.2lf\n", 10 / 3.0);
    //小数直接参与计算可能不精确
    printf("%lf\n", 10.1 / 3.0);//只保留六位小数
    //3.整数除以0会报错，程序会直接崩溃
    //printf("%d\n", 10 / 0);
    //4.小数除以0不会报错，结果是无穷大
    printf("%lf\n", 10.0 / 0.0);
    //5.整数取余的结果一定是整数，取余的数据前后都得是整数
    printf("%d\n", 10 % 3);
    //获取取余的结果的符号和被除数的符号一致
    printf("%d\n", -10 % 3);
    //不能除0
    //printf("%d\n", 10 / 0);






    return 0;
}