#include "reader_writer_lock.h"

void rwlock_init(rwlock_t *rw) {
    rw->readers = 0;
    rw->lock = sem_open("rw_lock", O_CREAT, 777, 1);
    rw->writelock = sem_open("rw_writelock", O_CREAT, 777, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
    sem_wait(rw->lock);
    rw->readers++;
    if (rw->readers == 1)
        sem_wait(rw->writelock); // first reader acquires writelock
    sem_post(rw->lock);
}

void rwlock_release_readlock(rwlock_t *rw) {
    sem_wait(rw->lock);
    rw->readers--;
    if (rw->readers == 0)
        sem_post(rw->writelock); // last reader releases writelock
    sem_post(rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
    sem_wait(rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw) {
    sem_post(rw->writelock);
}
