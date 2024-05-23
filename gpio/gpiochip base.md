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
先來看 dynamic gpiochip base,  
以 /drivers/gpio/gpio-pca953x.c, 為例,
設定 dynamic gpiochip base 的地方在:  
```c
static int mxc_gpio_probe(struct platform_device *pdev)
{
	/* ... (other initialization code) ... */
        chip->base = -1;
        /* ... (rest of the probe function) ... */
        return pwmchip_add(chip);
}
```
接下來看 linux 是怎麽 dynamic 設定 gpiochip base 的:  
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
        // 即為 dynamic 設定 gpiochip base 的地方
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

	start = bitmap_find_next_zero_area(allocated_pwms, MAX_PWMS, from,
					   count, 0);

	if (pwm >= 0 && start != pwm)
		return -EEXIST;

	if (start + count > MAX_PWMS)
		return -ENOSPC;

	return start;
}
```
