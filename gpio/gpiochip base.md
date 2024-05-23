以 imx8mp 為例: https://github.com/torvalds/linux/blob/master/arch/arm64/boot/dts/freescale/imx8mp.dtsi  

------------------------------------------------------------------------------------------------  
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
同一個 gpiochip 中的多個 gpio pins 的 global gpio numbers, 可以用 base + offset 來表示,  
如上圖, gpiochip0 的 gpiochip base 為 0。  

------------------------------------------------------------------------------------------------  
怎麽設定 gpiochip base?  
gpiochip base 的設定分成兩種方法,  
第一種是 dynamic gpiochip base,  
第二種是 static gpiochip base  

------------------------------------------------------------------------------------------------  
先來看 dynamic gpiochip base,  
以 https://github.com/torvalds/linux/blob/master/drivers/gpio/gpio-pca953x.c 為例,
設定 dynamic gpiochip base 的地方在:  
```c
static int mxc_gpio_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct mxc_gpio_port *port;
	int irq_count;
	int irq_base;
	int err;

	port = devm_kzalloc(&pdev->dev, sizeof(*port), GFP_KERNEL);
	if (!port)
		return -ENOMEM;

	port->dev = &pdev->dev;
	port->hwdata = device_get_match_data(&pdev->dev);

	port->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(port->base))
		return PTR_ERR(port->base);

	irq_count = platform_irq_count(pdev);
	if (irq_count < 0)
		return irq_count;

	if (irq_count > 1) {
		port->irq_high = platform_get_irq(pdev, 1);
		if (port->irq_high < 0)
			port->irq_high = 0;
	}

	port->irq = platform_get_irq(pdev, 0);
	if (port->irq < 0)
		return port->irq;

	/* the controller clock is optional */
	port->clk = devm_clk_get_optional_enabled(&pdev->dev, NULL);
	if (IS_ERR(port->clk))
		return PTR_ERR(port->clk);

	if (of_device_is_compatible(np, "fsl,imx7d-gpio"))
		port->power_off = true;

	pm_runtime_get_noresume(&pdev->dev);
	pm_runtime_set_active(&pdev->dev);
	pm_runtime_enable(&pdev->dev);

	/* disable the interrupt and clear the status */
	writel(0, port->base + GPIO_IMR);
	writel(~0, port->base + GPIO_ISR);

	if (of_device_is_compatible(np, "fsl,imx21-gpio")) {
		/*
		 * Setup one handler for all GPIO interrupts. Actually setting
		 * the handler is needed only once, but doing it for every port
		 * is more robust and easier.
		 */
		port->irq_high = -1;
		port->mx_irq_handler = mx2_gpio_irq_handler;
	} else
		port->mx_irq_handler = mx3_gpio_irq_handler;

	mxc_update_irq_chained_handler(port, true);
	err = bgpio_init(&port->gc, &pdev->dev, 4,
			 port->base + GPIO_PSR,
			 port->base + GPIO_DR, NULL,
			 port->base + GPIO_GDIR, NULL,
			 BGPIOF_READ_OUTPUT_REG_SET);
	if (err)
		goto out_bgio;

	port->gc.request = mxc_gpio_request;
	port->gc.free = mxc_gpio_free;
	port->gc.to_irq = mxc_gpio_to_irq;
	port->gc.base = (pdev->id < 0) ? of_alias_get_id(np, "gpio") * 32 :
					     pdev->id * 32;

	err = devm_gpiochip_add_data(&pdev->dev, &port->gc, port);
	if (err)
		goto out_bgio;

	irq_base = devm_irq_alloc_descs(&pdev->dev, -1, 0, 32, numa_node_id());
	if (irq_base < 0) {
		err = irq_base;
		goto out_bgio;
	}

	port->domain = irq_domain_add_legacy(np, 32, irq_base, 0,
					     &irq_domain_simple_ops, NULL);
	if (!port->domain) {
		err = -ENODEV;
		goto out_bgio;
	}

	irq_domain_set_pm_device(port->domain, &pdev->dev);

	/* gpio-mxc can be a generic irq chip */
	err = mxc_gpio_init_gc(port, irq_base);
	if (err < 0)
		goto out_irqdomain_remove;

	list_add_tail(&port->node, &mxc_gpio_ports);

	platform_set_drvdata(pdev, port);
	pm_runtime_put_autosuspend(&pdev->dev);

	return 0;

out_irqdomain_remove:
	irq_domain_remove(port->domain);
out_bgio:
	pm_runtime_disable(&pdev->dev);
	pm_runtime_put_noidle(&pdev->dev);
	dev_info(&pdev->dev, "%s failed with errno %d\n", __func__, err);
	return err;
}
```
