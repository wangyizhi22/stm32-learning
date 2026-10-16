#include <stdio.h>

int main()
{
    int i;
    printf("%d\n",(i = 3, ++i, ++i));
    //一定要小括号
    
    return 0;
}