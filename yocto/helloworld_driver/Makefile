obj-m := mydriver.o

# KERNEL_SRC := $(STAGING_KERNEL_DIR)

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) clean
