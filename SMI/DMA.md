Direct Memory Access (DMA)  
  
原本由 CPU 負責對記憶體進行讀寫的工作,  
改為交給 DMA controller 進行。  
CPU 會先初始化任務,  
告訴 DMA controller 資料來源、目的跟資料量,  
接下來 DMA controller 就會進行這個記憶體任務。  
  
快取一致性問題的  
1. CPU 寫入快取但未更新外部記憶體（Write-back Cache）  
   情境：  
   CPU更新了快取中的某塊數據，但由於快取使用的是「寫回（write-back）」策略，數據尚未寫入外部記憶體。  
   此時，DMA從外部記憶體讀取這些數據，卻獲取到了舊的數據，因為最新數據還留在快取中。  
   後果：  
   DMA 讀取的數據與 CPU 更新後的數據不一致，導致數據錯誤。  
