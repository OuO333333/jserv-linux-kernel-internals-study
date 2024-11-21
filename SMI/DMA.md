Direct Memory Access (DMA)  
  
原本由 CPU 負責對記憶體進行讀寫的工作,  
改為交給 DMA controller 進行。  
CPU 會先初始化任務,  
告訴 DMA controller 資料來源、目的跟資料量,  
接下來 DMA controller 就會進行這個記憶體任務。  
