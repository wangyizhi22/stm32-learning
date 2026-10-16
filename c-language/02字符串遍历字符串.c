#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>




int main()
{
    char str[100];
    //不能用指针，因为指针指向的内容是常量，不能修改

    printf("请输入字符串：\n");
    scanf("%s", str);//str本身是指针，不用加&符号
    
    //遍历字符串：遍历数组，不过数组的长度不确定不知道范围，所以要用'\0'来判断结束

    char *p = str;
    while (1)
    {
        char c = *p;
        if (c == '\0')
        {
            break;
        }
        printf("%c\n", c);
        p++;
    }





    return 0;
}

