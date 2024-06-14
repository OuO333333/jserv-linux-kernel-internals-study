reference:
https://www.yuanguohuo.com/2020/03/31/linux-preemption-mode/  
分析 gpio_get_value_cansleep / gpio_get_value 的差別  
以 linux kernel 5.10.72 為例。

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

/* ... (other initialization code) ... */

#else
  static inline void ___might_sleep(const char *file, int line,
                                   int preempt_offset) { }
  static inline void __might_sleep(const char *file, int line,
                                   int preempt_offset) { }
# define might_sleep() do { might_resched(); } while (0)
```
```c
#ifdef CONFIG_DEBUG_ATOMIC_SLEEP
static inline int preempt_count_equals(int preempt_offset)
{       
        int nested = preempt_count() + rcu_preempt_depth();
        
        return (nested == preempt_offset);
}

void __might_sleep(const char *file, int line, int preempt_offset)
{
        /*
         * Blocking primitives will set (and therefore destroy) current->state,
         * since we will exit with TASK_RUNNING make sure we enter with it,
         * otherwise we will destroy state.
         */
        WARN_ONCE(current->state != TASK_RUNNING && current->task_state_change,
                        "do not call blocking ops when !TASK_RUNNING; "
                        "state=%lx set at [<%p>] %pS\n",
                        current->state,
                        (void *)current->task_state_change,
                        (void *)current->task_state_change);

        ___might_sleep(file, line, preempt_offset);
}
EXPORT_SYMBOL(__might_sleep);

void ___might_sleep(const char *file, int line, int preempt_offset)
{
        /* Ratelimiting timestamp: */
        static unsigned long prev_jiffy;

        unsigned long preempt_disable_ip;

        /* WARN_ON_ONCE() by default, no rate limit required: */
        rcu_sleep_check();

        if ((preempt_count_equals(preempt_offset) && !irqs_disabled() &&
             !is_idle_task(current) && !current->non_block_count) ||
            system_state == SYSTEM_BOOTING || system_state > SYSTEM_RUNNING ||
            oops_in_progress)
                return;

        if (time_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
                return;
        prev_jiffy = jiffies;

        /* Save this before calling printk(), since that will clobber it: */
        preempt_disable_ip = get_preempt_disable_ip(current);

        printk(KERN_ERR
                "BUG: sleeping function called from invalid context at %s:%d\n",
                        file, line);
        printk(KERN_ERR
                "in_atomic(): %d, irqs_disabled(): %d, non_block: %d, pid: %d, name: %s\n",
                        in_atomic(), irqs_disabled(), current->non_block_count,
                        current->pid, current->comm);

        if (task_stack_end_corrupted(current))
                printk(KERN_EMERG "Thread overran stack, or stack corrupted\n");

        debug_show_held_locks(current);
        if (irqs_disabled())
                print_irqtrace_events(current);
        if (IS_ENABLED(CONFIG_DEBUG_PREEMPT)
            && !preempt_count_equals(preempt_offset)) {
                pr_err("Preemption disabled at:");
                print_ip_sym(KERN_ERR, preempt_disable_ip);
        }

        trace_android_rvh_schedule_bug(NULL);

        dump_stack();
        add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_SYMBOL(___might_sleep);
```
```c
#ifdef CONFIG_PREEMPT_VOLUNTARY
extern int _cond_resched(void);
# define might_resched() _cond_resched()
#else
# define might_resched() do { } while (0)
#endif
```
可以觀察到,  
在沒有 CONFIG_DEBUG_ATOMIC_SLEEP 的情況下,  
might_sleep() 是  
```c
# define might_sleep() do { might_resched(); } while (0)
```
在沒有 CONFIG_PREEMPT_VOLUNTARY 的情況下,  
might_resched() 是  
```c
# define might_resched() do { } while (0)
```
也就是說, 在一般沒有開啟 CONFIG_DEBUG_ATOMIC_SLEEP 功能且是 Voluntary Kernel Preemption (Desktop) 的情況下,  
might_sleep() 是沒有實質作用的。  
在 Voluntary Kernel Preemption (Desktop) 的情況下(即 CONFIG_PREEMPT_VOLUNTARY 有 define), might_sleep() 會執行 might_resched(), 且 might_resched() 不為空。  
這個 might_resched() 就是在告訴系統我可以放棄 CPU 了, 要重新排程。  

------------------------------------------------------------------------------------------------  
```c
int gpiod_get_raw_value(const struct gpio_desc *desc)
{
        VALIDATE_DESC(desc);
        /* Should be using gpiod_get_raw_value_cansleep() */
        WARN_ON(desc->gdev->chip->can_sleep);
        return gpiod_get_raw_value_commit(desc);
}
EXPORT_SYMBOL_GPL(gpiod_get_raw_value);
```
```c
int gpiod_get_raw_value_cansleep(const struct gpio_desc *desc)
{
        might_sleep_if(extra_checks);
        VALIDATE_DESC(desc);
        return gpiod_get_raw_value_commit(desc);
}       
EXPORT_SYMBOL_GPL(gpiod_get_raw_value_cansleep);
```
gpiod_get_raw_value 跟 gpiod_get_raw_value_cansleep 為什麽要這樣設計呢(set 同理)?  
這是因為 i2c / spi 會需要 sleep, 而 sleep 會釋放 CPU 使用權,  
所以 get gpio value 不能在"不可中斷的上下文中"執行, 因此這時候就需要使用 gpiod_get_raw_value_cansleep,  
因為他會呼叫 might_sleep(), 當你開啟 CONFIG_DEBUG_ATOMIC_SLEEP 功能時, ___might_sleep() 就會檢查是否處在"不可中斷的上下文中"。
當你使用了 gpiod_get_raw_value, 因為是 i2c / spi, 他們的 desc->gdev->chip->can_sleep 是 1, 這時就會跟你警告, 說應該使用 gpiod_get_raw_value_cansleep。  
簡單來說, 透過 i2c / spi 來 get gpio value 時, 要使用 gpiod_get_raw_value_cansleep, 因為他會幫你防呆, 避免你在 "不可中斷的上下文中" 呼叫 get gpio value。
