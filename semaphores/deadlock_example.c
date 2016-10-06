/*
 * In OS X, compile with
 * > gcc -o deadlock_example deadlock_example.c buffer.c
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

/*
 * 對比 semaphores_as_condition_variables.c
 * 差別在執行 sem_wait 之前
 * 先嘗試取得鎖
 * 還有 sem_post(full) 後釋放鎖
 */
void *producer(void *arg) {
    int i;
    int loops = 4;

    for (i = 1; i < 4; i++) {
        pthread_mutex_lock(&mutex);   // new code
        sem_wait(empty);
        put(i);
        sem_post(full);
        pthread_mutex_unlock(&mutex); // new code
        printf("put %d\n", i);
    }

    pthread_exit(NULL);
}

/*
 * 對比 semaphores_as_condition_variables.c
 * 差別在執行 sem_wait 之前
 * 先嘗試取得鎖
 * 還有 sem_post(empty) 後釋放鎖
 */
void *consumer(void *arg) {
    int i, tmp = 0;

    while (tmp != -1) {
        pthread_mutex_lock(&mutex);   // new code
        sem_wait(full);
        tmp = get();
        sem_post(empty);
        pthread_mutex_unlock(&mutex); // new code
        printf("get: %d\n", tmp);
    }

    pthread_exit(NULL);
}

/*
 * buffer size = 10
 *
 * 假如 consumer 先執行
 * 取得 lock 後接著是 sem_wait(full)
 * 把 full 的初始值 0 減一後得到 -1 < 0
 * 所以 consumer 在此進入 waiting 狀態
 * 等待 producer 放東西進 buffer
 *
 * 接著輪到 producer 執行
 * 它嘗試取得 lock 但發現它正被其他 thread 持有
 * 所以 producer 在此進入 waiting 狀態
 * 等待某個 thread 釋放 lock
 *
 * consumer 等 producer 放資料進 buffer ，但 consumer 持有鎖
 * producer 等 consumer 釋放鎖，它才能放資料進 buffer
 * => deadlock
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
