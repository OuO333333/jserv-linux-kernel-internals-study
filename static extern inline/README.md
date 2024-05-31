static: 只在當前 file 可見  
extern: 在別處宣告該 function, 在別處宣告該 funtion 時需 include 宣告 extern 的檔案  
inline: 編譯器的優化, 將 function 內嵌  
  
內容說明:  
1. 用 config.h 來控制 func()
2. 若 IS_STATIC_INLINE 為 'y', 則在 my.h 中定義 func()
3. 若 IS_STATIC_INLINE 不為 'y', 則在 function.h 中定義 func()
