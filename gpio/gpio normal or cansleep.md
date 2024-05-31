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
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/46a50c36-207b-4f89-9ed3-8cc5d8cb6648)
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/5e5f6307-45e2-4d31-b054-51c0f2609348)
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/7f9147aa-f70a-469a-a285-2b9ce578389c)
