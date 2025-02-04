#include <stdio.h>

#define N 9

// 檢查數字 num 是否可以放在 board[row][col]
int isValid(int board[N][N], int row, int col, int num) {
    for (int i = 0; i < N; i++) {
        if (board[row][i] == num || board[i][col] == num)
            return 0;
    }

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i + startRow][j + startCol] == num)
                return 0;

    return 1;
}

// 回溯求解數獨
int solveSudoku(int board[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (board[row][col] == 0) {  // 找到一個空位
                for (int num = 1; num <= 9; num++) {  // 嘗試填入 1~9
                    if (isValid(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board))
                            return 1;
                        board[row][col] = 0;  // 回溯
                    }
                }
                return 0;  // 若無法填入任何數字，回溯
            }
        }
    }
    return 1;  // 全部填完，成功解出
}

// 印出數獨棋盤
void printBoard(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

// 測試
int main() {
    int board[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    printf("Original Sudoku:\n");
    printBoard(board);

    if (solveSudoku(board)) {
        printf("\nSolved Sudoku:\n");
        printBoard(board);
    } else {
        printf("\nNo solution exists\n");
    }

    return 0;
}
