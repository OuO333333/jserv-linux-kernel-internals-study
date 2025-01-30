#include <stdio.h>
#include <string.h>

void reverseWords(char *str) {
    char *start = str;  // 指向單字起始位置
    char *end = str;    // 指向單字結束位置
    char *wordArray[100]; // 用於儲存單字的指針陣列
    int wordCount = 0;  // 記錄單字數量

    // 分割單字
    while (*end != '\0') {
        if (*end == ' ') {         // 碰到空格，表示找到一個單字
            *end = '\0';           // 將空格替換成終止符，結束該單字
            wordArray[wordCount++] = start; // 儲存單字起始地址
            start = end + 1;       // 更新下一個單字的起始位置
        }
        end++;
    }
    wordArray[wordCount++] = start; // 最後一個單字

    // 反轉輸出單字
    for (int i = wordCount - 1; i >= 0; i--) {
        printf("%s", wordArray[i]);
        if (i > 0) {
            printf(" "); // 單字間加空格
        }
    }
    printf("\n"); // 換行
}

int main() {
    char input[100];
    printf("Enter a sentence: ");
    fgets(input, sizeof(input), stdin);

    // 移除 fgets 帶來的換行符號
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    reverseWords(input);

    return 0;
}

