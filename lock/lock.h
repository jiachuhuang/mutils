
#ifndef LOCK_H
#define LOCK_H

#include <pthread.h>

#ifndef PTHREADMUTEX_LOCKS
#define PTHREADMUTEX_LOCKS
int pthreadmutex_create(pthread_mutex_t *lock, char **error);
void pthreadmutex_destroy(pthread_mutex_t *lock);
int pthreadmutex_lock(pthread_mutex_t *lock, char **error);
void pthreadmutex_unlock(pthread_mutex_t *lock, char **error);
#endif

#define CREATE_LOCK(lock, error) m_create_lock(lock, error)
#define DESTROY_LOCK(lock) m_destroy_lock(lock)
#define LOCK(lock, error) m_lock(lock, error)
#define UNLOCK(lock, error) m_unlock(lock, error)


#if defined(PTHREADMUTEX_LOCKS)
#define m_lck_t pthread_mutex_t
#define m_create_lock(a,b) pthreadmutex_create(a,b)
#define m_destroy_lock(a) pthreadmutex_destroy(a)
#define m_lock(a,b) pthreadmutex_lock(a,b)
#define m_unlock(a,b) pthreadmutex_unlock(a,b)
#endif

#endif /* LOCK_H */
