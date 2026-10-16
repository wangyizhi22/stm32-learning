#include <stdio.h>

int main()
{
    int hp = 200;
    int damage = 80;
    int restore = 100;

    hp = hp - damage + restore;
    if(hp > 200)
    {
        hp = 200;
    }   
    printf("HP: %d\n", hp);
//细节
//判断的结果是一个数字，非0表示成立，0表示不成立
//如果大括号内语句体只有一行的话，大括号可以省略不写



    int satisfy = 1;
    if(satisfy)
    {    
        printf("满足条件\n");
    }
    else
    {
        printf("不满足条件\n");
    }   
    return 0;
}