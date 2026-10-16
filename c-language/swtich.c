#include <stdio.h>

int main()
{//switch最好是对有限个数进行匹配
    //if主要是对范围进行匹配
    int day;
    printf("请输入今天是星期几：\n");
    scanf("%d",&day);
    switch(day)//表达式必须是整型
    {
         case 1: //只能是整数或字符，且case后的值不能重复，
         //case穿透：没遇到break继续执行直到遇到break。**细节只会往下穿，不会往上穿
            printf("跑步\n");
            break;
         case 2:
            printf("游泳\n");
            break;
         case 3:
            printf("慢走\n");
            break;
         case 4:
            printf("单车\n");
            break;
         case 5:
            printf("爬山\n");
            break;
        default:
            printf("没有这个星期");
            break;
    }
    return 0;
}