#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>




int main()
{
    //定义一个数组，存储五个名字并遍历
    //字符串本质是数组，所以这是个二维数组
    char names[5][100] = {"张三", "李四", "王五", "赵六", "孙七"};
    for (int i = 0; i < 5; i++)
    {
        printf("%s\n", names[i]);
    }



    //用指针数组存储五个名字并遍历
    char *names2[5] = {"张三", "李四", "王五", "赵六", "孙七"};
    for (int i = 0; i < 5; i++)
    {
        printf("%s\n", names2[i]);
    }
    return 0;
}

