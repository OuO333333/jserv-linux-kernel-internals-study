![Screenshot from 2024-05-13 15-15-31](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/935030ae-a2e0-4b96-b6af-655c7b42dc72)  
在 head.s 完成部分初始化之後，就開始調用 C 語言函數，而被調用的第一個 C 語言函數就是 start_kernel。  
而對於設備樹的處理，基本上就在 setup_arch() 這個函數中。  

------------------------------------------------------------------------------------------------

```c
void __init __no_sanitize_address setup_arch(char **cmdline_p)
{    
    // 根据传入的设备树dtb的首地址完成一些初始化操作
    setup_machine_fdt(__fdt_pointer);
    
    // ...
    
    // 保证设备树dtb本身存在于内存中而不被覆盖
    arm64_memblock_init();
    
    // ...
    // 对设备树具体的解析
    /* Parse the ACPI tables for possible boot-time configuration */
    acpi_boot_table_init();
    if (acpi_disabled)
        unflatten_device_tree();
}
```
這三個被調用的函數就是主要的設備樹處理函數：  
1. setup_machine_fdt()：根據傳入的設備樹dtb的首地址完成一些初始化操作。
2. arm_memblock_init()：主要是內存相關函數，為設備樹保留相應的內存空間，保證設備樹dtb本身存在於內存中而不被覆蓋。用戶可以在設備樹中設置保留內存，這一部分同時作了保留指定內存的工作。
3. unflatten_device_tree()：對設備樹具體的解析，事實上在這個函數中所做的工作就是將設備樹各節點轉換成相應的struct device_node結構體。
下面我們再來通過代碼跟蹤仔細分析。
