#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

// 通用交换函数：能交换任意类型
void swap(void* a, void* b, size_t size) {
    // 用 temp 做中转，逐字节交换
    char temp[size];          // 或者用 malloc
    
    memcpy(temp, a, size);    // 把 a 的内容拷贝到 temp
    memcpy(a, b, size);       // 把 b 的内容拷贝到 a
    memcpy(temp, b, size);    // 把 temp 的内容拷贝到 b
}

int main() {
    // 交换 int
    int x = 10, y = 20;
    printf("交换前: x=%d, y=%d\n", x, y);
    swap(&x, &y, sizeof(int));
    printf("交换后: x=%d, y=%d\n\n", x, y);

    // 交换 double
    double d1 = 1.5, d2 = 9.8;
    printf("交换前: d1=%.2f, d2=%.2f\n", d1, d2);
    swap(&d1, &d2, sizeof(double));
    printf("交换后: d1=%.2f, d2=%.2f\n\n", d1, d2);

    // 交换 char
    char c1 = 'A', c2 = 'Z';
    printf("交换前: c1=%c, c2=%c\n", c1, c2);
    swap(&c1, &c2, sizeof(char));
    printf("交换后: c1=%c, c2=%c\n\n", c1, c2);

    // 交换结构体
    typedef struct {
        int age;
        char name[20];
    } Person;

    Person p1 = {18, "Alice"};
    Person p2 = {25, "Bob"};

    printf("交换前: p1=%d/%s, p2=%d/%s\n", p1.age, p1.name, p2.age, p2.name);
    swap(&p1, &p2, sizeof(Person));
    printf("交换后: p1=%d/%s, p2=%d/%s\n", p1.age, p1.name, p2.age, p2.name);

    return 0;
}