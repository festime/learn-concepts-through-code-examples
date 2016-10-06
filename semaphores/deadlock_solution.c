/*
 * In OS X, compile with
 * > gcc -o deadlock_solution deadlock_solution.c buffer.c
 */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"
#include <unistd.h>

#define NUM_THREADS 3

sem_t *empty = NULL;
sem_t *full = NULL;
pthread_mutex_t mutex;

void *producer(void *arg) {
    int i;
    int loops = 4;

    for (i = -4; i <= -1; ++i) {
        sem_wait(empty);
        pthread_mutex_lock(&mutex);   // 取鎖改放在這裡，而不是 sem_wait 上面
        put(i);
        pthread_mutex_unlock(&mutex); // 釋鎖改放在這裡，而不是 sem_post 下面
        sem_post(full);
        printf("put %d\n", i);
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int i, tmp = 0;

    while (tmp != -1) {
        sem_wait(full);
        pthread_mutex_lock(&mutex);   // 取鎖改放在這裡，而不是 sem_wait 上面
        tmp = get();
        pthread_mutex_unlock(&mutex); // 釋鎖改放在這裡，而不是 sem_post 下面
        sem_post(empty);
        printf("get: %d\n", tmp);
    }

    pthread_exit(NULL);
}

/*
 * 改變 lock 和 unlock 的位置
 * 能解決 deadlock_example.c 裡的死結問題
 * 因為現在即使 consumer 進入 waiting 狀態
 * consumer 也沒有佔用鎖
 * 所以 producer 可以正常的對 buffer 放資料
 * 這樣 consumer 也能在輪到它執行時，正常的從 buffer 取資料
 */
int main(int argc, char *argv[]) {
    pthread_t consumer_thread;
    pthread_t producer_threads[NUM_THREADS];
    pthread_t producer_thread;

    pthread_mutex_init(&mutex, NULL);
    empty = sem_open("empty", O_CREAT, 777, 10);
    full = sem_open("full", O_CREAT, 777, 0);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    pthread_create(&producer_thread, NULL, producer, NULL);

    pthread_exit(NULL);
}

