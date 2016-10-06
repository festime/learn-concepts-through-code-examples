/*
 * In OS X, compile with
 * > gcc -o starvation_example starvation_example.c reader_writer_lock.c
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "reader_writer_lock.h"

#define ARRAY_SIZE 1000000
#define NUM_READERS 4

/* static int array[ARRAY_SIZE] = { 0, 1, 2, 3, 4 }; */
/* static rwlock_t *reader_writer_lock; */
static rwlock_t reader_writer_lock;

void *reader(int *array) {
    int n = 0;

    rwlock_acquire_readlock(&reader_writer_lock);
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        n = array[i];
    }
    rwlock_release_readlock(&reader_writer_lock);
    printf("Finish read!\n");

    pthread_exit(NULL);
}

void *writer(int *array) {
    rwlock_acquire_writelock(&reader_writer_lock);
    for (int i = 0; i < 5; ++i) {
        array[i] = -i;
        printf("array[%d] = %d\n", i, -i);
    }
    rwlock_release_writelock(&reader_writer_lock);

    pthread_exit(NULL);
}

/*
 * 如果 writer 前面有很多 reader
 * 或是 writer 前的 reader 在做耗時久的讀取時
 * （例如上述 reader 讀一個 size = 5000_0000 的陣列裡所有元素）
 *
 * 因為現有 reader-writer lock 的設計
 * 要等所有 reader 全部讀取完畢時 writer 才能寫
 * 就可能造成 writer 飢餓的情形
 * 執行這隻程式可以發現 writer 最後才寫
 *
 * 另外發現一件有趣的事情
 * 因為我在陣列 array 宣告前面加上 static
 * 所以 array 的資料會存在靜態檔案裡面！
 * 會發現這個是因為編譯時間明顯有差距
 * 當 size = 5000_0000 比 size = 1000_1000 明顯慢了不少
 * 看檔案大小才發現， size = 5000_0000 時
 * 編譯出來的執行擋大小 200 MB ！
 *
 * 當把 array 寫成 main 裡的區域變數
 * 然後用傳指標的方式傳給 thread 當參數
 * 執行檔大小變成只有 9 KB ！
 *
 * 但是執行時發生 segmentation fault
 * 猜測可能是 array 大小的關係？
 * 就把 ARRAY_SIZE 從 1000_0000 改成 100_0000
 * 重新編譯執行，發現就正常執行了
 * 為什麼？
 * 因為超過一個 stack frame 的大小上限嗎？
 */
int main() {
    pthread_t reader_threads[NUM_READERS];
    pthread_t writer_thread;
    int array[ARRAY_SIZE] = { 0, 1, 2, 3, 4 };

    rwlock_init(&reader_writer_lock);
    for (int i = 0; i < 4; ++i) {
      pthread_create(&reader_threads[i], NULL, (void *) reader, array);
    }
    pthread_create(&writer_thread, NULL, (void *) writer, array);

    pthread_exit(NULL);
}
