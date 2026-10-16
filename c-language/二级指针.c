#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>




int main()
{
    //定义变量
    int a = 10;
    int b = 20;

    //定义一级指针
    int *p = &a;

    //定义二级指针
    int **pp = &p;

    //二级指针的作用一：修改一级指针里面记录的地址
    *pp = &b;
    printf("%p\n",&a);
    printf("%p\n",&b);
    printf("pp 存的地址(p的地址): %p\n", pp);      // 0x3000
    printf("p 存的地址(b的地址):  %p\n", p);       // 0x2000（因为 *pp=&b 让 p 指向了 b）
    printf("*pp 就是 p 本身:     %p\n", *pp);      // 0x2000，和 p 一样


    //二级指针的作用二：修改一级指针所记录的变量的值
    **pp = 30;
    printf("**pp = %d\n", **pp);
    printf("*p = %d\n", *p);

    
// p一个盒子，里面写着"东西在 3 号柜"
// *p
// 打开盒子，按提示去 3 号柜拿东西
// pp
// 一个盒子，里面写着"p 那个盒子在 5 号柜"
// **pp
// 打开 pp → 找到 p 的盒子 → 打开 p → 拿到最终的东西
// 每多一层指针，就多套了一层盒子。每多一个 *，就多拆一层。



    return 0;
}

