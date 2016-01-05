/**
 * Project: mod_chardet :: Character set detect API
 * File:    mod_chardet_class.c
 *
 * Copyright (c) 2015 JoungKyun.Kim
 *
 * LICENSE: MPL or GPL
 *
 * @category    Charset
 * @package     mod_chardet
 * @author      JoungKyun.Kim <http://oops.org>
 * @copyright   2015 OOPS.org
 * @license     MPL or GPL
 * @version     SVN: $Id$
 * @since       File available since release 0.0.1
 */

/*
 * PHP5 Charaset Detect library module "chardet"
 */

/* {{{ Class API */
#define Z_CHARDET_P(zv) chardet_fetch_object(Z_OBJ_P(zv))

static inline chardet_obj * chardet_fetch_object (zend_object * obj) {
	return (chardet_obj *) ((char *) obj - XtOffsetOf(chardet_obj, std));
}

static int chardet_free_persistent (zend_resource * le, void * ptr) {
	return le->ptr == ptr ? ZEND_HASH_APPLY_REMOVE : ZEND_HASH_APPLY_KEEP;
}

static void chardet_object_free_storage (zend_object * object) {
	chardet_obj * intern = (chardet_obj *) chardet_fetch_object (object);

	zend_object_std_dtor (&intern->std);

	if ( intern->u.ptr ) {
		if ( intern->u.fp->rsrc ) {
			zend_list_delete (intern->u.fp->rsrc);
			zend_hash_apply_with_argument (
				&EG(persistent_list),
				(apply_func_arg_t) chardet_free_persistent,
				&intern->u.ptr
			);
		}
	}

	SAFE_EFREE (object);
}

static void chardet_object_new (zend_class_entry *class_type, zend_object_handlers *handlers, zend_object **retval TSRMLS_DC)
{
	chardet_obj * intern;
	zval  * tmp;

	intern = ecalloc (1, sizeof (chardet_obj) + zend_object_properties_size (class_type));
	zend_object_std_init (&intern->std,class_type TSRMLS_CC);
	handlers->offset = XtOffsetOf(chardet_obj, std);
	handlers->free_obj = (zend_object_free_obj_t) chardet_object_free_storage;
	intern->std.handlers = handlers;

	*retval = &intern->std;
}

static zend_object * chardet_object_new_main (zend_class_entry * class_type TSRMLS_DC) {
	zend_object * retval;

	chardet_object_new (class_type, &chardet_object_handlers, &retval TSRMLS_CC);
	return retval;
}

static zend_object * chardet_object_new_exception (zend_class_entry * class_type TSRMLS_DC) {
	zend_object * retval;

	chardet_object_new (class_type, &chardet_object_handlers_exception, &retval TSRMLS_CC);
	return retval;
}
/* Class API }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
