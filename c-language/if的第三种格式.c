#include <stdio.h>

int main()
{
    int total;

    printf("请输入考试成绩\n");
    scanf("%d",&total   );


    if(total >= 85 && total <=100)
    {
     printf("A\n");
    }
    else if(total >= 70 && total <=84)
    {
     printf("B\n");
    }
    else if(total >= 60 && total <=69)
    {
     printf("C\n");
    }
    else
    {
     printf("D\n");
    }
    return 0;
}