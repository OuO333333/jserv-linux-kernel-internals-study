static: 只在當前 file 可見  
extern: 在別處宣告該 function, 在別處宣告該 funtion 時需 include 宣告 extern 的檔案  
inline: 編譯器的優化, 將 function 內嵌  
  
若 main.c 中沒有 define IS_STATIC_INLINE, 則編譯時不用加入 function.c,  
gcc main.c my.c 即可。  
若 main.c 中有 define IS_STATIC_INLINE, 則編譯時不用加入 my.c,  
gcc main.c function.c 即可。  
