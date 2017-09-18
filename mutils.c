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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "SAPI.h"
#include "ext/standard/info.h"
#include "php_mutils.h"

ZEND_DECLARE_MODULE_GLOBALS(mutils)


/* True global resources - no need for thread safety here */
static int le_mutils;

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_BOOLEAN("mutils.ratelimit_enable",      "1", PHP_INI_ALL, OnUpdateBool, ratelimit_enable, zend_mutils_globals, mutils_globals)
	STD_PHP_INI_ENTRY("mutils.ratelimit_slot_nums", 		"5", PHP_INI_ALL, OnUpdateLongGEZero, ratelimit_slot_nums, zend_mutils_globals, mutils_globals)
	STD_PHP_INI_ENTRY("mutils.ratelimit_limit", 		"64", PHP_INI_ALL, OnUpdateLongGEZero, ratelimit_limit, zend_mutils_globals, mutils_globals)
PHP_INI_END()

/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(mutils)
{
	REGISTER_INI_ENTRIES();
	
	MUTILS_STARTUP(ratelimit);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(mutils)
{
	UNREGISTER_INI_ENTRIES();

	MUTILS_SHUTDOWN(ratelimit);
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(mutils)
{
#if defined(COMPILE_DL_MUTILS) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(mutils)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(mutils)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "mutils support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ mutils_functions[]
 *
 * Every user visible function must have an entry in mutils_functions[].
 */
const zend_function_entry mutils_functions[] = {
	PHP_FE_END	/* Must be the last line in mutils_functions[] */
};
/* }}} */

/* {{{ mutils_module_entry
 */
zend_module_entry mutils_module_entry = {
	STANDARD_MODULE_HEADER,
	"mutils",
	mutils_functions,
	PHP_MINIT(mutils),
	PHP_MSHUTDOWN(mutils),
	PHP_RINIT(mutils),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(mutils),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(mutils),
	PHP_MUTILS_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MUTILS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(mutils)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
