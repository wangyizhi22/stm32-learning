#include <stdio.h>

int main()
{
    int ai;
    printf("请输入心爱的好感度的值：");
    scanf("%d", &ai);
    if(ai > 60)
    {
        printf("同意表白");
    }
    else
    {
        printf("表白别人");
    }

    return 0;
}