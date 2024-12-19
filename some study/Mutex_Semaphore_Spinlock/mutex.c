#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex; // 初始化 Mutex
int shared_counter = 0;

void* increment_counter(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex); // 獲取 Mutex 鎖
        shared_counter++;
        pthread_mutex_unlock(&mutex); // 釋放 Mutex 鎖
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&mutex, NULL); // 初始化 Mutex
    pthread_create(&t1, NULL, increment_counter, NULL);
    pthread_create(&t2, NULL, increment_counter, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter value: %d\n", shared_counter);
    pthread_mutex_destroy(&mutex); // 銷毀 Mutex
    return 0;
}

