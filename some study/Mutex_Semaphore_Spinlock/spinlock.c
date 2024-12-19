#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

atomic_flag spinlock = ATOMIC_FLAG_INIT; // 初始化自旋鎖
int shared_counter = 0;

void lock_spinlock() {
    // 設為 true 並回傳先前的值
    while (atomic_flag_test_and_set(&spinlock)) {
        // 自旋，忙等待
    }
}

void unlock_spinlock() {
    // 設為 false
    atomic_flag_clear(&spinlock); // 釋放自旋鎖
}

void* increment_counter(void* arg) {
    for (int i = 0; i < 100000; i++) {
        lock_spinlock(); // 獲取自旋鎖
        shared_counter++;
        unlock_spinlock(); // 釋放自旋鎖
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment_counter, NULL);
    pthread_create(&t2, NULL, increment_counter, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter value: %d\n", shared_counter);
    return 0;
}

