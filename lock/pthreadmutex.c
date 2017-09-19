#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#ifdef PTHREADMUTEX_LOCKS

int pthreadmutex_create(pthread_mutex_t *lock, char **error) {
	pthread_mutexattr_t attr;
	int ret = 0;

	if((ret = pthread_mutexattr_init(&attr))) {
		sprintf(*error, "Init mutex attr failed : %s", strerror(errno));
		return ret;
	}

	if((ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED))) {
		pthread_mutexattr_destroy(&attr);
		sprintf(*error, "Init mutex attr failed : %s", strerror(errno));
		return ret;		
	}	

	if((ret = pthread_mutex_init(lock, &attr))) {
		pthread_mutexattr_destroy(&attr);
		sprintf(*error, "Init mutex failed : %s", strerror(errno));
		return ret;			
	}

	pthread_mutexattr_destroy(&attr);

	return ret;
}

void pthreadmutex_destroy(pthread_mutex_t *lock) {
	return;
}

int pthreadmutex_lock(pthread_mutex_t *lock, char **error) {
	int ret = 0;

    ret = pthread_mutex_lock(lock);
    if(ret == EINVAL) {
        *error = "unable to obtain pthread lock (EINVAL)";
        return ret;
    } else if(ret == EDEADLK) {
        *error = "unable to obtain pthread lock (EDEADLK)";
        pthread_mutex_unlock(lock);
        return ret;
    }	

    return ret;
}

void pthreadmutex_unlock(pthread_mutex_t *lock, char **error) {
	int ret = 0;
    if((ret = pthread_mutex_unlock(lock))) {
        *error = "unable to unlock pthread lock";
    }
    return ret;
}

#endif /* PTHREADMUTEX_LOCKS */


