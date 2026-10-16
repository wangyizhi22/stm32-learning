#include <stdio.h>

int sum(int base, int adition)
{
    int sum = base + adition;
    printf("sum = %d\n", sum);
    return sum;
}








int main()
{
    int score2 = sum(93, 10);
    int score1 = sum(87, 9);
    if(score1 > score2)
    {
        printf("score1 > score2\n");
    }
    else if(score1 == score2)
    {
        printf("score1 = score2\n");
    }
    else
    {
        printf("score1 < score2\n");
    }
    return 0;
}
        