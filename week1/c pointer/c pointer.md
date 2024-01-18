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
  
<pre><code>int *pointer = &b;</code></pre>  
-> *表示這個變數是指標  
-> pointer 表示這個變數的名稱  
-> int 代表這個指標變數指向的變數的類型  
  
-------------------------------------------------------------  
  
```c
int b = 2;  
int *pointer = &b;  
```  
*pointer = 100; 表示 &b 這個位址的值被 assign 成 100。  
所以 *pointer = 100, b = 100。  
*pointer 代表的就是變數 b。所以我們可以把 *pointer 當作變數 b 來使用。  
  
-------------------------------------------------------------  
  
```c
void qsort(void *base, size_t nmeb, size_t size,  
           int (*compar)(const void *, const void *));
```
-> void *base 是一段記憶體位址  
-> size_t nmeb 是一個單位有多大  
-> size_t size 是數量  

-------------------------------------------------------------  

In the expression x[i], which is equivalent to (*((x)+(i))), 所以以下程式  
```c
int main() {  
    int x[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};  
    printf("%d %d %d %d\n", x[4], *(x + 4), *(4 + x), 4[x]);  
}
```
的結果會是  
4&nbsp;4&nbsp;4&nbsp;4  
  
-------------------------------------------------------------  
  
As formal parameters in a function definition, char s[]; and char *s are equivalent.  
這個語句指的是在函數定義中，使用 char s[] 和 char *s 作為形式參數是等效的。  
  
-------------------------------------------------------------  
  
sizeof(int) = 4  
sizeof(double) = 8
  
-------------------------------------------------------------  
  
```c
int a[3];  
struct { double v[3]; double length; } b[17];  
int calendar[12][31];  
int main() { }
```
gcc -o s -Og -g gdb.c  
gdb -q s  
  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/ecd3fff1-b0d6-405b-bb77-655200250f67)  
&b+1 是遞移到整個 b 數組下一個結構體的位置,這邊是遞移到 a, 取決於 compiler 怎麽分配計記憶體。  
&b[0]+1 是遞移一個 b[0] 佔用的空間。
簡單來說, +1 也好, -1 也好, 他 +1/-1 的單位取決於前面的 &(?), ? 東西的大小。  
  
-------------------------------------------------------------  
  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/43a7cca7-0e72-4fa2-ad9e-1af10641409d)  

![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/74560aa8-2269-428a-aeed-6c252f1e59b4)![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/1f005f2e-bf62-402c-839d-ae8d2d93e6b7)  
在 C 語言中, b[0].v 和 b[0]->v 是等價的, 兩者都用於訪問結構成員 v。  
兩種寫法的選擇通常取決於使用的表達風格。  
1\. b[0].v：使用.運算符，通常在直接使用結構的實例時使用。  
```c
double d = b[0].v[0];
```
2\. b[0]->v：使用->運算符，通常在操作指向結構的指標時使用。  
```c
struct { double v[3]; double length; } *ptr = &b[0];  
double firstElement = ptr->v[0];
```

 &b[0].v 實際上是 &(b[0].v), 所以是個指標。  
 &b[0]->v 實際上是 &(b[0]->v), 所以是個指標。  
 (&b[0])->v 是在取得 b[0] 的地址之後，再使用箭頭運算子 -> 來存取結構的成員 v。這與 b[0].v 是等效的。  
  
-------------------------------------------------------------  
  
gcc -o s -g gdb.c  
gdb s  
list  
break 10  
p b[0].v  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/679a7202-34a6-4364-9d47-19635fd24f70)  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/901b604f-46c7-41c5-8b7a-ea2f5f640260)  
這是因為原本是個 double(8 bytes), 取值的時候被當成 int(4 bytes), 只取了前面 4 bytes, 所以為 0。  
下圖顯示了該 double 的 8 個byte。  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/3c35fa3b-5750-4616-9961-7ce2b4d084f2)  
用 little endian 將 0	0	0	0	0	0	-16	63 排成 0x3FF0000000000000 後, 0x3FF0000000000000 根據 IEEE 754 雙精度浮點轉成 double 後即為 1。  
  
-------------------------------------------------------------  
  
gcc -o s -g gdb.c  
gdb s  
break 11  
run  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/4918e790-6192-456d-870d-e95ce34049e2)  
用 memcpy 將 b[0] copy 到 calendar,  
p calendar[0][0] 讀取時因為 calendar 是 int, 所以只讀 4 bytes, 為0。  
p *(double *) &calendar[0][0] 讀取時是讀 8 bytes, 所以是 1。
  
-------------------------------------------------------------  

```c
#include <stdio.h>
int main(int argc, char (*argv)[0])
{
  puts(((char **) argv)[0]);
  return 0;
}
```c
gcc -o arg arg.c
