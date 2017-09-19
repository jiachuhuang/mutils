#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_mutils.h"
#include "SAPI.h"

#include "m_ratelimit.h"
#include "alloc/alloc.h"

#include <time.h>

zend_class_entry *m_ratelimit_ce;
rlimit *rlimit_slots;

ZEND_BEGIN_ARG_INFO_EX(arginfo_m_ratelimit_ctor, 0, 0, 0)
	ZEND_ARG_INFO(0, slot)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_m_ratelimit_void, 0, 0, 0)
ZEND_END_ARG_INFO()


PHP_METHOD(m_ratelimit, __construct) {
	long solt = 0, limit = MUTILS_G(ratelimit_limit);

	if(!MUTILS_G(ratelimit_enable)) {
		RETURN_FALSE;
	}

	if(zend_parse_parameters(ZEND_NUM_ARGS(), "|ll", &solt, &limit) == FAILURE) {
		return;
	}

	if(solt < 0 || solt >= MUTILS_G(ratelimit_slot_nums)) {
		php_error_docref(NULL, E_WARNING, "Solt index out of size [0,%d) ", MUTILS_G(ratelimit_slot_nums));
		return;			
	}

	if(limit <= 0 ) {
		php_error_docref(NULL, E_WARNING, "Invaild property limit(%d) ", limit);		
		return;			
	}

	zend_update_property_long(m_ratelimit_ce, getThis(), ZEND_STRL(M_RATELIMIT_PROPERTY_NAME_SLOT), solt);
	zend_update_property_long(m_ratelimit_ce, getThis(), ZEND_STRL(M_RATELIMIT_PROPERTY_NAME_LIMIT), limit);
}

PHP_METHOD(m_ratelimit, acquire) {
	zval *slot, *limit;
	time_t tv;

	if(!MUTILS_G(ratelimit_enable)) {
		RETURN_TRUE;
	}

	slot = zend_read_property(m_ratelimit_ce, getThis(),  ZEND_STRL(M_RATELIMIT_PROPERTY_NAME_SLOT), 1, NULL);
	limit = zend_read_property(m_ratelimit_ce, getThis(),  ZEND_STRL(M_RATELIMIT_PROPERTY_NAME_LIMIT), 1, NULL);

	tv = time(NULL);
	pthread_mutex_lock(&(rlimit_slots[Z_LVAL_P(slot)].mutex));

	if(tv < rlimit_slots[Z_LVAL_P(slot)].timeout) {
		rlimit_slots[Z_LVAL_P(slot)].visit += 1;
		if( rlimit_slots[Z_LVAL_P(slot)].visit <= Z_LVAL_P(limit)) {
			goto allow;
		} else {
			goto deny;
		}
	} else {
		rlimit_slots[Z_LVAL_P(slot)].visit = 1;
		rlimit_slots[Z_LVAL_P(slot)].timeout = tv + 1;
		if( rlimit_slots[Z_LVAL_P(slot)].visit <= Z_LVAL_P(limit)) {
			goto allow;
		} else {
			goto deny;
		}
	}

allow:
	pthread_mutex_unlock(&(rlimit_slots[Z_LVAL_P(slot)].mutex));
	RETURN_TRUE;

deny:
	pthread_mutex_unlock(&(rlimit_slots[Z_LVAL_P(slot)].mutex));
	RETURN_FALSE;
}

zend_function_entry m_ratelimit_methods[] = {
	PHP_ME(m_ratelimit, __construct, arginfo_m_ratelimit_ctor, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)	
	PHP_ME(m_ratelimit, acquire, arginfo_m_ratelimit_void, ZEND_ACC_PUBLIC)	
	{NULL, NULL, NULL}
};

MUTILS_STARTUP_FUNCTION(ratelimit) {
	zend_class_entry ce;
	alloc_handlers *ah;
	char *error;
	size_t size;
	int i;
	pthread_mutexattr_t attr;

	if(MUTILS_G(ratelimit_enable)) {
		if(MUTILS_G(ratelimit_slot_nums) > 10) {
			php_error(E_ERROR, "Ratelimit slot nums(%d) bigger than 10", MUTILS_G(ratelimit_slot_nums));
			return FAILURE;	
		}

		size = MUTILS_G(ratelimit_slot_nums) * sizeof(rlimit);
		ah = &alloc_handler;
		if(!(ah->create_segments((void **)&rlimit_slots, size, &error))) {
			php_error(E_ERROR, "Shared memory allocator failed '%s': %s", error, strerror(errno));
			return FAILURE;			
		}

		if(pthread_mutexattr_init(&attr)) {
			php_error(E_ERROR, "Init mutex attr failed : %s", strerror(errno));
			ah->detach_segment((void **)&rlimit_slots, size);
			return FAILURE;	
		}

		if(pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)) {
			php_error(E_ERROR, "Set mutex attr failed : %s", strerror(errno));
			pthread_mutexattr_destroy(&attr);
			ah->detach_segment((void **)&rlimit_slots, size);
			return FAILURE;				
		}

		for (i = 0; i < MUTILS_G(ratelimit_slot_nums); ++i) {
			if(pthread_mutex_init(&(rlimit_slots[i].mutex), &attr)) {
				pthread_mutexattr_destroy(&attr);
				ah->detach_segment((void **)&rlimit_slots, size);
				return FAILURE;
			}
			rlimit_slots[i].visit = 0;
			rlimit_slots[i].timeout = 0;
		}

		pthread_mutexattr_destroy(&attr);
	} 

	INIT_CLASS_ENTRY(ce, "M_ratelimit", m_ratelimit_methods);
	m_ratelimit_ce = zend_register_internal_class(&ce);

	zend_declare_property_long(m_ratelimit_ce, ZEND_STRL(M_RATELIMIT_PROPERTY_NAME_LIMIT), MUTILS_G(ratelimit_limit), ZEND_ACC_PROTECTED);
	zend_declare_property_long(m_ratelimit_ce, ZEND_STRL(M_RATELIMIT_PROPERTY_NAME_SLOT), 0, ZEND_ACC_PROTECTED);

	return SUCCESS;
}

MUTILS_SHUTDOWN_FUNCTION(ratelimit) {
	size_t size;
	alloc_handlers *ah;

	if(MUTILS_G(ratelimit_enable)) {
		size = MUTILS_G(ratelimit_slot_nums) * sizeof(rlimit);
		ah->detach_segment((void **)&rlimit_slots, size);
	}

	return SUCCESS;
}



