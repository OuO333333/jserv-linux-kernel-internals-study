https://github.com/nxp-auto-linux/auto_yocto_bsp/tree/release/bsp35.0
環境:
Ubuntu 18.04 下 build auto_yocto_bsp bsp35.0
燒錄版為 s32g399ardb3
目標:
在使用 bitbake fsl-image-auto 編譯時加入新的 driver
並在燒錄時自動加載該 driver

1.
在 Yocto 項目根目錄下運行以下命令來創建新層
bitbake-layers create-layer meta-mydrivers

2.
將新層添加到 conf/bblayers.conf 中的 BBLAYERS 變量中，使其成為 Yocto 項目的有效層

3.
編寫驅動配方
在 meta-mydrivers 層中，為自定義驅動創建一個配方，例如在 recipes-kernel/mydriver/mydriver_1.0.bb

4.
編譯驅動並生成鏡像
bitbake <image>

5.
build 出來的 .ko 檔在
/Yocto 項目根目錄/build_s32g399ardb3/tmp/work/s32g399ardb3-fsl-linux/example/0.1-r0

6.
/Yocto 項目根目錄/sources/meta-alb/recipes-fsl/images/fsl-image-auto.bb
加入
IMAGE_INSTALL += "example"

