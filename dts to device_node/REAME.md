reference:  
https://www.cnblogs.com/schips/p/linux_driver_dtb_to_device_node.html  
http://www.wowotech.net/memory_management/440.html  
https://www.cnblogs.com/zhangzhiwei122/p/16060453.html  
https://zhuanlan.zhihu.com/p/523477612  
http://www.wowotech.net/memory_management/fixmap.html
https://blog.csdn.net/yhb1047818384/article/details/108328097  
https://www.laumy.tech/798.html  
https://rhythm16.github.io/page_table/  


認識 dtb 怎麽轉換成 device_node 要先認識 fixmap。  
1. 為什麽需要 fixmap?  
在內核完全啟動之後, 內存管理可以提供各種豐富的 API 讓內核的其他模塊可以完成虛擬地址分配和建立地址映射的功能,  
在內核的啟動過程中, 有些模塊需要使用虛擬內存並 mapping 到指定的物理地址上, 而且, 這些模塊也沒有辦法等待完整的內存管理模塊初始化之後再進行地址映射。  
因此, linux kernel 固定分配了一些 fixmap 的虛擬地址, 這些地址有固定的用途, 使用該地址的模塊在初始化的時候, 講這些固定分配的地址mapping 到指定的物理地址上去。
2. fixmap 在哪?  
fixmap 的地址區域位於 FIXADDR_START 和 FIXADDR_TOP 之間
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/18c34bea-3dbc-458d-82a0-45b37812a675)
3. dtb image 的處理過程?  
（1）bootloader copy dtb image 到 memory 的某個位置上。  
具體的位置隨便, 當然還是要滿足8字節對齊, dtb image 不能越過 2M section size 的邊界的要求,  
畢竟我們也想一條section mapping就搞定dtb image。  
（2）bootloader 通過寄存器 x0 傳遞 dtb image 的物理地址, dtb image 的虛擬地址在編譯 kernel image 的時候就確定了。(寫死在 fixmap_remap_fdt 中)  
（3）匯編初始化階段不對 dtb image 做任何處理  
（4）在 start kernel 之後的初始化代碼中（具體在setup_arch--->setup_machine_fdt--->fixmap_remap_fdt--->create_mapping_noalloc中）, 創建 dtb image 的相關映射, 之後就可以自由的訪問 dtb image 了。

------------------------------------------------------------------------------------------------
在 head.s 完成部分初始化之後，就開始調用 C 語言函數，而被調用的第一個 C 語言函數就是 start_kernel。  
而對於設備樹的處理，基本上就在 setup_arch() 這個函數中。
```c
asmlinkage __visible void __init __no_sanitize_address start_kernel(void)
{
        char *command_line;
        // ...
        setup_arch(&command_line);
        // ...
}
```
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
fixmap_remap_fdt 透過 fixmap 將 physical address map 到 logical address。  

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
        // offset 為 dtb 在 physical address 中這個 2M block 中的起始位置
        offset = dt_phys % SWAPPER_BLOCK_SIZE;
        dt_virt = (void *)dt_virt_base + offset;

        /* map the first chunk so we can read the size from the header */
        // 這裡映射2M空間。
        create_mapping_noalloc(round_down(dt_phys, SWAPPER_BLOCK_SIZE),
                        dt_virt_base, SWAPPER_BLOCK_SIZE, prot);

        if (fdt_magic(dt_virt) != FDT_MAGIC)
                return NULL;

        *size = fdt_totalsize(dt_virt);
        if (*size > MAX_FDT_SIZE)
                return NULL;
        // 如果(dtb 文件起始地址 + size)超過上一個建立映射的地址範圍，就必須緊接著再映射 2M 空間。
        if (offset + *size > SWAPPER_BLOCK_SIZE)
                create_mapping_noalloc(round_down(dt_phys, SWAPPER_BLOCK_SIZE), dt_virt_base,
                               round_up(offset + *size, SWAPPER_BLOCK_SIZE), prot);

        return dt_virt;
}
```
從上述可以知道, logical address 會為了 fixmap 預留一塊空間。  
FIX_FDT_END = 1, 為 dtb 在 fixmap 中的起始 page number。  
FIX_FDT = 1024, 為 dtb 在 fixmap 中的最後一個 page number。  
PAGE_SIZE = 4096(bytes), 為 4K。  
__fix_to_virt(FIX_FDT) 會得到 fix 中的 dtb 在 logical address 中的起始位置。  
```c
// PAGE_SHIFT = 12, 與 PAGE_SIZE = 4096 同義
// FIXADDR_TOP 為 fixmap 結束的 logical address, 其必為 2M 對齊。
// FIXADDR_START 為 fixmap 起始的 logical address。
#define __fix_to_virt(x)	(FIXADDR_TOP - ((x) << PAGE_SHIFT))
#define __virt_to_fix(x)	((FIXADDR_TOP - ((x) & PAGE_MASK)) >> PAGE_SHIFT)
```
MAX_FDT_SIZE = 2M, 因為需要對齊 2M, 所以
```c
#define FIX_FDT_SIZE		(MAX_FDT_SIZE + SZ_2M)
```
會從 dtb 搬兩個 2M 對齊的到 fixmap,  
但 dtb 在 physical address 不是 2M 對齊的, 所以要取
```c
// SWAPPER_BLOCK_SIZE = 2M
offset = dt_phys % SWAPPER_BLOCK_SIZE;
dt_virt = (void *)dt_virt_base + offset;
```
------------------------------------------------------------------------------------------------
  
最後來看, 是怎麽建立映射的。
首先, early_fixmap_init 會先建立 Init_pg_dir, bm_pud, bm_pmd, bm_pte 4 個表間的查找關係
```c
void __init early_fixmap_init(void)
{
        pgd_t *pgdp; 
        p4d_t *p4dp, p4d;
        pud_t *pudp;
        pmd_t *pmdp;
        unsigned long addr = FIXADDR_START;

        // 獲取 Init_pg_dir 中 addr 對應的 entry, Init_pg_dir 的索引位為 addr[47:39]
        pgdp = pgd_offset_k(addr);

        p4dp = p4d_offset(pgdp, addr);
        p4d = READ_ONCE(*p4dp);
        if (CONFIG_PGTABLE_LEVELS > 3 &&
            !(p4d_none(p4d) || p4d_page_paddr(p4d) == __pa_symbol(bm_pud))) {
                /*
                 * We only end up here if the kernel mapping and the fixmap
                 * share the top level pgd entry, which should only happen on
                 * 16k/4 levels configurations.
                 */
                BUG_ON(!IS_ENABLED(CONFIG_ARM64_16K_PAGES));
                pudp = pud_offset_kimg(p4dp, addr);
        } else {
                if (p4d_none(p4d))
                        // 將 bm_pud 的物理地址寫進 Init_pg_dir 中
                        __p4d_populate(p4dp, __pa_symbol(bm_pud), PUD_TYPE_TABLE);
                pudp = fixmap_pud(addr);
        }
        if (pud_none(READ_ONCE(*pudp)))
                // 將 bm_pmd 的物理地址寫進 bm_pud 中
                __pud_populate(pudp, __pa_symbol(bm_pmd), PMD_TYPE_TABLE);
        pmdp = fixmap_pmd(addr);
        // 將 bm_pte 的物理地址寫進 bm_pmd 中
        __pmd_populate(pmdp, __pa_symbol(bm_pte), PMD_TYPE_TABLE);

        /*
         * The boot-ioremap range spans multiple pmds, for which
         * we are not prepared:
         */
        BUILD_BUG_ON((__fix_to_virt(FIX_BTMAP_BEGIN) >> PMD_SHIFT)
                     != (__fix_to_virt(FIX_BTMAP_END) >> PMD_SHIFT));

        if ((pmdp != fixmap_pmd(fix_to_virt(FIX_BTMAP_BEGIN)))
             || pmdp != fixmap_pmd(fix_to_virt(FIX_BTMAP_END))) {
                WARN_ON(1);
                pr_warn("pmdp %p != %p, %p\n",
                        pmdp, fixmap_pmd(fix_to_virt(FIX_BTMAP_BEGIN)),
                        fixmap_pmd(fix_to_virt(FIX_BTMAP_END)));
                pr_warn("fix_to_virt(FIX_BTMAP_BEGIN): %08lx\n",
                        fix_to_virt(FIX_BTMAP_BEGIN));
                pr_warn("fix_to_virt(FIX_BTMAP_END):   %08lx\n",
                        fix_to_virt(FIX_BTMAP_END));

                pr_warn("FIX_BTMAP_END:       %d\n", FIX_BTMAP_END);
                pr_warn("FIX_BTMAP_BEGIN:     %d\n", FIX_BTMAP_BEGIN);
        }
}
```
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/64010454-f169-4158-9748-bc476836052a)

```c
static void __init create_mapping_noalloc(phys_addr_t phys, unsigned long virt,
                                  phys_addr_t size, pgprot_t prot)
{
        if ((virt >= PAGE_END) && (virt < VMALLOC_START)) {
                pr_warn("BUG: not creating mapping for %pa at 0x%016lx - outside kernel range\n",
                        &phys, virt);
                return;
        }
        __create_pgd_mapping(init_mm.pgd, phys, virt, size, prot, NULL,
                             NO_CONT_MAPPINGS);
}
```
```c
// pgdir: 想要建立映射的頁表的根(虛擬位址)
// phys: 建立映射的物理位址起始位址
// virt: 建立映射的虛擬位址起始位址
// size: 映射的大小
// prot: 映射的屬性
// pgtable_alloc; 建立頁表過程中，使用的記憶體分配函式
// flags: 一些決定建立頁表過程行為的選項
static void __create_pgd_mapping(pgd_t *pgdir, phys_addr_t phys,
                                 unsigned long virt, phys_addr_t size,
                                 pgprot_t prot,
                                 phys_addr_t (*pgtable_alloc)(int),
                                 int flags)
{
        unsigned long addr, end, next;
        pgd_t *pgdp = pgd_offset_pgd(pgdir, virt);

        /*
         * If the virtual and physical address don't have the same offset
         * within a page, we cannot map the region as the caller expects.
         */
        if (WARN_ON((phys ^ virt) & ~PAGE_MASK))
                return;
        // 從 2M 對齊的 physical address, logical address 開始 map
        // 但是要 map 完整個 size, 所以 end 是(virt + size)
        phys &= PAGE_MASK;
        addr = virt & PAGE_MASK;
        end = PAGE_ALIGN(virt + size);

        do {
                // pgd_addr_end回傳(addr + 一個 pgd entry 的映射範圍)，或end，
                // 看哪個比較小
                next = pgd_addr_end(addr, end);
                // addr(這個iteration要建立映射的虛擬位址的開頭)
                // next(這個iteration要建立映射的虛擬位址結尾)
                // phys(這個iteration要建立映射的物理位址開頭)
                // prot, pgtable_alloc, flag傳入處理pud層級的函式
                // alloc_init_pud建立addr~next的映射
                alloc_init_pud(pgdp, addr, next, phys, prot, pgtable_alloc,
                               flags);
                // phys 往前推進 next - addr
                phys += next - addr;
        // 換下一個 pgd entry，更新 addr，如果addr == end則結束
        } while (pgdp++, addr = next, addr != end);
}
```
經實驗, create_mapping_noalloc 下的 __create_pgd_mapping:  
phys = 0x42400000,  
virt = 0xfffffffefe600000,  
end = 0xfffffffefe800000,  
size = 2097152,  

