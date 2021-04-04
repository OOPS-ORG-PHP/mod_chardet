#ifndef PHP_CHARDET_CLASS_H
#define PHP_CHARDET_CLASS_H

/* {{{ Exception entry */
#if PHP_VERSION_ID >= 50300
const
#endif
zend_function_entry chardet_methods_exception[] = {
    {NULL, NULL, NULL}
};
/* }}} */

/* Exception declear {{{
 */
#if PHP_MAJOR_VERSION >= 5
#if defined(HAVE_SPL) && PHP_VERSION_ID >= 50100
extern PHPAPI zend_class_entry *spl_ce_RuntimeException;
extern PHPAPI zend_class_entry *spl_ce_Countable;
#endif // defined(HAVE_SPL) && ((PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1))

#if PHP_VERSION_ID >= 50300
#define CHARDET_REPLACE_ERROR_HANDLING \
	zend_replace_error_handling ( \
		object ? EH_THROW : EH_NORMAL, \
		chardet_ce_exception, \
		&error_handling TSRMLS_CC \
	)
#define CHARDET_RESTORE_ERROR_HANDLING zend_restore_error_handling (&error_handling TSRMLS_CC)
#else
#define CHARDET_REPLACE_ERROR_HANDLING \
	php_set_error_handling ( \
		object ? EH_THROW : EH_NORMAL, \
		chardet_ce_exception TSRMLS_CC \
	)
#define CHARDET_RESTORE_ERROR_HANDLING php_std_error_handling()
#endif // PHP_VERSION_ID >= 50300

#else
#define CHARDET_REPLACE_ERROR_HANDLIN0 int chardet_error_dummy_handing = 1
#define CHARDET_RESTORE_ERROR_HANDLING chardet_error_dummy_handing = 0
#endif
/* }}} */

/* {{{ chardet_deps[]
 *
 * CHARDET dependancies
 */
#if PHP_VERSION_ID >= 50300
const
#endif
zend_module_dep chardet_deps[] = {
//#if defined(HAVE_SPL) && ((PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1))
#if defined(HAVE_SPL) && PHP_VERSION_ID >= 50100
	ZEND_MOD_REQUIRED("spl")
#endif
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ For Class declears */
#if PHP_VERSION_ID >= 50300
const
#endif
zend_function_entry chardet_methods[] = {
	PHP_ME_MAPPING (__construct,   chardet_open,               NULL,                   ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (close,         chardet_close,              arginfo_chardet_close,  ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (detect,        chardet_detect,             arginfo_chardet_detect, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

#define REGISTER_CHARDET_CLASS(parent) { \
	zend_class_entry ce; \
	INIT_CLASS_ENTRY (ce, "CHARDET", chardet_methods); \
	ce.create_object = chardet_object_new_main; \
	chardet_ce = zend_register_internal_class_ex (&ce, parent, NULL TSRMLS_CC); \
	memcpy(&chardet_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
	chardet_object_handlers.clone_obj = NULL; \
	chardet_ce->ce_flags |= ZEND_ACC_FINAL_CLASS; \
}

#define REGISTER_CHARDET_PER_CLASS(name, c_name, parent) { \
	zend_class_entry ce; \
	INIT_CLASS_ENTRY(ce, "CHARDET" # name, chardet_methods_ ## c_name); \
	ce.create_object = chardet_object_new_ ## c_name; \
	chardet_ce_ ## c_name = zend_register_internal_class_ex(&ce, parent, NULL TSRMLS_CC); \
	memcpy(&chardet_object_handlers_ ## c_name, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
	chardet_object_handlers_ ## c_name.clone_obj = NULL; \
	chardet_ce_ ## c_name->ce_flags |= ZEND_ACC_FINAL_CLASS; \
}

zend_class_entry * chardet_ce;
zend_class_entry * chardet_ce_exception;
static zend_object_handlers chardet_object_handlers;
static zend_object_handlers chardet_object_handlers_exception;

typedef struct _chardet_object {
	zend_object     std;
	union {
		CharDetFP * fp;
		void      * ptr;
	} u;
} chardet_obj;
/* For Class declears }}} */

#endif	/* PHP_CHARDET_CLASS_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
