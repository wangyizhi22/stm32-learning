#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>




int main()
{
   char *rightusername = "van";
   char *rightpassword = "123456";
   char username[100];
    char password[100];
   
    for(int i = 1; i<=3; i++)
    {


        printf("请输入用户名：");
        scanf("%s", username);
        printf("请输入密码：");
        scanf("%s", password);

        if (strcmp(username, rightusername) && strcmp(password, rightpassword))
        {
            printf("用户名或密码错误,你还有%d次机会\n", 3 - i);
        }
        else
        {
            printf("登录成功\n");
            break;
        }

        if (i == 3)
        {
            printf("登录失败,可惜，账户：%s\n已被锁定", rightusername);
            break;
        }
    }
    

    return 0;
}

