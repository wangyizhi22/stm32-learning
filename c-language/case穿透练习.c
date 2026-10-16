#include <stdio.h>

int main()
{
    int num;
    printf("请输入你所需要的服务：\n");
    scanf("%d", &num);
    switch(num)
    {
        case 1:
            printf("机票查询\n");
            break;
        case 2:
            printf("机票预订\n");
            break;
        case 3:
            printf("机票改签\n");
            break;
        case 4:
        default:
            printf("退出服务\n");
            break;
    }
    return 0;
}