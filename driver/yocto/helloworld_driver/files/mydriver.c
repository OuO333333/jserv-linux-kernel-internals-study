#include <linux/kernel.h>
#include <linux/module.h>
#include "mydriver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Hello World driver");
MODULE_VERSION("1.0");

void mydriver_hello(void) {
    printk(KERN_INFO "Hello, world from mydriver!\n");
}

static int __init mydriver_init(void) {
    printk(KERN_INFO "MyDriver: Initializing the Hello World driver\n");
    mydriver_hello();
    return 0;
}

static void __exit mydriver_exit(void) {
    printk(KERN_INFO "MyDriver: Exiting the Hello World driver\n");
}

module_init(mydriver_init);
module_exit(mydriver_exit);
