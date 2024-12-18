i#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore; // 定義信號量
int shared_counter = 0;

void* increment_counter(void* arg) {
    for (int i = 0; i < 100000; i++) {
        sem_wait(&semaphore); // 減少信號量（進入臨界區）
        shared_counter++;
        sem_post(&semaphore); // 增加信號量（離開臨界區）
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    sem_init(&semaphore, 0, 1); // 初始化信號量，初始值為 1, 表示可有 1 個 thread 進入 critical section 
    pthread_create(&t1, NULL, increment_counter, NULL);
    pthread_create(&t2, NULL, increment_counter, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter value: %d\n", shared_counter);
    sem_destroy(&semaphore); // 銷毀信號量
    return 0;
}

