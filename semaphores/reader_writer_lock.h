#ifndef _READER_WRITER_LOCK_
#define _READER_WRITER_LOCK_

#include <semaphore.h>

typedef struct _rwlock_t {
    sem_t *lock;      // binary semaphore (basic lock)
    sem_t *writelock; // used to allow ONE writer or MANY readers
    int readers;      // count of readers reading in critical section
} rwlock_t;

void rwlock_init(rwlock_t *rw);
void rwlock_acquire_readlock(rwlock_t *rw);
void rwlock_release_readlock(rwlock_t *rw);
void rwlock_acquire_writelock(rwlock_t *rw);
void rwlock_release_writelock(rwlock_t *rw);

#endif
