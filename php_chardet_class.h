/*
 * Copyright (c) 2016 JoungKyun.Kim <http://oops.org> all rights reserved
 *
 * This file is part of mod_chardet
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published
 * by the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU LESSER GENERAL PUBLIC LICENSE for more details.
 *
 * You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef PHP_CHARDET_CLASS_H
#define PHP_CHARDET_CLASS_H

/* {{{ Exception entry */
const zend_function_entry chardet_methods_exception[] = {
    {NULL, NULL, NULL}
};
/* }}} */

/* Exception declear {{{
 */
#if defined(HAVE_SPL)
extern PHPAPI zend_class_entry *spl_ce_RuntimeException;
extern PHPAPI zend_class_entry *spl_ce_Countable;
#endif

#define CHARDET_REPLACE_ERROR_HANDLING \
	zend_replace_error_handling ( \
		EH_THROW, \
		chardet_ce_exception, \
		&error_handling \
	)

#define CHARDET_RESTORE_ERROR_HANDLING zend_restore_error_handling (&error_handling)
/* }}} */

/* {{{ chardet_deps[]
 *
 * CHARDET dependancies
 */
const zend_module_dep chardet_deps[] = {
#if defined(HAVE_SPL)
	ZEND_MOD_REQUIRED("spl")
#endif
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ For Class declears */
const zend_function_entry chardet_methods[] = {
	PHP_ME_MAPPING (__construct,   chardet_open,               NULL,                   ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (close,         chardet_close,              arginfo_chardet_close,  ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING (detect,        chardet_detect,             arginfo_chardet_detect, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

#define REGISTER_CHARDET_CLASS(parent) { \
	zend_class_entry ce; \
	INIT_CLASS_ENTRY (ce, "CHARDET", chardet_methods); \
	ce.create_object = chardet_object_new_main; \
	chardet_ce = zend_register_internal_class_ex (&ce, parent); \
	memcpy(&chardet_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
	chardet_object_handlers.clone_obj = NULL; \
	chardet_ce->ce_flags |= ZEND_ACC_FINAL; \
}

#define REGISTER_CHARDET_PER_CLASS(name, c_name, parent) { \
	zend_class_entry ce; \
	INIT_CLASS_ENTRY(ce, "CHARDET" # name, chardet_methods_ ## c_name); \
	ce.create_object = chardet_object_new_ ## c_name; \
	chardet_ce_ ## c_name = zend_register_internal_class_ex(&ce, parent); \
	memcpy(&chardet_object_handlers_ ## c_name, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
	chardet_object_handlers_ ## c_name.clone_obj = NULL; \
	chardet_ce_ ## c_name->ce_flags |= ZEND_ACC_FINAL; \
}

zend_class_entry * chardet_ce;
zend_class_entry * chardet_ce_exception;
static zend_object_handlers chardet_object_handlers;
static zend_object_handlers chardet_object_handlers_exception;

typedef struct _chardet_object {
	CharDetFP * fp;
	zend_object std;
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
