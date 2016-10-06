/*
 * In OS X, compile with
 * > gcc -o semaphores_as_condition_variables semaphores_as_condition_variables.c buffer.c
 */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"
#include <unistd.h>

#define NUM_THREADS 3

sem_t *empty = NULL;
sem_t *full = NULL;

void *producer(void *arg) {
    int i;
    int loops = 4;

    for (i = 1; i < 4; i++) {
        sem_wait(empty);
        put(i);
        sem_post(full);
        printf("put %d\n", i);
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int i, tmp = 0;

    while (tmp != -1) {
        sem_wait(full);
        tmp = get();
        sem_post(empty);
        printf("get: %d\n", tmp);
    }

    pthread_exit(NULL);
}

/*
 * buffer size = 10
 *
 * 產生 3 個 threads
 * 每個 thread 都對 buffer 塞三個數字 1, 2, 3
 * 預期 buffer 內會裝 3 個 1, 3 個 2, 3 個 3
 * 大部分情況的確如此沒錯，順序可能不同而已
 *
 * 但偶而會發生只有 2 個 1 的情況
 * 原因是
 * semaphore 在初始值設定大於 1 的時候
 * 就不再具備一般 lock 的作用
 * 有可能會發生 race condition ！
 *
 * producer 1 執行到 sem_wait
 * 把 semaphore 的初始值減一 10 - 1 = 9 >= 0
 * 不用等待，接著往下執行
 * 讀到 index 0 是空的
 * 準備往 index 0 設值前被 producer 2 preempt
 *
 * producer 2 執行到 sem_wait
 * 把 semaphore 的值減一 9 - 1 = 8 >= 0
 * 不用等待，接著往下執行
 * 發現 index 0 是空的，設值
 *
 * producer 1 恢復執行，對 index 0 設值
 * => producer 2 設的值被覆蓋！
 */
int main(int argc, char *argv[]) {
    pthread_t consumer_thread;
    pthread_t producer_threads[NUM_THREADS];
    pthread_t producer_thread;

    sem_init(empty, 0, BUFFER_SIZE);
    sem_init(full, 0, 0);
    /* empty = sem_open("empty", O_RDWR|O_CREAT, 00777, 10); */
    /* full = sem_open("full", O_RDWR|O_CREAT, 00777, 0); */

    /* pthread_create(&producer_thread, NULL, producer, NULL); */
    /* pthread_create(&consumer_thread, NULL, &consumer, NULL); */
    pthread_create(&producer_threads[0], NULL, producer, NULL);
    pthread_create(&producer_threads[1], NULL, &producer, NULL);
    pthread_create(&producer_threads[2], NULL, &producer, NULL);
    sleep(1);
    print_buffer();

    pthread_exit(NULL);
}
