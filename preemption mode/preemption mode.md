Reference: https://www.yuanguohuo.com/2020/03/31/linux-preemption-mode/  
  
在 Ubuntu 22.04, 6.5.0-35-generic 上實驗不同的 preemption mode。  
Linux kernel 有多種 Preemption mode, 這邊我們介紹其中 3 種。  
1. No Forced Preemption (Server)  
2. Voluntary Kernel Preemption (Desktop)  
3. Preemptible Kernel (Low-Latency Desktop)
在 6.5.0-35-generic kernel 中, 已經可以動態的去改變 Preemption mode(早期的 linux kernel 需先編譯過才能改變)
先來查看 Preemption mode 的相關參數:
```
cat /boot/config-$(uname -r) | grep CONFIG_PREEMPT
```
