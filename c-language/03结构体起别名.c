#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


typedef struct saohuo
{
    char name[100];
    int saoqichengdu;
} s;


int main()
{
    s sh1 = {"wxj", 114};
    s sh2 = {"pcy", 514};
    s sh3 = {"bl", 1919};
    s sharr[3] = {sh1, sh2, sh3};

    for (int i = 0; i < 3; i++)
    {
        s temp = sharr[i];
        printf("saohuoming%s  saoqichengdu%d\n", temp.name, temp.saoqichengdu);
    }

    return 0;
}

