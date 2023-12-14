Implement abs function branches.
Test abs function with (INT32_MIN) & (INT32_MIN + 1).

imp1:
    原理:
        1. 若 x >= 0，產生的 mask 是 0x00000000，做 XOR 之後根本不會變，做完的結果再扣掉 mask 也不會變。
        2. 反之，若 x < 0，產生的 mask 就會是 0xFFFFFFFF，也就是二補數的 -1。上面的作法剛好就是把 x 扣掉 1 之後再反轉所有位元。
imp2:
    原理:
        1. 當 x >= 0 時，mask 為 0，於是回傳值就是 x。
        2. 當 x <= 0 時，根據程式有 abs(x) = (~x) + 1，
           驗證 abs(x) + x = (~x) + x + 1 = 0xFFF...FFF + 1 = 0，得 abs(x) 確實為 x 的負數。

Overflow: 
    在二補數中，正數的最大值是 (2 ^ (n - 1))，負數的最小值是 (-(2 ^ (n - 1) - 1))。
    因此，abs 是不能處理 (INT32_MIN) 的，最小只能處理 (INT32_MIN + 1)。


Execute:
    gcc -o abs.o abs.c
    ./abs.o
