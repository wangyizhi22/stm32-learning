#include <stdio.h>

int main()
{
    //&&两边都为真，结果才为真
    //||两边都为假，结果才为假
    //!取反，真变假，假变真
    int a = 10;
    int b = 20;
    printf("%d\n",a && b);
    printf("%d\n",a || b);
    printf("%d\n",!a);
    printf("%d\n",!b);




    //拓展点
    //逻辑运算符的短路特性
    //用输入用户名和密码来理解，一个能判断下一个就不运行
    
    return 0;
}