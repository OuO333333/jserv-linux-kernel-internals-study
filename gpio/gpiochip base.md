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
先來看 static gpiochip base,  
以 /drivers/gpio/gpio-pca953x.c, 為例,
設定 static gpiochip base 的地方在:  
```c
static int mxc_gpio_probe(struct platform_device *pdev)
{
	/* ... (other initialization code) ... */
        // 在這裡設定你要的 gpiochip base
        chip->base = -1;
        /* ... (rest of the probe function) ... */
        return pwmchip_add(chip);
}
```
接下來看 linux 是怎麽 static 設定 gpiochip base 的:  
```c
/**
 * pwmchip_add() - register a new PWM chip
 * @chip: the PWM chip to add
 *
 * Register a new PWM chip. If chip->base < 0 then a dynamically assigned base
 * will be used. The initial polarity for all channels is normal.
 *
 * Returns: 0 on success or a negative error code on failure.
 */
int pwmchip_add(struct pwm_chip *chip)
{
        return pwmchip_add_with_polarity(chip, PWM_POLARITY_NORMAL);
}
```
```c
/**
 * pwmchip_add_with_polarity() - register a new PWM chip
 * @chip: the PWM chip to add
 * @polarity: initial polarity of PWM channels
 *
 * Register a new PWM chip. If chip->base < 0 then a dynamically assigned base
 * will be used. The initial polarity for all channels is specified by the
 * @polarity parameter.
 *
 * Returns: 0 on success or a negative error code on failure.
 */
int pwmchip_add_with_polarity(struct pwm_chip *chip,
                              enum pwm_polarity polarity)
{
	/* ... (other initialization code) ... */
        // 即為 static 設定 gpiochip base 的地方
        ret = alloc_pwms(chip->base, chip->npwm);
        /* ... (rest of the probe function) ... */
}
```
```c
static int alloc_pwms(int pwm, unsigned int count)
{
	unsigned int from = 0;
	unsigned int start;

	if (pwm >= MAX_PWMS)
		return -EINVAL;

	if (pwm >= 0)
		from = pwm;
        // MAX_PWMS 為 allocated_pwms 長度
        // from 為 開始找的 index
        // count 為 連續找多少個還沒被 occupy 的 index
        // 根據 allocated_pwms, 找出從 from 開始第一個連續還沒被 occupy 的index
	start = bitmap_find_next_zero_area(allocated_pwms, MAX_PWMS, from,
					   count, 0);
        // 若找出的與 pwm(指定的) 不同, 則失敗
	if (pwm >= 0 && start != pwm)
		return -EEXIST;

	if (start + count > MAX_PWMS)
		return -ENOSPC;

	return start;
}
```
```c
/**
 * bitmap_find_next_zero_area - find a contiguous aligned zero area
 * @map: The address to base the search on
 * @size: The bitmap size in bits
 * @start: The bitnumber to start searching at
 * @nr: The number of zeroed bits we're looking for
 * @align_mask: Alignment mask for zero area
 *
 * The @align_mask should be one less than a power of 2; the effect is that
 * the bit offset of all zero areas this function finds is multiples of that
 * power of 2. A @align_mask of 0 means no alignment is required.
 */
static inline unsigned long
bitmap_find_next_zero_area(unsigned long *map,
                           unsigned long size,
                           unsigned long start,
                           unsigned int nr,
                           unsigned long align_mask)
{
        return bitmap_find_next_zero_area_off(map, size, start, nr,
                                              align_mask, 0);
}
```
```c
/**
 * bitmap_find_next_zero_area_off - find a contiguous aligned zero area
 * @map: The address to base the search on
 * @size: The bitmap size in bits
 * @start: The bitnumber to start searching at
 * @nr: The number of zeroed bits we're looking for
 * @align_mask: Alignment mask for zero area
 * @align_offset: Alignment offset for zero area.
 *
 * The @align_mask should be one less than a power of 2; the effect is that
 * the bit offset of all zero areas this function finds plus @align_offset
 * is multiple of that power of 2.
 */
unsigned long bitmap_find_next_zero_area_off(unsigned long *map,
                                             unsigned long size,
                                             unsigned long start,
                                             unsigned int nr,
                                             unsigned long align_mask,
                                             unsigned long align_offset)
{
        unsigned long index, end, i;
again:
        // 找第一個為 0 的 index
        index = find_next_zero_bit(map, size, start);
        /* Align allocation */
        index = __ALIGN_MASK(index + align_offset, align_mask) - align_offset;
        // end 為該次最多檢查到第幾個 index
        end = index + nr;
        if (end > size) 
                return end;
        // 在 index 與 end 間找第一個為 1 的索引
        i = find_next_bit(map, end, index);
        // 若 index 與 end 間有 1, 則下一次從 i + 1 開始找
        if (i < end) {
                start = i + 1; 
                goto again;
        }
        return index; 
}
EXPORT_SYMBOL(bitmap_find_next_zero_area_off);
```
不難看出, static gpiochip base 雖然是指定 gpio chip base, 但還是會檢查有沒有連續沒被 occupy 的 index 能用,  
在 alloc_pwms() 中, 若找到的連續沒被 occupy 的起點, 與指定的不同, 則會報錯。  

------------------------------------------------------------------------------------------------
在我使用的板子, 他會 probe 兩次(執行兩次 mxc_gpio_probe()),  
因此改成 static gpiochip base 後, 會出錯,  
第 1 次的 probe 會成功, 因為第 1 次指定 gpiochip base 時, 是沒被 occupy 的,  
第 2 次的 probe 會失敗, 因為第 2 次指定 gpiochip base 時, 還是指定相同的 gpiochip base, 而這時已經被第一次 occupy 了。

------------------------------------------------------------------------------------------------
