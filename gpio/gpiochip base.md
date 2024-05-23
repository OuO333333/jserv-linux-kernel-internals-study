以 imx8mp 為例: https://github.com/torvalds/linux/blob/master/arch/arm64/boot/dts/freescale/imx8mp.dtsi  
  
什麽是 gpiochip base?  
一個 gpio-controller 為一個 gpiochip,即我們常在 dts 跟cat sys/kernel/debug/gpio 看到的  
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
同一個 gpiochip 中的多個 gpio pins 的 global gpio numbers, 可以用 base + offset 來表示
