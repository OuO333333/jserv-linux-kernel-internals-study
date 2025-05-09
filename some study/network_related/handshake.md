# 3-Way Handshake 和 4-Way Handshake

## **3-Way Handshake（三次握手）**
三次握手是用於建立 TCP 連接的過程，確保雙方可以正常通信。  

### **步驟：**
1. **SYN（同步）：**  
   客戶端向伺服器發送一個帶有 SYN 標誌的數據包，請求建立連接，並發送初始序列號（Seq = x）。  

2. **SYN-ACK（同步確認）：**  
   伺服器收到 SYN 後，回應一個帶有 SYN 和 ACK 標誌的數據包，確認收到客戶端的 SYN，並回送自己的初始序列號（Seq = y, Ack = x+1）。  

3. **ACK（確認）：**  
   客戶端收到 SYN-ACK 後，發送一個帶有 ACK 標誌的數據包，確認伺服器的序列號（Ack = y+1）。  
   完成握手，建立連接。

---

### **圖示：**
Client -------SYN-------> Server, Seq = x  
Client <---SYN-ACK---- Server, Seq = y, Ack = x+1  
Client -------ACK-------> Server, Ack = y+1  


---

## **4-Way Handshake（四次握手）**
四次握手是用於釋放 TCP 連接的過程，確保雙方的數據傳輸已經結束。

### **步驟：**
1. **FIN（終止）：**  
   客戶端發送一個帶有 FIN 標誌的數據包，請求關閉連接。  

2. **ACK（確認）：**  
   伺服器收到 FIN 後，回應一個帶有 ACK 標誌的數據包，表示收到關閉請求，並進入半關閉狀態。  

3. **FIN（終止）：**  
   伺服器再發送一個帶有 FIN 標誌的數據包，請求關閉連接。  

4. **ACK（確認）：**  
   客戶端收到 FIN 後，回應一個帶有 ACK 標誌的數據包，確認關閉連接。  
   完成握手，釋放連接。

---

### **圖示：**
**Client 發出關閉請求**  
Client -------FIN-------> Server  
**Server 確認請求**  
Client <-------ACK------- Server  
**Server 發出關閉請求**  
Client <-------FIN------- Server  
**Client 確認請求**  
Client -------ACK-------> Server

---

### **總結**
- **3-Way Handshake** 用於 **建立 TCP 連接**，確保雙方準備就緒並且可以可靠通信。  
- **4-Way Handshake** 用於 **釋放 TCP 連接**，確保雙方的數據傳輸完成且連接可以安全終止。
