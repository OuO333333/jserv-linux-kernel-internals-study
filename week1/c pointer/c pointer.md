變數位址&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;變數值&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;變數名稱  
0xXXXXXXXX&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;2&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;b   
0xYYYYYYYY&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;0xXXXXXXXX&nbsp;&nbsp;&nbsp;&thinsp;pointer


  
  
指標(Pointer): 某變數的位址  
指標變數(Pointer Variable): 用來存放指標的變數  

&b = 0xXXXXXXXX(取出變數的位址)  
*&b = 2(取出位址的值)  
  
-------------------------------------------------------------  
  
變數位址&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;變數值&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;變數名稱&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;程式碼  
0xXXXXXXXX&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;2(int)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;b&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;int b = 2;  
0xYYYYYYYY&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;0xXXXXXXXX&nbsp;&nbsp;&nbsp;&thinsp;pointer&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int *pointer = &b;  
  
int *pointer = &b;  
-> *表示這個變數是指標  
-> pointer 表示這個變數的名稱  
-> int 代表這個指標變數指向的變數的類型  
  
-------------------------------------------------------------  
  
int b = 2;  
int *pointer = &b;  
*pointer = 100; 表示 &b 這個位址的值被 assign 成 100。  
所以 *pointer = 100, b = 100。  
*pointer 代表的就是變數 b。所以我們可以把 *pointer 當作變數 b 來使用。  
  
-------------------------------------------------------------  
  
void qsort(void *base, size_t nmeb, size_t size,  
           int (*compar)(const void *, const void *));  
-> void *base 是一段記憶體位址  
-> size_t nmeb 是一個單位有多大  
-> size_t size 是數量  

-------------------------------------------------------------  

In the expression x[i], which is equivalent to (*((x)+(i))), 所以以下程式  
int main() {  
&nbsp;&nbsp;&nbsp;&nbsp;int x[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};  
&nbsp;&nbsp;&nbsp;&nbsp;printf("%d %d %d %d\n", x[4], *(x + 4), *(4 + x), 4[x]);  
}  
的結果會是  
4&nbsp;4&nbsp;4&nbsp;4  
  
-------------------------------------------------------------  
  
As formal parameters in a function definition, char s[]; and char *s are equivalent.  
這個語句指的是在函數定義中，使用 char s[] 和 char *s 作為形式參數是等效的。  
  
-------------------------------------------------------------  
  
sizeof(int) = 4  
sizeof(double) = 8
  
-------------------------------------------------------------  
  
int a[3];  
struct { double v[3]; double length; } b[17];  
int calendar[12][31];  
int main() { }  
gcc -o s -Og -g gdb.c  
gdb -q s  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/ecd3fff1-b0d6-405b-bb77-655200250f67)  
&b+1 是遞移到整個 b 數組下一個結構體的位置,這邊是遞移到 a, 取決於 compiler 怎麽分配計記憶體。  
&b[0]+1 是遞移一個 b[0] 佔用的空間。
