這邊會介紹 ADS7142, 簡單介紹怎麽使用 ADS7142(SW 視角),  
以及掛載上 B57703M(一個 NTC thermistors, 以型號 B57703M0103A019 為例) 後從 ADC 讀到的數值是怎麽轉換到溫度的。  
  
-------------------------------------------------------------  
  
ADC即數位類比轉換器,  
為一個將連續的類比訊號或者物理量(通常為電壓)轉換成數位訊號。    
  
-------------------------------------------------------------  
  
![image](https://github.com/user-attachments/assets/9ec88865-aaaa-4a2a-b1ae-3cdb92cf242a)
![image](https://github.com/user-attachments/assets/6a310a31-c6ba-4bfc-8b09-26b5efb1310a)
藉由兩個電阻 R1 & R2 來控制 I2C Address, 可以看到 R1 跟 R2 是串聯的, 所以 ADDR 看到的電壓是 AVDD * (R2 / (R1 + R2))。  
圖二可以看到 ADDR 與 I2C Address 的對照。
  
-------------------------------------------------------------  
  
![image](https://github.com/user-attachments/assets/9fb23f6f-e6fa-46d4-889d-ef388f502ea9)
![image](https://github.com/user-attachments/assets/e9ea1613-3b6c-49ff-aebb-17d06d8bfcec)
![image](https://github.com/user-attachments/assets/0210ceb8-dc57-4236-841c-1fc81e22451e)

ADS7142 總共有 10 個引腳,  
其中引腳 AIN0, AIN1 可以外接電阻, 如外接 NTC thermistors,
這個外接電阻即是上圖中的 NTC, 其電阻為 R<sub>NTC</sub>,  
我們還需要另一個電阻 R<sub>2</sub>,  
令 R<sub>2</sub> = 10kΩ,    
可以藉由這兩個電阻來控制輸入 ADC 的電壓 V<sub>in</sub> 在 0V - (AVDD)V 之間。  
可以看到 R<sub>NTC</sub> 與 C 並聯, 直流電中, 電容的電阻值通常被視為無限大,  
因此這兩個電阻的總電阻是 R<sub>NTC</sub>, 又這這兩個電阻跟 R<sub>2</sub> 串聯,  
AVDD = 3.3V, 因此輸入 ADC 的電壓 V<sub>in</sub> = AVDD * R<sub>NTC</sub> / (R<sub>NTC</sub> + R<sub>2</sub>)。  
接下來來看 NTC thermistors 的 spec,  
![image](https://github.com/OuO333333/driver/assets/37506309/b7d88dcc-27d7-4be5-8c06-cf6fe233133b)  
![image](https://github.com/OuO333333/driver/assets/37506309/18738edc-a845-4db3-bb7a-fc7736bc164d)  
圖四可以看到, R<sub>T</sub> 與 R<sub>25</sub> 的比值(在各個溫度的電阻值與在 25&deg;C 的電阻值的比值),  
圖五可以看到 R<sub>25</sub> 為 10kΩ。  
假設最後 ADC 讀出來值為 x, 對應傳感器的溫度為 y&deg;C, 會有以下關係,  
1\. y 經查表得到 R<sub>y</sub>  
2\. V<sub>in</sub> = AVDD * R<sub>y</sub> / (R<sub>y</sub> + R<sub>2</sub>)  
3\. ADC 線性轉換公式

對於給定的兩個點：

- \((1V, 0)\)
- \((AVDD, 4095)\)

假設 ADC 的轉換關系是線性方程：

y = 4095*(x - 1)/(AVDD - 1)

其中：
- \(x\) 是輸入電壓 V<sub>in</sub>
- \(y\) 是 ADC 的數值
- \(AVDD\) 是 ADC 的電壓

example:  
已知 R<sub>2</sub> = 10kΩ, AVDD = 3.3V, x = 1798 時,  
代入公式 3, 可得 Vin = 2.01,  
再帶入公式 2,  
解得 R<sub>y</sub> = 15581Ω, 經查表得 y 為 15&deg;C
