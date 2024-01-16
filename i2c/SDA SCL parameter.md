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
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/fbfab57f-34cf-4c52-9aa0-639d0b9af1f8)  
**tHD;DAT**:&nbsp;&nbsp;&nbsp;&nbsp;data hold time  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;&thinsp;指的是在 SCL 的下降沿之後，SDA 需要保持穩定的時間。  

**tSU;DAT**:&nbsp;&nbsp;&nbsp;&nbsp;data set-up time  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;指的是在 SCL 的上升沿之前，SDA 必須保持穩定的時間。  
  
這兩個要從 SDA 的 data 穩定時間看比較容易理解(見上圖)。  
SDA 的 data 穩定時間指的是, 指的是在 SDA 的上升沿(下降沿), 下降沿之間(上升沿), SDA 保持穩定的時間。  
為 SDA 上升沿高度 70% - SDA 下降沿高度 70%, 或 SDA 下降沿高度 30% - SDA 上升沿高度 30%。  
在 SDA 穩定時間中,  
一開始 SCL 是 LOW 的, 要等一段時間後 SCL 才會拉 HIGH, 這段時間稱之為 **tSU;DAT**(data setup time),  
**tSU;DAT** 後會經過 SCL 的 **tr**,  
**tr** 後到 **tHIGH** 時才能讀取資料,  
**tHIGH** 後會經過 SCL 的 **tf**,  
**tf** 後到結束 data valid time 的時間稱之為 **tHD;DAT**(data hold time)。  
流程為:&nbsp;&nbsp;&nbsp;&nbsp;**tSU;DAT** -> **tr** -> **tLOW** -> **tf** -> **tHD;DAT**  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/f9f1da09-b268-457b-8ebc-991870952212)  
定義起使條件:  
當 SCL 和 SDA 都處於高電平時，主設備發送一個低電平脈沖到 SDA, 形成起始條件。  
  
**tSU;STA**:&nbsp;&nbsp;&nbsp;&nbsp;Setup Time for Start Condition  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;&thinsp;**tSU;STA** 表示發送起始條件前, SDA 必須穩定的時間,  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;&thinsp;為 SCL 上升沿 70%, 到 SDA 下降沿 70% 之前。  

**tHD;STA**:&nbsp;&nbsp;&nbsp;&nbsp;Hold Time for Start Condition  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;**tHD;STA** 表示在起始條件後, SDA 需要保持穩定的時間,  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;為 SDA 下降沿 30%, 到 SCL 下降沿 70% 之前。  

START 有三種發生情況  
1\. 第一個 START, 這邊的 START 不定義 **tSU;STA**  
2\. STOP & START 相接的 START, 這邊的 START 不定義 **tSU;STA**  
3\. 同種 operation 下(R/W), 為了切換不同 slave address 而發生的 START,  
這邊的 START 才有定義 **tSU;STA**  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/057c81bb-c57d-4397-b5ba-2aa07b0e2b3c)  

定義停止條件:  
當 SCL 處於高電平, SDA 處於低電平時，主設備發送一個高電平脈沖到 SDA, 形成停止條件。  

**tSU;STO**:&nbsp;&nbsp;&nbsp;&nbsp;Setup Time for Stop Condition  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**tSU;STO** 表示在發送停止條件之前，SDA 必須保持在一個可靠的電平上,  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;為 SCL 上升沿 70%, 到 SDA 上升沿 30%。  
  
**tBUF**:&nbsp;&nbsp;&nbsp;&nbsp;Bus Free Time  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;&thinsp;是指在總線上不存在通信活動的時間, 
即在兩個連續的傳輸周期之間的時間間隔。  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;&thinsp;**tBUF** 保證在啟動條件和停止條件之後, 總線能夠保持空閒, 以便進行下一次通信。  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&thinsp;&thinsp;為停止條件後, SDA 上升沿 70%, 到 SDA 下降沿 70% 的時間。
