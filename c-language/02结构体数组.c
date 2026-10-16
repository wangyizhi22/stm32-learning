#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct student
{
    char name[100];
    int age;
};


int main()
{
    struct student stu1 = {"zhangsan", 18};
    struct student stu2 = {"lisi", 18};
    struct student stu3 = {"wangwu", 18};

    struct student stuarr[3] = {stu1, stu2, stu3};
    for (int i = 0; i < 3; i++)
    {
        struct student temp = stuarr[i];
        printf("学生姓名%s  学生年龄%d\n", temp.name, temp.age);
    }


    return 0;
}

