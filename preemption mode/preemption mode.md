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

------------------------------------------------------------------------------------------------ 
No Forced Preemption (Server) 下只有 2 種情況會進行線程切換:  
1. 從內核態返回用戶態的時候
2. 在內核模式下主動放棄CPU（等待資源）(sleep 算這種  

使用 1 個 cpu 執行結果為:  
```
sudo taskset --cpu-list 0 ./pthread
```
```
T1 start at 1718264162.000305293
T2 start at 1718264162.000305403
T1 stop at 1718264175.000304070. elapse: 12.999998777 seconds.
T2 stop at 1718264175.000304133. elapse: 12.999998730 seconds.
```
使用多個 cpu 執行結果為:  
```
sudo ./pthread
```
```
T1 start at 1718264246.000711656
T2 start at 1718264246.000711657
T1 stop at 1718264249.000711755. elapse: 3.000000099 seconds.
T2 stop at 1718264259.000979032. elapse: 13.000267375 seconds.
```
