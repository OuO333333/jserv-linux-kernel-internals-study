目標:  
在使用 bitbake 編譯時加入新的 driver  
並在燒錄時自動加載該 driver  

1. 在 Yocto 項目根目錄下運行以下命令來創建新層
   ```bash
   bitbake-layers create-layer meta-mydrivers
  
2. 將新層添加到 `conf/bblayers.conf` 中的 `BBLAYERS` 變量中，使其成為 Yocto 項目的有效層：

   在 `/Yocto 項目根目錄/build_燒錄設備/conf` 文件夾中，找到 `conf/bblayers.conf` 文件，並在以下位置添加新層路徑：

   ```bash
   BBLAYERS ?= " \
       /Yocto 項目根目錄/build_燒錄設備/meta-mydrivers \
       ..."
  
4. 編寫驅動配方  
   在 `meta-mydrivers` 層中，為自定義驅動創建一個配方，文件路徑為：  
   `/meta-mydrivers/recipes-example/example/example_0.1.bb`

5. 編譯驅動並生成鏡像  
   使用以下命令編譯驅動：  

   ```bash
   bitbake example
  
6. 編譯生成的 `.ko` 文件路徑  
   編譯完成後，生成的 `.ko` 文件位於：  
   `/Yocto 項目根目錄/build_燒錄設備/tmp/work/燒錄目標-fsl-linux/example/0.1-r0`


7. 修改镜像配方  
   在以下路径找到镜像配方：  
   `/Yocto 項目根目錄/sources/meta-alb/recipes-fsl/images/fsl-image-auto.bb`  
   在配方中加入以下内容：  

   ```bash
   IMAGE_INSTALL += "example"


