以 linux v5.10.72 為例,  
以 imx8mp 為例: https://github.com/torvalds/linux/blob/master/arch/arm64/boot/dts/freescale/imx8mp.dtsi  

------------------------------------------------------------------------------------------------  
什麽是 gpiochip base?  
一個 gpio-controller 為一個 gpiochip,即我們常在 dts 跟 cat sys/kernel/debug/gpio 看到的  
```c
gpio1: gpio@30200000 {
				compatible = "fsl,imx8mp-gpio", "fsl,imx35-gpio";
				reg = <0x30200000 0x10000>;
				interrupts = <GIC_SPI 64 IRQ_TYPE_LEVEL_HIGH>,
					     <GIC_SPI 65 IRQ_TYPE_LEVEL_HIGH>;
				clocks = <&clk IMX8MP_CLK_GPIO1_ROOT>;
				gpio-controller;
				#gpio-cells = <2>;
				interrupt-controller;
				#interrupt-cells = <2>;
				gpio-ranges = <&iomuxc 0 5 30>;
			};
```
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/0bda6f34-0d9f-4751-b1f9-d10353baea65)  
一個 gpiochip 控制著多個 gpio pin,
每一個 gpio pin 有一個 global gpio number,  
同一個 gpiochip 中的多個 gpio pins 的 global gpio numbers, 可以用 base + offset 來表示,  
如上圖, gpiochip0 的 gpiochip base 為 0。  

------------------------------------------------------------------------------------------------  
怎麽設定 gpiochip base?  
gpiochip base 的設定分成兩種方法,  
第一種是 dynamic gpiochip base,  
第二種是 static gpiochip base  

------------------------------------------------------------------------------------------------  
dynamic gpiochip base 跟 static gpiochip base 是在同個地方進行設定,  
以 /drivers/gpio/gpio-pca953x.c, 為例, 他是 dynamic gpiochip base,  
其設定 dynamic gpiochip base 的地方在:  
```c
static int pca953x_probe(struct i2c_client *client,
			 const struct i2c_device_id *i2c_id)
{
	/* ... (other initialization code) ... */
	ret = devm_gpiochip_add_data(&client->dev, &chip->gpio_chip, chip);
	if (ret)
		goto err_exit;
        /* ... (rest of the probe function) ... */
}
```
接下來看 linux 是怎麽 dynamic 設定 gpiochip base 的:  
```c
#define gpiochip_add_data(gc, data) ({          \
                static struct lock_class_key lock_key;  \
                static struct lock_class_key request_key;         \
                gpiochip_add_data_with_key(gc, data, &lock_key, \
                                           &request_key);         \
        })
#define devm_gpiochip_add_data(dev, gc, data) ({ \
                static struct lock_class_key lock_key;  \
                static struct lock_class_key request_key;         \
                devm_gpiochip_add_data_with_key(dev, gc, data, &lock_key, \
                                           &request_key);         \
        })
#else
#define gpiochip_add_data(gc, data) gpiochip_add_data_with_key(gc, data, NULL, NULL)
#define devm_gpiochip_add_data(dev, gc, data) \
        devm_gpiochip_add_data_with_key(dev, gc, data, NULL, NULL)

```
```c
/**
 * devm_gpiochip_add_data_with_key() - Resource managed gpiochip_add_data_with_key()
 * @dev: pointer to the device that gpio_chip belongs to.
 * @gc: the GPIO chip to register
 * @data: driver-private data associated with this chip
 * @lock_key: lockdep class for IRQ lock
 * @request_key: lockdep class for IRQ request
 *
 * Context: potentially before irqs will work
 *
 * The gpio chip automatically be released when the device is unbound.
 *
 * Returns:
 * A negative errno if the chip can't be registered, such as because the
 * gc->base is invalid or already associated with a different chip.
 * Otherwise it returns zero as a success code.
 */
int devm_gpiochip_add_data_with_key(struct device *dev, struct gpio_chip *gc, void *data,
                                    struct lock_class_key *lock_key,
                                    struct lock_class_key *request_key)
{
        struct gpio_chip **ptr;
        int ret;

        ptr = devres_alloc(devm_gpio_chip_release, sizeof(*ptr),
                             GFP_KERNEL);
        if (!ptr)
                return -ENOMEM;

        ret = gpiochip_add_data_with_key(gc, data, lock_key, request_key);
        if (ret < 0) {
                devres_free(ptr);
                return ret;     
        }

        *ptr = gc;            
        devres_add(dev, ptr);

        return 0;
}
EXPORT_SYMBOL_GPL(devm_gpiochip_add_data_with_key);

```
```c
int gpiochip_add_data_with_key(struct gpio_chip *gc, void *data,
                               struct lock_class_key *lock_key,
                               struct lock_class_key *request_key)
{
	/* ... (other initialization code) ... */
        /*
         * TODO: this allocates a Linux GPIO number base in the global
         * GPIO numberspace for this chip. In the long run we want to
         * get *rid* of this numberspace and use only descriptors, but
         * it may be a pipe dream. It will not happen before we get rid
         * of the sysfs interface anyways.
         */
        if (base < 0) {
                base = gpiochip_find_base(gc->ngpio);
                if (base < 0) {
                        ret = base;
                        spin_unlock_irqrestore(&gpio_lock, flags);
                        goto err_free_label;
                }
                /*
                 * TODO: it should not be necessary to reflect the assigned
                 * base outside of the GPIO subsystem. Go over drivers and
                 * see if anyone makes use of this, else drop this and assign
                 * a poison instead.
                 */
                gc->base = base;
        }
        gdev->base = base;
        /* ... (rest of the probe function) ... */
}
```
```c
/* dynamic allocation of GPIOs, e.g. on a hotplugged device */
static int gpiochip_find_base(int ngpio)
{
        struct gpio_device *gdev;
        int base = ARCH_NR_GPIOS - ngpio;

        list_for_each_entry_reverse(gdev, &gpio_devices, list) {
                /* found a free space? */
                if (gdev->base + gdev->ngpio <= base)
                        break;
                else
                        /* nope, check the space right before the chip */
                        base = gdev->base - ngpio;
        }

        if (gpio_is_valid(base)) {
                pr_debug("%s: found new base at %d\n", __func__, base);
                return base;
        } else {
                pr_err("%s: cannot find free range\n", __func__);
                return -ENOSPC;
        }
}
```

不難看出, dynamic gpiochip base 在指定 gpiochip base = -1 時運作,  
運作方式如下:  


------------------------------------------------------------------------------------------------
怎麽將 dynamic gpiochip base 改成使用 static gpiochip base?  
在我使用的板子, 他會 probe 兩次(兩個 device map 到這個 driver, 執行兩次 pca953x_probe()),  
因此改成 static gpiochip base 後, 會出錯,  
第 1 次的 probe 會成功, 因為第 1 次指定 gpiochip base 時, 是沒被 occupy 的,  
第 2 次的 probe 會失敗, 因為第 2 次指定 gpiochip base 時, 還是指定相同的 gpiochip base, 而這時已經被第一次 occupy 了。  
解決的方法如下:
```c
static int count = 0;
static int pca953x_probe(struct i2c_client *client,
			 const struct i2c_device_id *i2c_id)
{
	/* ... (other initialization code) ... */
        count++;
        if(count == 1)
            chip->gpio_chip.base = 333;
        else
            chip->gpio_chip.base = 353;
	ret = devm_gpiochip_add_data(&client->dev, &chip->gpio_chip, chip);
	if (ret)
		goto err_exit;
        /* ... (rest of the probe function) ... */
}
```
結果如下:  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/13ad13e2-ec4f-49b2-8577-a5bdd1d13baf)  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/202ed52b-561c-4e2b-a92e-e747f58b943a)



------------------------------------------------------------------------------------------------
