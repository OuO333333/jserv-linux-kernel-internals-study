這邊會介紹 APL6012, 簡單介紹怎麽使用 APL6012(SW 視角),  
以及掛載上 B57703M(一個 NTC thermistors, 以型號 B57703M0103A019 為例) 後從 ADC 讀到的數值是怎麽轉換到溫度的。  
  
-------------------------------------------------------------  
  
ADC即數位類比轉換器,  
為一個將連續的類比訊號或者物理量(通常為電壓)轉換成數位訊號。    
  
-------------------------------------------------------------  
  
![image](https://github.com/OuO333333/driver/assets/37506309/f7bf3fdf-ae5a-4364-bdd5-8a4b8f85cf4d)  
![image](https://github.com/OuO333333/driver/assets/37506309/c7131a38-9024-4fcd-834c-aff07eb85b7c)
![image](https://github.com/OuO333333/driver/assets/37506309/f8097445-8ed5-4886-8096-3bd8774cf317)  
藉由兩個電阻 R1 & R2 來控制 i2c address, 可以看到 R1 跟 R2 是串聯的, 所以 Alert/ADDR 看到的電壓是 VDD * (R2 / (R1 + R2))。  
圖二可以看到 ADDR Input Voltage (% of VDD) 與 Address 的對照。
  
-------------------------------------------------------------  
  
![image](https://github.com/OuO333333/driver/assets/37506309/0b54b30c-5ada-4320-af2c-3a33c2aa5ccd)
![image](https://github.com/OuO333333/driver/assets/37506309/360dc56f-47a0-4087-aeb8-09fe7dceada5)
![image](https://github.com/OuO333333/driver/assets/37506309/f04dab99-62ff-46c2-b8fd-5d770613fbcf)
APL6012 總共有 20 個引腳,  
其中引腳 AIN1 - AIN15 可以外接電阻, 如外接 NTC thermistors,
這個外接電阻即是上圖中的 R<sub>NTCx</sub>, 我們還需要另一個電阻 R<sub>INx</sub>,  
令 R<sub>INx</sub> = 10kΩ,    
可以藉由這兩個電阻來控制輸入 ADC 的電壓 V<sub>AINx</sub> 在 1V - 3.56V 之間。  
可以看到 R<sub>NTCx</sub> 與 R<sub>Cx</sub> 並聯, 直流電中, 電容的電阻值通常被視為無限大,  
因此這兩個電阻的總電阻是 R<sub>NTCx</sub>, 又這這兩個電阻跟 R<sub>INx</sub> 串聯,  
令 VDD = 3.3V, 因此輸入 ADC 的電壓 V<sub>AINx</sub> = VDD * R<sub>NTCx</sub> / (R<sub>NTCx</sub> + R<sub>INx</sub>)。  
接下來來看 NTC thermistors 的 spec,  
![image](https://github.com/OuO333333/driver/assets/37506309/b7d88dcc-27d7-4be5-8c06-cf6fe233133b)  
![image](https://github.com/OuO333333/driver/assets/37506309/18738edc-a845-4db3-bb7a-fc7736bc164d)  
圖四可以看到, R<sub>T</sub> 與 R<sub>25</sub> 的比值(在各個溫度的電阻值與在 25&deg;C 的電阻值的比值),  
圖五可以看到 R<sub>25</sub> 為 10kΩ。  
假設最後 ADC 讀出來值為 x, 對應傳感器的溫度為 y&deg;C, 會有以下關係,  
1\. y 經查表得到 R<sub>y</sub>  
2\. V<sub>AINx</sub> = VDD * R<sub>y</sub> / (R<sub>y</sub> + R<sub>INx</sub>)  
3\. x = (V<sub>AINx</sub> - 1V) / 10 mV  
example:  
已知 R<sub>INx</sub> = 10kΩ, VDD = 3.3V, x = 101 時,  
解得 R<sub>y</sub> = 15581Ω, 經查表得 y 為 15&deg;C



  
-------------------------------------------------------------  

![image](https://github.com/OuO333333/driver/assets/37506309/e2c0bf2e-f41e-44cf-a5c9-1d076a5250c2)  
進入 ADC 的電壓 V<sub>AINx</sub> 的 range 為 1V - 3.56V 之間, 又 APL6012 有 10mV 的精確度,  
(3.56V - 1V) / 10mV  = 256,  
兩點(1V, 0), (3.56V, 255)解聯立方程式, 得圖一  
![image](https://github.com/OuO333333/driver/assets/37506309/91247f93-6a67-4b24-ace3-d69acc080b62)  
![image](https://github.com/OuO333333/driver/assets/37506309/e76b75da-c1c1-43ee-b3df-2b3ed9db5b62)  
將圖二式子帶入圖一式子, 可得圖三式子。  
圖一二三中的 VDD 跟 VIN 為相同的東西, 應統一以 VDD 表示較適當。  

