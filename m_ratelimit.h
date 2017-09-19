
#ifndef M_RATELIMIT_H
#define M_RATELIMIT_H

#include "php_mutils.h"
#include <pthread.h>

#define M_RATELIMIT_PROPERTY_NAME_LIMIT "_limit"
#define M_RATELIMIT_PROPERTY_NAME_SLOT "_slot"

typedef struct {
	pthread_mutex_t mutex;
	size_t visit;
	size_t timeout;
} rlimit;

extern zend_class_entry *m_ratelimit_ce;

MUTILS_STARTUP_FUNCTION(ratelimit);
MUTILS_SHUTDOWN_FUNCTION(ratelimit);

#endif /* M_RATELIMIT_H */