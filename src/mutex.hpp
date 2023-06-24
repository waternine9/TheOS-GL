#ifndef H_TOS_MUTEX
#define H_TOS_MUTEX

#include <atomic>

typedef struct {
    std::atomic<bool> Taken;
} mutex;

mutex MutexCreate();
bool MutexTryLock(mutex *Mutex);
void MutexLock(mutex *Mutex);
void MutexRelease(mutex *Mutex);

#endif