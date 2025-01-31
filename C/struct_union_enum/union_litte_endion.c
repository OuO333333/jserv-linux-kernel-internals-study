#include <stdio.h>
union AA
{
  char a[2];  // 2 字節的字元陣列
  int s;      // 4 字節的整數
};
int main() {
  union AA aa = { 0 };  // 初始化聯合體，所有成員設為 0
  aa.a[0] = 12;   // 設定 a[0] = 0x0C
  aa.a[1] = 1;    // 設定 a[1] = 0x01
  printf("%x\n", aa.s);    
  printf("%d\n", sizeof(aa));    
  return 0;
  
  // 地址:  0x100  0x101  0x102  0x103
  // 數據:  0x0C   0x01   0x00   0x00  （小端序：低位放低地址）
  // 變數:  a[0]   a[1]   未初始化 未初始化

}
