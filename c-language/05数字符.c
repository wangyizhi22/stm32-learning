#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>




int main()
{
   char str[100];
//不能用指针，因为指针指向的内容是常量，不能修改

   printf("请输入一个字符串：");
    scanf("%s", str);
    int bigcount = 0;
    int smallcount = 0;
    int numcount = 0;
    for (int i = 0; i <= strlen(str); i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            bigcount++;
        }
        else if (str[i] >= 'a' && str[i] <= 'z')
        {
            smallcount++;
        }
        else if (str[i] >= '0' && str[i] <= '9')
        {
            numcount++;
        }
    }
    printf("大写字母个数：%d\n", bigcount);
    printf("小写字母个数：%d\n", smallcount);
    printf("数字个数：%d\n", numcount);

    return 0;
}

