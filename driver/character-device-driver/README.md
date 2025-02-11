# What it does

## init operation:
- Implement static & dynamic methods to register device region.

## open/release operation:
- Based on *Linux Device Drivers, 3rd Edition*, restrict access to a single user at a time.

## R/W operation:
- Maintain a buffer and perform read/write operations on it.

## ioctl operation:
- Use ioctl operation for more customized behavior on devices.

## test/dev.c:
- Perform operations on the character device driver's buffer.

## test/check.c:
- Check if access restriction is set to 8, validate read/write operation, and test ioctl operation.

## test/pthread.c:
- Conduct experiments on *Ubuntu 22.04* with *kernel version 6.5.0-35-generic*.
- Examine `might_sleep();` behavior under three different *Preemption Modes*.

# How to use

## In driver:
1. `sudo make clean`
2. `sudo make`
3. `sudo insmod mychardev.ko`
4. `sed -n '/^Character/, /^$/ { /^$/ !p }' /proc/devices`
5. `sudo rmmod mychardev.ko`

## In test/check.c:
1. `sudo make`
2. `sudo make check`
3. `sudo tail -n50 /var/log/kern.log`

## In test/dev.c:
1. `sudo make`
2. `./dev ...`

## In test/pthread.c:
1. `sudo make`
2. `echo "voluntary" | sudo tee /sys/kernel/debug/sched/preempt`
3. `sudo ./pthread`
4. `sudo taskset --cpu-list 0 ./pthread`


Reference:  
    &nbsp;&nbsp;&nbsp;&nbsp;character device drivers:  
    &nbsp;&nbsp;&nbsp;&nbsp;https://linux-kernel-labs.github.io/refs/heads/master/labs/device_drivers.html  
    &nbsp;&nbsp;&nbsp;&nbsp;MKDEV(MAJOR, MINOR):  
    &nbsp;&nbsp;&nbsp;&nbsp;https://www.cnblogs.com/lilto/p/11878288.html  
    &nbsp;&nbsp;&nbsp;&nbsp;alloc_chrdev_region:  
    &nbsp;&nbsp;&nbsp;&nbsp;https://blog.csdn.net/kangear/article/details/13746091  
    &nbsp;&nbsp;&nbsp;&nbsp;gcc obj to exec:  
    &nbsp;&nbsp;&nbsp;&nbsp;https://stackoverflow.com/questions/1846913/how-to-make-an-executable-file-from-a-c-object-file  
    &nbsp;&nbsp;&nbsp;&nbsp;PHONY:  
    &nbsp;&nbsp;&nbsp;&nbsp;https://blog.csdn.net/lxq19980430/article/details/84800916  
    &nbsp;&nbsp;&nbsp;&nbsp;atomic example:  
    &nbsp;&nbsp;&nbsp;&nbsp;https://www.hitchhikersguidetolearning.com/2021/01/03/linux-kernel-atomic-operations-sample-code/  
    &nbsp;&nbsp;&nbsp;&nbsp;-Wimplicit-function-declaration:  
    &nbsp;&nbsp;&nbsp;&nbsp;https://hackmd.io/@unis/44455566  
    &nbsp;&nbsp;&nbsp;&nbsp;Blocking open & Non Blocking open  
    &nbsp;&nbsp;&nbsp;&nbsp;https://www.makelinux.net/ldd3/chp-6-sect-6.shtml
