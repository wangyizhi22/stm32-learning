#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>




int main()
{
    //1.利用字符数组+双引号创建字符串
    char str1[4] = "abc";
    printf("str1 = %s\n", str1);

    //细节1:在底层，c语言会把字符串abc转化为一个字符数组，末尾会自动加上一个'\0'，所以str1的长度是4
    //细节2:数组的长度可以不写
    //细节3:这种存储方法内容可以修改，因为str1是一个数组，数组的内容是可以修改的
    str1[0] = 'A';
    printf("str1 = %s\n", str1);



    //2.利用指针+双引号创建字符串
    char *str2 = "abcd";
    char* str3 = "abcd";
    printf("str2 = %s\n", str2);
    printf("str3 = %s\n", str3);
    //细节1:在底层，c语言会把字符串abc转化为一个字符数组，末尾会自动加上一个'\0'，所以str2的长度是4
    //细节2:这种存储方法内容不可以修改，因为str2是一个指针，指针指向的内容是常量，不能修改
    //str2[0] = 'A'; //错误，不能修改
    //printf("str2 = %s\n", str2);
    //只读常亮区的特点：1.内容不可以修改 2.程序结束后，系统会自动释放内存 3.可以复用
    printf("%p\n", str2);
    printf("%p\n", str3);






    return 0;
}

