Reference: https://www.yuanguohuo.com/2020/03/31/linux-preemption-mode/  
  
在 Ubuntu 22.04, 6.5.0-35-generic 上實驗不同的 preemption mode。  
Linux kernel 有多種 Preemption mode, 這邊我們介紹其中 3 種。  
1. No Forced Preemption (Server)  
2. Voluntary Kernel Preemption (Desktop)  
3. Preemptible Kernel (Low-Latency Desktop)

------------------------------------------------------------------------------------------------  
選擇”No Forced Preemption (Server)”產生的配置項是：
```
CONFIG_PREEMPT_NONE=y
# CONFIG_PREEMPT_VOLUNTARY is not set
# CONFIG_PREEMPT is not set
```
選擇”Voluntary Kernel Preemption (Desktop)”產生的配置項是：
```
# CONFIG_PREEMPT_NONE is not set
CONFIG_PREEMPT_VOLUNTARY=y
# CONFIG_PREEMPT is not set
```
選擇”Preemptible Kernel (Low-Latency Desktop)”產生的配置項是：
```
CONFIG_PREEMPT_RCU=y
# CONFIG_PREEMPT_NONE is not set
# CONFIG_PREEMPT_VOLUNTARY is not set
CONFIG_PREEMPT=y
CONFIG_PREEMPT_COUNT=y
CONFIG_DEBUG_PREEMPT=y
# CONFIG_PREEMPT_TRACER is not set
```  
先來查看 6.5.0-35-generic kernel 的 Preemption mode 的相關參數:
```
cat /boot/config-$(uname -r) | grep CONFIG_PREEMPT
```
```
CONFIG_PREEMPT_BUILD=y
# CONFIG_PREEMPT_NONE is not set
CONFIG_PREEMPT_VOLUNTARY=y
# CONFIG_PREEMPT is not set
CONFIG_PREEMPT_COUNT=y
CONFIG_PREEMPTION=y
CONFIG_PREEMPT_DYNAMIC=y
CONFIG_PREEMPT_RCU=y
CONFIG_PREEMPT_NOTIFIERS=y
# CONFIG_PREEMPT_TRACER is not set
# CONFIG_PREEMPTIRQ_DELAY_TEST is not set
```
可以看出 6.5.0-35-generic kernel 的預設是 Voluntary Kernel Preemption (Desktop)。  

------------------------------------------------------------------------------------------------  
在 6.5.0-35-generic kernel 中, 已經可以動態的去改變 Preemption mode(早期的 linux kernel 需先編譯過才能改變),  
改變方式如下:  
```
echo "none" | sudo tee /sys/kernel/debug/sched/preempt
```
查看當前 Preemption mode 方法如下:  
```
sudo cat /sys/kernel/debug/sched/preempt
```

------------------------------------------------------------------------------------------------  
以下對不同的 Preemption Mode 進行測試,  
task 1: sleep 3 秒  
task 2: sleep 1 秒, 空轉 6 秒, might_sleep(), 空轉 6 秒  
使用步驟為:  
1. make(編譯 mychardev.c)
2. sudo insmod mychardev.ko
3. sed -n '/^Character/, /^$/ { /^$/ !p }' /proc/devices
4. gcc pthread.c -lpthread -o pthread

------------------------------------------------------------------------------------------------ 
No Forced Preemption (Server) 下只有 2 種情況會進行線程切換:  
1. 從內核態返回用戶態的時候
2. 在內核模式下主動放棄CPU（等待資源）(sleep) 算這種  

使用 1 個 CPU 執行結果為:  
```
sudo taskset --cpu-list 0 ./pthread
```
```
T1 start at 1718264162.000305293
T2 start at 1718264162.000305403
T1 stop at 1718264175.000304070. elapse: 12.999998777 seconds.
T2 stop at 1718264175.000304133. elapse: 12.999998730 seconds.
```
使用多個 CPU 執行結果為:  
```
sudo ./pthread
```
```
T1 start at 1718264246.000711656
T2 start at 1718264246.000711657
T1 stop at 1718264249.000711755. elapse: 3.000000099 seconds.
T2 stop at 1718264259.000979032. elapse: 13.000267375 seconds.
```
結果如預期, 多個 CPU 可直接視為平行處理,  
1 個 CPU, 則 t1 sleep 後轉到 t2 sleep, 接著 t2 會執行空轉 6 秒, t2 繼續空轉 6 秒, 此時 t1, t2 結束(13 秒)。  

------------------------------------------------------------------------------------------------  
Preemptible Kernel (Low-Latency Desktop),  
它允許在除臨界區之外的所有內核 code 中進行線程切換。  
  
臨界區也叫做 atomic context, 主要是:  
1. spinlock保護的區間  
2. irq-handler, 即中斷上下文  
  
除了這兩種區間的內核 code, 其他地方都允許線程切換,  
所以, 低優先級的線程, 即使在內核中且非自願的情況下（例如純 CPU 計算）也能被搶占,  
讓 CPU 去服務優先級更高的線程（例如交互性事件）。  
用代價如:  
1. throughput降低  
2. 線程切換更多(消耗 CPU)  

以換取更高的交互性,適合於Desktop及嵌入式系統。  
使用 1 個 CPU 執行結果為:  
```
sudo taskset --cpu-list 0 ./pthread
```
```
T1 start at 1718264582.000206109
T2 start at 1718264582.000206194
T1 stop at 1718264585.000206160. elapse: 3.000000051 seconds.
T2 stop at 1718264595.000204247. elapse: 12.999998053 seconds.
```
使用多個 CPU 執行結果為:  
```
sudo ./pthread
```
```
T1 start at 1718266480.000890981
T2 start at 1718266480.000890982
T1 stop at 1718266483.000891133. elapse: 3.000000152 seconds.
T2 stop at 1718266493.000888412. elapse: 12.999997430 seconds.
```
結果如預期, 多個 CPU 可直接視為平行處理  
1 個 CPU, 則 t1 sleep 後轉到 t2 sleep, 接著 t2 會執行空轉 2 秒, 此時 t1 醒來, 因為 t1 priority 較高, 所以切換到 t1, 此時 t1 結束(3 秒), 切換回 t2, 繼續空轉 4 秒, 再空轉 6 秒, 此時 t2 結束(13 秒)。  

------------------------------------------------------------------------------------------------  
Voluntary Kernel Preemption,  
”Voluntary Kernel Preemption”這種和”No Forced Preemption”很像,  
內核不會強制切換線程,  而是靠內核函數的開發者主動去放棄 CPU:  
在複雜的代碼中, 時不時的檢查rescheduling是否必要, 這些檢查點(”explicit preemption point”)就是 might_sleep() 函數。  
可以理解為在”No Forced Preemption”的基礎上增加了“不等待資源的主動放棄”:  
1. 從內核返回用戶態的時候, 和”No Forced Preemption”一樣  
2. 在內核模式下主動放棄CPU(等待資源), 和”No Forced Preemption”一樣(如 sleep)  
3. 在內核模式下主動放棄CPU(不等待資源, 如 might_sleep() 主動放棄 CPU)

使用 1 個 CPU 執行結果為:  
```
sudo taskset --cpu-list 0 ./pthread
```
```
T1 start at 1718267655.000163779
T2 start at 1718267655.000163870
T1 stop at 1718267668.000396444. elapse: 13.000232665 seconds.
T2 stop at 1718267668.000396717. elapse: 13.000232847 seconds.
```
使用多個 CPU 執行結果為:  
```
sudo ./pthread
```
```
T1 start at 1718268473.000469680
T2 start at 1718268473.000469723
T1 stop at 1718268476.000469832. elapse: 3.000000152 seconds.
T2 stop at 1718268486.000964083. elapse: 13.000494360 seconds.
```
結果如預期, 多個 CPU 可直接視為平行處理,  
1 個 CPU, 則 t1 sleep 後轉到 t2 sleep, 接著 t2 會執行空轉 6 秒, t2 繼續空轉 6 秒, 此時 t1, t2 結束(13 秒)。  

接下來在 driver 中加入 might_sleep()
```c
unsigned long end_time = jiffies + msecs_to_jiffies(6000); // 計算 6 秒後的結束時間
while (time_before(jiffies, end_time)) {
    mdelay(1); // 每次循環延遲 1 毫秒
}
might_sleep();
unsigned long end_time2 = jiffies + msecs_to_jiffies(6000); // 計算 6 秒後的結束時間
while (time_before(jiffies, end_time2)) {
    mdelay(1); // 每次循環延遲 1 毫秒
}
```
則 t1 & t2 行為變為  
t1: sleep 3 秒  
t2: sleep 1 秒, 空轉 6 秒, might_sleep(), 空轉 6 秒  
使用 1 個 CPU 執行結果為:  
```
sudo taskset --cpu-list 0 ./pthread
```
```
T1 start at 1718269595.000267425
T2 start at 1718269595.000267490
T1 stop at 1718269602.000267955. elapse: 7.000000530 seconds.
T2 stop at 1718269608.000268213. elapse: 13.000000723 seconds.
```
使用多個 CPU 執行結果為:  
```
sudo ./pthread
```
```
T2 start at 1718269682.000103079
T1 start at 1718269682.000103150
T1 stop at 1718269685.000103279. elapse: 3.000000129 seconds.
T2 stop at 1718269695.000100200. elapse: 12.999997121 seconds.
```
結果如預期, 多個 CPU 可直接視為平行處理,  
1 個 CPU, 則 t1 sleep 後轉到 t2 sleep, 接著 t2 會執行空轉 6 秒, 此時 t2 主動放棄 CPU(might_sleep()), 切換到 t1, 此時 t1 結束(7 秒), 接著再換回 t2 空轉 6 秒, 此時 t2 結束(13 秒)。  

------------------------------------------------------------------------------------------------  
