分析 gpio_get_value_cansleep / gpio_get_value 的差別  

------------------------------------------------------------------------------------------------  
首先是 gpio_get_value_cansleep  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/6442fa51-8c41-4b18-8b49-9a7d2bd769a3)
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/059c4e6d-b20c-4ef0-9122-e74f4c5aab66)
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/50785992-df06-4a18-9e51-cb748055e78b)

------------------------------------------------------------------------------------------------  
再來是 gpio_get_value
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/bb09b836-2aaf-48a0-9daf-e65e048afb38)
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/3e83e584-9e23-43f7-9bfd-f58377c4458f)
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/b9632615-4a32-4f22-b56c-3d32b3fae12b)

------------------------------------------------------------------------------------------------  
```c
#define might_sleep_if(cond) do { if (cond) might_sleep(); } while (0)
```
```c
#ifdef CONFIG_DEBUG_ATOMIC_SLEEP
extern void ___might_sleep(const char *file, int line, int preempt_offset);
extern void __might_sleep(const char *file, int line, int preempt_offset);
extern void __cant_sleep(const char *file, int line, int preempt_offset);

/**
 * might_sleep - annotation for functions that can sleep
 *
 * this macro will print a stack trace if it is executed in an atomic
 * context (spinlock, irq-handler, ...). Additional sections where blocking is
 * not allowed can be annotated with non_block_start() and non_block_end()
 * pairs.
 *
 * This is a useful debugging help to be able to catch problems early and not
 * be bitten later when the calling function happens to sleep when it is not
 * supposed to.
 */
# define might_sleep() \
        do { __might_sleep(__FILE__, __LINE__, 0); might_resched(); } while (0)
```
```c
#else
  static inline void ___might_sleep(const char *file, int line,
                                   int preempt_offset) { }
  static inline void __might_sleep(const char *file, int line,
                                   int preempt_offset) { }
# define might_sleep() do { might_resched(); } while (0)
```
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/46a50c36-207b-4f89-9ed3-8cc5d8cb6648)
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/5e5f6307-45e2-4d31-b054-51c0f2609348)
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/7f9147aa-f70a-469a-a285-2b9ce578389c)
