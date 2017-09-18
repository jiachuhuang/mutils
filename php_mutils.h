/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_MUTILS_H
#define PHP_MUTILS_H

extern zend_module_entry mutils_module_entry;
#define phpext_mutils_ptr &mutils_module_entry

#define PHP_MUTILS_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_MUTILS_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_MUTILS_API __attribute__ ((visibility("default")))
#else
#	define PHP_MUTILS_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define MUTILS_STARTUP_FUNCTION(module) ZEND_MINIT_FUNCTION(m_##module)
#define MUTILS_RINIT_FUNCTION(module)    ZEND_RINIT_FUNCTION(m_##module)
#define MUTILS_STARTUP(module)         ZEND_MODULE_STARTUP_N(m_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define MUTILS_SHUTDOWN_FUNCTION(module)   ZEND_MSHUTDOWN_FUNCTION(m_##module)
#define MUTILS_SHUTDOWN(module)        ZEND_MODULE_SHUTDOWN_N(m_##module)(INIT_FUNC_ARGS_PASSTHRU)


ZEND_BEGIN_MODULE_GLOBALS(mutils)
  zend_long  ratelimit_slot_nums;
  zend_bool  ratelimit_enable;
  zend_long  ratelimit_limit;
ZEND_END_MODULE_GLOBALS(mutils)


/* Always refer to the globals in your function as MUTILS_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define MUTILS_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(mutils, v)

#if defined(ZTS) && defined(COMPILE_DL_MUTILS)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

// 没这句编译不了？
extern ZEND_DECLARE_MODULE_GLOBALS(mutils);

#endif	/* PHP_MUTILS_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
