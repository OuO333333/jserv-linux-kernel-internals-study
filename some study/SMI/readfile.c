#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char filename[] = "CMD.txt"; // 要讀取的檔案名稱
    char buffer[256]; // 用來儲存每行讀取內容的緩衝區

    // 開啟檔案，只讀模式 ("r")
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("無法開啟檔案");
        return 1; // 回傳非零值表示錯誤
    }

    printf("檔案內容：\n");
    
    // 使用fgets讀取每一行，直到檔案結束
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    // 關閉檔案
    fclose(file);
    
    return 0;
}

