#include<stdio.h>
#include <string.h>
int a[3];
struct { double v[3]; double length; } b[17];
int calendar[12][31];
int main() {
    b[0].v[0] = 1;
    b[0].v[1] = 2;
    b[0].v[2] = 3;
    memcpy(calendar, b, sizeof(b[0]));
}
