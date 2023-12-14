Implement min function without branches.  
Test min function.

原理:  
    1. a - b = 0:  
       &nbsp;&nbsp;&nbsp;&nbsp;(0 & (0 >> 31))  
       &nbsp;&nbsp;&nbsp;return b  
    2. a - b > 0:  
       &nbsp;&nbsp;&nbsp;diff 最高位為 0, 因此右移 31 bit = 0  
       &nbsp;&nbsp;&nbsp;(diff & 0) = 0  
       &nbsp;&nbsp;&nbsp;return b  
    3. a - b < 0:  
       &nbsp;&nbsp;&nbsp;diff 最高位為 1，因此右移 31 bit = 0xFFFFFFFF(注意是算術位移)  
       &nbsp;&nbsp;&nbsp;(diff & 0xFFFFFFFF) = diff = (a - b)  
       &nbsp;&nbsp;&nbsp;return b + (a - b) = a

Overflow:  
    INT32_MIN <= (b - a) <= INT32_MAX 條件成立時, 才會正確。  
    由於 INT32_MAX - INT32_MIN >= INT32_MAX, 所以 min(INT32_MAX, INT32_MIN)  

類似常見用途:  
    考慮以下程式碼:  
        int32_t a, b, c, x;  
        ...  
        if (a > b)  
        x += c;  
        當 INT32_MIN <= (b - a) <= INT32_MAX 條件成立時, 我們可改寫 if (a > b) x += c; 為以下程式碼:  
        x += ((b - a) >> 31) & c;  

Execute:  
    gcc -o min.o min.c  
    ./min.o
