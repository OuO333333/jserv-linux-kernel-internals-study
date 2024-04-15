#include <stdio.h>

// 结构B
struct B {
    int x;
    char str[30];
};

// 结构C
struct C {
    double y;
    float z;
};

// 结构A 包含B和C的指针
struct A {
    struct B *ptr_to_B;
    struct C *ptr_to_C;
};

int main() {
    // 输出各结构的大小
    printf("sizeof(struct B): %ld\n", sizeof(struct B));
    printf("sizeof(struct C): %ld\n", sizeof(struct C));
    printf("sizeof(struct A): %ld\n", sizeof(struct A));
    printf("sizeof(int): %ld\n", sizeof(int));
    printf("sizeof(char): %ld\n", sizeof(char));
    printf("sizeof(double): %ld\n", sizeof(double));
    printf("sizeof(float): %ld\n", sizeof(float));

    return 0;
}

