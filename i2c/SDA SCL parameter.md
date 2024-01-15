![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/b4768418-957d-4bde-8bae-1087f879a441)
上圖為 nxp 的 I2C-bus specification and user manual 中的圖。  
基本上, 上升沿, 下降沿以高度 30% 與 70 % 區分, 高於 70% 算 up，低於 30% 算 low。  
接下來會逐個介紹這些參數。  
  
**fSCL**:&nbsp;&nbsp;&nbsp;&nbsp;SCL clock frequency,  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;&thinsp;SCL 下降沿高度 30% 到下一個 SCL 下降沿高度 30% 為一個 clock 

**tHIGH**:&nbsp;&nbsp;&nbsp;&nbsp;HIGH period of the SCL clock,  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;為 SCL 上升沿高度 70% 與 SCL 下降沿高度 70% 的時間差  

**tLOW**:&nbsp;&nbsp;&nbsp;&nbsp;LOW period of the SCL clock  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;&thinsp;為 SCL 下降沿高度 30% 與 SCL 上升沿高度 30% 的時間差  

**tr**:&nbsp;&nbsp;&nbsp;&nbsp;rise time of both SDA and SCL signals  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;為上升沿高度 30% & 70% 的時間差  

**tf**:&nbsp;&nbsp;&nbsp;&nbsp;fall time of both SDA and SCL signals  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;為下降沿高度 30% & 70% 的時間差  

**tHD;DAT**:&nbsp;&nbsp;&nbsp;&nbsp;data hold time  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;&thinsp;指的是在 SCL 的下降沿之後，SDA 需要保持穩定的時間。  

**tSU;DAT**:&nbsp;&nbsp;&nbsp;&nbsp;data set-up time  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;指的是在 SCL 的上升沿之前，SDA 必須保持穩定的時間。  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/fbfab57f-34cf-4c52-9aa0-639d0b9af1f8)  
這個要從 SDA 的 data 穩定時間看比較容易理解(見上圖)。  
SDA 的 data 穩定時間指的是, 指的是在 SDA 的上升沿(下降沿), 下降沿之間(上升沿), SDA 保持穩定的時間。  
為 SDA 上升沿高度 70% - SDA 下降沿高度 70%, 或 SDA 下降沿高度 30% - SDA 上升沿高度 30%。  
