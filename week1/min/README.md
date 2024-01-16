Implement min function without branches.  
Test min function.

原理:  
    &nbsp;&nbsp;&nbsp;&nbsp;1. a - b = 0:  
       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(0 & (0 >> 31))  
       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return b  
    &nbsp;&nbsp;&nbsp;&nbsp;2. a - b > 0:  
       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;diff 最高位為 0, 因此右移 31 bit = 0  
       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(diff & 0) = 0  
       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return b  
    &nbsp;&nbsp;&nbsp;&nbsp;3. a - b < 0:  
       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;diff 最高位為 1，因此右移 31 bit = 0xFFFFFFFF(注意是算術位移)  
       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(diff & 0xFFFFFFFF) = diff = (a - b)  
       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return b + (a - b) = a

Overflow:  
    &nbsp;&nbsp;&nbsp;&nbsp;INT32_MIN <= (a - b) <= INT32_MAX 條件成立時, 才會正確。  
    &nbsp;&nbsp;&nbsp;&nbsp;由於 INT32_MAX - INT32_MIN >= INT32_MAX, 所以 min(INT32_MAX, INT32_MIN)會錯誤  

類似常見用途:  
    &nbsp;&nbsp;&nbsp;&nbsp;考慮以下程式碼:  
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int32_t a, b, c, x;  
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;...  
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;if (a > b)  
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;x += c;  
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;當 INT32_MIN <= (b - a) <= INT32_MAX 條件成立時, 我們可改寫 if (a > b) x += c; 為以下程式碼:  
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;x += ((b - a) >> 31) & c;  

Execute:  
    &nbsp;&nbsp;&nbsp;&nbsp;gcc -o min.o min.c  
    .&nbsp;&nbsp;&nbsp;&nbsp;/min.o
