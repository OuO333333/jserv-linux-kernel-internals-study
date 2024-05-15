reference:  
https://www.cnblogs.com/schips/p/linux_driver_dtb_to_device_node.html  
http://www.wowotech.net/memory_management/440.html  
https://www.cnblogs.com/zhangzhiwei122/p/16060453.html  
https://zhuanlan.zhihu.com/p/523477612
  
為什麽需要 fixmap?  
在 start_kernel 之前，內核會建立 idmap_pg_dir 和 init_pg_dir 頁表，並使能 MMU，從而將系統切換到虛擬地址空間。  
這時只有內核鏡像部分的內存被映射，而且內存管理系統還沒有初始化，因此在內核的視角只有內核鏡像部分的內存可用。  
由於內存管理系統的重要性，接下去的當務之急自然是要盡快初始化該模塊。  
很不幸這時我們又遇到了經典的先有雞還是先有蛋問題，內存初始化前我們需要先從 dtb 中讀取系統的內存配置信息，  
而這需要先為 dtb 創建頁表，同時一些其它情形也需要執行內存的靜態或動態映射。  
但是頁表創建需要兩個條件：
（1）需要為內存分配虛擬地址
（2）需要為頁表本身分配內存
顯然，此時還不能調用內核的通用內存接口，本著哪里有困難哪里就有我的精神，fixmap作為救火隊長閃亮登場了。  
為了解決該問題，內核為其提供了以下機制：
為fixmap映射單獨預留一段虛擬地址空間。在當前內核版本下其在內核總虛擬地址空間中的位置如下：
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/dcc7eb4b-d025-49ea-9ed8-059c583768ca)



　　顯然，此時還不能調用內核的通用內存接口，本著哪里有困難哪里就有我的精神，fixmap作為救火隊長閃亮登場了。為了解決該問題，內核為其提供了以下機制：
（1）為fixmap映射單獨預留一段虛擬地址空間。在當前內核版本下其在內核總虛擬地址空間中的位置如下：
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
------------------------------------------------------------------------------------------------
  
```c
static void __init setup_machine_fdt(phys_addr_t dt_phys)
{
        int size;
        void *dt_virt = fixmap_remap_fdt(dt_phys, &size, PAGE_KERNEL);
        const char *name;

        if (dt_virt)
                memblock_reserve(dt_phys, size);
        // 內存地址檢查
        if (!dt_virt || !early_init_dt_scan(dt_virt)) {
                pr_crit("\n"
                        "Error: invalid device tree blob at physical address %pa (virtual address 0x%p)\n"
                        "The dtb must be 8-byte aligned and must not exceed 2 MB in size\n"
                        "\nPlease check your bootloader.",
                        &dt_phys, dt_virt);

                while (true)
                        cpu_relax();
        }

        /* Early fixups are done, map the FDT as read-only now */
        fixmap_remap_fdt(dt_phys, &size, PAGE_KERNEL_RO);

        name = of_flat_dt_get_machine_name();
        if (!name)
                return;

        pr_info("Machine model: %s\n", name);
        dump_stack_set_arch_desc("%s (DT)", name);
}
```
fixmap_remap_fdt 透過 fdt 將 physical address map 到 logical address。  

------------------------------------------------------------------------------------------------
```c
void *__init fixmap_remap_fdt(phys_addr_t dt_phys, int *size, pgprot_t prot)
{
        const u64 dt_virt_base = __fix_to_virt(FIX_FDT);
        int offset;
        void *dt_virt;

        /*
         * Check whether the physical FDT address is set and meets the minimum
         * alignment requirement. Since we are relying on MIN_FDT_ALIGN to be
         * at least 8 bytes so that we can always access the magic and size
         * fields of the FDT header after mapping the first chunk, double check
         * here if that is indeed the case.
         */
        BUILD_BUG_ON(MIN_FDT_ALIGN < 8);
        if (!dt_phys || dt_phys % MIN_FDT_ALIGN)
                return NULL;

        /*
         * Make sure that the FDT region can be mapped without the need to
         * allocate additional translation table pages, so that it is safe
         * to call create_mapping_noalloc() this early.
         *
         * On 64k pages, the FDT will be mapped using PTEs, so we need to
         * be in the same PMD as the rest of the fixmap.
         * On 4k pages, we'll use section mappings for the FDT so we only
         * have to be in the same PUD.
         */
        BUILD_BUG_ON(dt_virt_base % SZ_2M);

        BUILD_BUG_ON(__fix_to_virt(FIX_FDT_END) >> SWAPPER_TABLE_SHIFT !=
                     __fix_to_virt(FIX_BTMAP_BEGIN) >> SWAPPER_TABLE_SHIFT);

        offset = dt_phys % SWAPPER_BLOCK_SIZE;
        dt_virt = (void *)dt_virt_base + offset;

        /* map the first chunk so we can read the size from the header */
        create_mapping_noalloc(round_down(dt_phys, SWAPPER_BLOCK_SIZE),
                        dt_virt_base, SWAPPER_BLOCK_SIZE, prot);

        if (fdt_magic(dt_virt) != FDT_MAGIC)
                return NULL;

        *size = fdt_totalsize(dt_virt);
        if (*size > MAX_FDT_SIZE)
                return NULL;

        if (offset + *size > SWAPPER_BLOCK_SIZE)
                create_mapping_noalloc(round_down(dt_phys, SWAPPER_BLOCK_SIZE), dt_virt_base,
                               round_up(offset + *size, SWAPPER_BLOCK_SIZE), prot);

        return dt_virt;
}
```
MAX_FDT_SIZE = 2M, 因為需要對齊 2M, 所以
```c
#define FIX_FDT_SIZE		(MAX_FDT_SIZE + SZ_2M)
```
FIX_FDT_END = 1, 為 dtb 在 fix 中的起始 page。  
PAGE_SIZE = 4096, 為 4K。  
FIX_FDT = 1024, 為 dtb 在 FDT 中的最後一個 page。  
__fix_to_virt(FIX_FDT) 會得到 fix 中的 dtb 在 logical address 中的起始位置。
其中, 會從 dtb 搬兩個 2M 到 fix,  
但 dtb 在 physical address 不是 2M 對齊的, 所以要取
```c
offset = dt_phys % SWAPPER_BLOCK_SIZE;
dt_virt = (void *)dt_virt_base + offset;
```
