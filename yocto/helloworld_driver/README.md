https://github.com/nxp-auto-linux/auto_yocto_bsp/tree/release/bsp35.0  
環境:  
Ubuntu 18.04 下 build auto_yocto_bsp bsp35.0   
  
目標:  
在使用 bitbake fsl-image-auto 編譯時加入新的 driver  
並在燒錄時自動加載該 driver  

1. 在 Yocto 項目根目錄下運行以下命令來創建新層
   ```bash
   bitbake-layers create-layer meta-mydrivers

2.
&nbsp;&nbsp;&nbsp;將新層添加到 conf/bblayers.conf 中的 BBLAYERS 變量中，使其成為 Yocto 項目的有效層  
在 /Yocto 項目根目錄/build_燒錄設備/conf 中,  
在 BBLAYERS ?= 中加入  
/Yocto 項目根目錄/build_燒錄設備/meta-mydrivers \

4.
&nbsp;&nbsp;&nbsp;編寫驅動配方
&nbsp;&nbsp;&nbsp;在 meta-mydrivers 層中，為自定義驅動創建一個配方，在 /meta-mydrivers/recipes-example/example/example_0.1.bb 中  

5.
&nbsp;&nbsp;&nbsp;編譯驅動並生成鏡像  
&nbsp;&nbsp;&nbsp;bitbake example

6.
&nbsp;&nbsp;&nbsp;build 出來的 .ko 檔在  
&nbsp;&nbsp;&nbsp;/Yocto 項目根目錄/build_燒錄設備/tmp/work/燒錄目標-fsl-linux/example/0.1-r0

7.
&nbsp;&nbsp;&nbsp;/Yocto 項目根目錄/sources/meta-alb/recipes-fsl/images/fsl-image-auto.bb  
&nbsp;&nbsp;&nbsp;加入  
&nbsp;&nbsp;&nbsp;IMAGE_INSTALL += "example"

