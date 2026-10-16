#include <stdio.h>

int main()
{
    char c1 = '你';
    printf("%c\n",c1);
    //不能用中文
    printf("%zu\n",sizeof(c1));
    //sizeof()函数返回的是字节数，char类型占1个字节
    
    return 0;
}