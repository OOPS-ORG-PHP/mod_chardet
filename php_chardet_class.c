/**
 * Project: mod_chardet :: Character set detect API
 * File:    mod_chardet_class.c
 *
 * Copyright 2021. JoungKyun.Kim all rights reserved
 *
 * LICENSE: MPL or GPL
 *
 * @category    Charset
 * @package     mod_chardet
 * @author      JoungKyun.Kim <http://oops.org>
 * @copyright   2021. JoungKyun.Kim
 * @license     MPL or GPL
 * @since       File available since release 0.0.1
 */

/*
 * PHP5 Charaset Detect library module "chardet"
 */

/* {{{ Class API */
static int chardet_free_persistent (zend_rsrc_list_entry * le, void * ptr TSRMLS_DC) {
	return le->ptr == ptr ? ZEND_HASH_APPLY_REMOVE : ZEND_HASH_APPLY_KEEP;
}

static void chardet_object_free_storage (void * object TSRMLS_DC) {
	chardet_obj * intern = (chardet_obj *) object;

	zend_object_std_dtor (&intern->std TSRMLS_CC);

	if ( intern->u.ptr ) {
		if ( intern->u.fp->rsrc ) {
			zend_list_delete (intern->u.fp->rsrc);
			zend_hash_apply_with_argument (
				&EG(persistent_list),
				(apply_func_arg_t) chardet_free_persistent,
				&intern->u.ptr TSRMLS_CC
			);
		}
	}

	efree(object);
}

static void chardet_object_new (zend_class_entry *class_type, zend_object_handlers *handlers, zend_object_value *retval TSRMLS_DC)
{
	chardet_obj * intern;
	zval  * tmp;

	intern = emalloc (sizeof (chardet_obj));
	memset (intern, 0, sizeof (chardet_obj));

	zend_object_std_init (&intern->std, class_type TSRMLS_CC);
	retval->handle = zend_objects_store_put(
		intern,
		(zend_objects_store_dtor_t) zend_objects_destroy_object,
		(zend_objects_free_object_storage_t) chardet_object_free_storage,
		NULL TSRMLS_CC
	);
	retval->handlers = handlers;
}

static zend_object_value chardet_object_new_main (zend_class_entry * class_type TSRMLS_DC) {
	zend_object_value retval;

	chardet_object_new (class_type, &chardet_object_handlers, &retval TSRMLS_CC);
	return retval;
}

static zend_object_value chardet_object_new_exception (zend_class_entry * class_type TSRMLS_DC) {
	zend_object_value retval;

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
