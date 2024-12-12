```c
module_i2c_driver(my_driver);
```
```c
/**
 * module_i2c_driver() - Helper macro for registering a modular I2C driver
 * @__i2c_driver: i2c_driver struct
 *
 * Helper macro for I2C drivers which do not do anything special in module
 * init/exit. This eliminates a lot of boilerplate. Each module may only
 * use this macro once, and calling it replaces module_init() and module_exit()
 */
#define module_i2c_driver(__i2c_driver) \
        module_driver(__i2c_driver, i2c_add_driver, \
                        i2c_del_driver)
```
```c
/**
 * module_driver() - Helper macro for drivers that don't do anything
 * special in module init/exit. This eliminates a lot of boilerplate.
 * Each module may only use this macro once, and calling it replaces
 * module_init() and module_exit().
 *
 * @__driver: driver name
 * @__register: register function for this driver type
 * @__unregister: unregister function for this driver type
 * @...: Additional arguments to be passed to __register and __unregister.
 *
 * Use this macro to construct bus specific macros for registering
 * drivers, and do not use it on its own.
 */
#define module_driver(__driver, __register, __unregister, ...) \
static int __init __driver##_init(void) \
{ \
        return __register(&(__driver) , ##__VA_ARGS__); \
} \
module_init(__driver##_init); \
static void __exit __driver##_exit(void) \
{ \
        __unregister(&(__driver) , ##__VA_ARGS__); \
} \
module_exit(__driver##_exit);
```
,  
```c
module_i2c_driver(my_driver);
```
會先展開成
```c
module_driver(my_driver, i2c_add_driver, i2c_del_driver);
```
再展開成
```c
static int __init my_driver_init(void)
{
    return i2c_add_driver(&my_driver);
}
module_init(my_driver_init);

static void __exit my_driver_exit(void)
{
    i2c_del_driver(&my_driver);
}
module_exit(my_driver_exit);
```
在C語言中, ## 是一個預處理操作符, 用於連接兩個符號（token）。  
在宏定義中, ## 允許將參數和前綴或後綴連接在一起, 形成一個新的標識符, 這對於在宏定義中創建唯一的標識符是很有用的。  
如:  
&nbsp;&nbsp;&nbsp;&nbsp;__driver##_init 會被展開為 my_driver_init。  
&nbsp;&nbsp;&nbsp;&nbsp;__register 會被展開為 i2c_add_driver。  
&nbsp;&nbsp;&nbsp;&nbsp;__unregister 會被展開為 i2c_del_driver。  
可以發現展開完跟我們最早接觸的 hello world driver 有點像了:
```c
/* hello.c */
#include <linux/init.h>
#include <linux/module.h>

MODULE_DESCRIPTION("Hello_world");
MODULE_LICENSE("GPL");

static int hello_init(void)
{
    printk(KERN_INFO "Hello world !\n");
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_INFO "ByeBye !\n");
}

module_init(hello_init);

module_exit(hello_exit);
```
module_i2c_driver 也是基於 module_init 跟 module_exit 的。  
其他類型的 driver 也應該是如此。  
  
-------------------------------------------------------------  

```c
static struct i2c_driver my_driver = {
	.driver = {
		.name = "my",
		.of_match_table = my_of_match,
	},
	.probe = my_probe,
	.remove = my_remove,
	// .id_table = my_id,
};
```
of_match_table 是用於 device tree 與 driver 的匹配, i2c 的 i2c_device_match 會優先選擇 device tree 做匹配。  
遠古時期的 linux kernel 似乎有沒 assign id_table 就無法執行 probe 的問題:  
https://www.cnblogs.com/xiaojiang1025/p/6501956.html  
但這問題 linux kernel 在好幾年前就解決了, 現在的 linux kernel 在做 device tree 的 match 時不用再 assign id_table。  
  
-------------------------------------------------------------  
  
MODULE_DEVICE_TABLE 一般用於動態加載驅動也就是熱插拔的時候使用。  
當 module 被載入時，內核會查詢 MODULE_DEVICE_TABLE 代入的 device table，以確定模組所支援的硬體。
  
-------------------------------------------------------------  
  
當 i2c 子系統偵測到對應的硬體時(這邊為偵測到 device tree 的), 會調用 probe 函數,  
而 probe 函數的參數 struct i2c_client *client 和 const struct i2c_device_id *id 是由 i2C 子系統傳遞給 probe 函數的,  
這些參數的值取決於硬體的配置，並在 i2C 子系統掃描硬體並與設備描述相匹配時被設置。  
  
-------------------------------------------------------------  

```c
struct iio_dev *iio;
struct my *adc;
iio = devm_iio_device_alloc(&client->dev, sizeof(*adc));
adc = iio_priv(iio);
```
```c
/**
 * iio_device_alloc() - allocate an iio_dev from a driver
 * @parent:             Parent device.
 * @sizeof_priv:        Space to allocate for private structure.
 **/
struct iio_dev *iio_device_alloc(struct device *parent, int sizeof_priv)
{
        struct iio_dev_opaque *iio_dev_opaque;
        struct iio_dev *dev;
        size_t alloc_size;

        alloc_size = sizeof(struct iio_dev_opaque);
        if (sizeof_priv) { 
                alloc_size = ALIGN(alloc_size, IIO_ALIGN);
                alloc_size += sizeof_priv;
        }

        iio_dev_opaque = kzalloc(alloc_size, GFP_KERNEL);
        if (!iio_dev_opaque)
                return NULL;

        dev = &iio_dev_opaque->indio_dev;
        dev->priv = (char *)iio_dev_opaque +
                ALIGN(sizeof(struct iio_dev_opaque), IIO_ALIGN);

        dev->dev.parent = parent;
        dev->dev.groups = dev->groups;
        dev->dev.type = &iio_device_type;
        dev->dev.bus = &iio_bus_type;
        device_initialize(&dev->dev);
        dev_set_drvdata(&dev->dev, (void *)dev);
        mutex_init(&dev->mlock);
        mutex_init(&dev->info_exist_lock);
        INIT_LIST_HEAD(&iio_dev_opaque->channel_attr_list);

        dev->id = ida_simple_get(&iio_ida, 0, 0, GFP_KERNEL);
        if (dev->id < 0) {
                /* cannot use a dev_err as the name isn't available */
                pr_err("failed to get device id\n");
                kfree(iio_dev_opaque);
                return NULL;
        }
        dev_set_name(&dev->dev, "iio:device%d", dev->id);
        INIT_LIST_HEAD(&iio_dev_opaque->buffer_list);

        return dev;
}
EXPORT_SYMBOL(iio_device_alloc);
```
簡單 trace 過 devm_iio_device_alloc & iio_device_alloc,  
基本上就是 allocate 一塊大小為 sizeof(struct iio_dev_opaque) + sizeof_priv 的記憶體給 iio_dev_opaque, 並清為0,  
struct iio_dev_opaque 包含了 iio_dev, 將指標 struct iio_dev *dev 指向 iio_dev_opaque 的 iio_dev(indio_dev),  
接下來計算 dev -> priv = (char *)iio_dev_opaque + ALIGN(sizeof(struct iio_dev_opaque), IIO_ALIGN),  
此為儲存 driver private data 的起始記憶體位置,  
最後開始設定 struct iio_dev *dev 中的 struct device dev, 如 dev->dev.parent = parent, 將 device 指向一開始輸入進來的參數,  
簡單來說是把 i2c_client 中的 device 搬到 iio_dev 中的 device, 並設定 driver private data 讀起始位置, 所有才會有 adc = iio_priv(iio); 這行。  
  
-------------------------------------------------------------  

```c
i2c_set_clientdata(client, iio);
```
```c
static inline void i2c_set_clientdata(struct i2c_client *client, void *data)
{               
        dev_set_drvdata(&client->dev, data);
}
```
```c
static inline void dev_set_drvdata(struct device *dev, void *data)
{               
        dev->driver_data = data;
}
```
將 iio_dev assign 給 i2c_client, 使得 i2c_client 能存取 iio_dev。  
  
-------------------------------------------------------------  
  
```c
iio->name = dev_name(&client->dev);
iio->modes = INDIO_DIRECT_MODE;
iio->info = &my_info;
iio->num_channels = ARRAY_SIZE(my_chan_array);
iio->channels = my_chan_array;

adc->i2c = client;
```
設定 namse, mode, info(iio_info), num_channels, channels, 其中設定 iio_info 會 assign read_raw。  
  
-------------------------------------------------------------  

```c
err = iio_device_register(iio);
if (err < 0){
    dev_err(&client->dev, "my: Couldn't register the device.\n");
    goto error_device_register;
}
dev_err(&client->dev, "my: Probe sucessful!\n");
return 0;

error_device_register:
    mutex_destroy(&adc->lock);
    mutex_destroy(&adc->data_lock);
    dev_err(&client->dev, "my: Probe fail\n");
    return err;
```
處理 iio_dev 註冊失敗。  
  
-------------------------------------------------------------  

```c
static int my_remove(struct i2c_client *client)
{
	struct iio_dev *iio = i2c_get_clientdata(client);
	struct my *adc = iio_priv(iio);

	iio_device_unregister(iio);
	mutex_destroy(&adc->lock);
	mutex_destroy(&adc->data_lock);

	return 0;
}
```
處理 driver remove。  
  
-------------------------------------------------------------  


