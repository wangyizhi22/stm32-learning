#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct girlfriend
{
    char name[100];
    int age;
    float height;
};


int main()
{
    struct girlfriend gf1;
    strcpy(gf1.name, "jz");
    gf1.age = 18;
    gf1.height = 1.7;
    printf("name:%s\n", gf1.name);
    printf("age:%d\n", gf1.age);
    printf("height:%f\n", gf1.height);



    struct girlfriend gf2;
    strcpy(gf2.name, "jz2");
    gf2.age = 18;
    gf2.height = 1.7;
    printf("name:%s\n", gf2.name);
    printf("age:%d\n", gf2.age);
    printf("height:%f\n", gf2.height);



   

    return 0;
}

