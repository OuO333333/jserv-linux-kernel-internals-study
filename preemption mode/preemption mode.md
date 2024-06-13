Reference: https://www.yuanguohuo.com/2020/03/31/linux-preemption-mode/  
  
在 Ubuntu 22.04, 6.5.0-35-generic 上實驗不同的 preemption mode。  
Linux kernel 有多種 Preemption mode, 這邊我們介紹其中 3 種。  
1. No Forced Preemption (Server)  
2. Voluntary Kernel Preemption (Desktop)  
3. Preemptible Kernel (Low-Latency Desktop)
選擇”No Forced Preemption (Server)”產生的配置項是：
```c
CONFIG_PREEMPT_NONE=y
# CONFIG_PREEMPT_VOLUNTARY is not set
# CONFIG_PREEMPT is not set
```
選擇”Voluntary Kernel Preemption (Desktop)”產生的配置項是：
```c
# CONFIG_PREEMPT_NONE is not set
CONFIG_PREEMPT_VOLUNTARY=y
# CONFIG_PREEMPT is not set
```
選擇”Preemptible Kernel (Low-Latency Desktop)”產生的配置項是：
```c
CONFIG_PREEMPT_RCU=y
# CONFIG_PREEMPT_NONE is not set
# CONFIG_PREEMPT_VOLUNTARY is not set
CONFIG_PREEMPT=y
CONFIG_PREEMPT_COUNT=y
CONFIG_DEBUG_PREEMPT=y
# CONFIG_PREEMPT_TRACER is not set
```
