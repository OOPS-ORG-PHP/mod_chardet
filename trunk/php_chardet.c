/**
 * Project: mod_chardet :: Character set detect API
 * File:    mod_chardet.c
 *
 * Copyright (c) 2012 JoungKyun.Kim
 *
 * LICENSE: MPL or GPL
 *
 * @category    Charset
 * @package     mod_chardet
 * @author      JoungKyun.Kim <http://oops.org>
 * @copyright   2012 OOPS.org
 * @license     MPL or GPL
 * @version     SVN: $Id$
 * @since       File available since release 0.0.1
 */

/*
 * PHP5 Charaset Detect library module "chardet"
 */

#ifdef HAVE_CONFIG_H
#include "config.h"

// avoid conflict redefine
#ifdef PACKAGE_NAME
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_VERSION
#undef PACKAGE_TARNAME
#undef PACKAGE_BUGREPORT
#endif
#endif

#include <stdio.h>
#include <unistd.h>

#ifdef HAVE_MOZ_CHARDET
#include <chardet.h>

// avoid conflict redefine
#ifdef PACKAGE_NAME
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_VERSION
#undef PACKAGE_TARNAME
#undef PACKAGE_BUGREPORT
#endif
#endif

#ifdef HAVE_ICU_CHARDET
#include <unicode/ucsdet.h>
#endif

#ifdef HAVE_PY_CHARDET
#include <Python.h>
#endif

#include "php.h"
#include "php_ini.h"
#include "SAPI.h"
#include "ext/standard/info.h"

#include "php_chardet.h"

/* If you declare any globals in php_chardet.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(chardet)
*/

/* True global resources - no need for thread safety here */
static int le_chardet;

ZEND_BEGIN_ARG_INFO_EX(arginfo_chardet_detect, 0, 0, 2)
	ZEND_ARG_INFO(0, fp_link)
	ZEND_ARG_INFO(0, buf)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_chardet_close, 0, 0, 1)
	ZEND_ARG_INFO(0, fp_link)
ZEND_END_ARG_INFO()

/* {{{ INCLUDE CHARDET Classify header */
#include "php_chardet_class.h"
/* INCLUDE CHARDET Classify header }}} */

/* {{{ chardet_functions[]
 *
 * Every user visible function must have an entry in chardet_functions[].
 */
const zend_function_entry chardet_functions[] = {
	PHP_FE(chardet_version,			NULL)
#ifdef HAVE_MOZ_CHARDET
	PHP_FE(chardet_moz_version,		NULL)
#endif
#ifdef HAVE_ICU_CHARDET
	PHP_FE(chardet_icu_version,		NULL)
#endif
#ifdef HAVE_PY_CHARDET
	PHP_FE(chardet_py_version,		NULL)
#endif
	PHP_FE(chardet_open,			NULL)
	PHP_FE(chardet_detect,			arginfo_chardet_detect)
	PHP_FE(chardet_close,			arginfo_chardet_close)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ chardet_module_entry
 */
zend_module_entry chardet_module_entry = {
#if ZEND_MODULE_API_NO >= 20050922
	STANDARD_MODULE_HEADER_EX,
	NULL,
	chardet_deps,
#elif ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"chardet",
	chardet_functions,
	PHP_MINIT(chardet),
	PHP_MSHUTDOWN(chardet),
	NULL,
	NULL,
	PHP_MINFO(chardet),
#if ZEND_MODULE_API_NO >= 20010901
	CHARDET_VERSION, /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CHARDET
ZEND_GET_MODULE(chardet)
#endif

/* {{{ _close_chardet_link (zend_rsrc_list_entry *rsrc TSRMLS_DC)
 */
static void _close_chardet_link (zend_rsrc_list_entry * rsrc TSRMLS_DC)
{
	CharDetFP * fp = (CharDetFP *) rsrc->ptr;

#ifdef HAVE_MOZ_CHARDET
	if ( fp->moz_status != 0 ) {
		detect_destroy (&(fp->moz));
		fp->moz_status = 0;
	}
#endif

#ifdef HAVE_ICU_CHARDET
	if ( fp->csd_status != 0 ) {
		ucsdet_close (fp->csd);
		fp->csd_status = 0;
	}
#endif

#ifdef HAVE_PY_CHARDET
	fp->pMainModule = NULL;
	fp->pMainDictionary = NULL;
#endif

	SAFE_EFREE(fp);
}
/* }}} */

/* {{{ INCLUDE CHARDET Classify API */
#include "php_chardet_class.c"
/* INCLUDE CHARDET Classify header }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(chardet)
{
	le_chardet = zend_register_list_destructors_ex (_close_chardet_link, NULL, "Chardet link", module_number);

	REGISTER_CHARDET_CLASS(NULL);
	chardet_ce->ce_flags &= ~ZEND_ACC_FINAL_CLASS;
	chardet_ce->constructor->common.fn_flags |= ZEND_ACC_FINAL;

#if defined(HAVE_SPL) && ((PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1))
	REGISTER_CHARDET_PER_CLASS(Exception, exception, spl_ce_RuntimeException);
#elif PHP_MAJOR_VERSION >= 5
	REGISTER_CHARDET_PER_CLASS(Exception, exception, zend_exception_get_default(TSRMLS_C));
#endif

	REGISTER_LONG_CONSTANT ("CHARDET_ICU", CHARDET_ICU, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("CHARDET_MOZ", CHARDET_MOZ, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("CHARDET_PY", CHARDET_PY, CONST_PERSISTENT | CONST_CS);

#ifdef HAVE_PY_CHARDET
	Py_InitializeEx (0);
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDORN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(chardet)
{
#ifdef HAVE_PY_CHARDET
	Py_Finalize ();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(chardet)
{
	php_info_print_table_start();
	php_info_print_table_colspan_header(2, "Character Set Detect extension support");
	php_info_print_table_row(2, "Summary", "Charset detect with "
#ifdef HAVE_MOZ_CHARDET
								"Mozilla "
#endif
#ifdef HAVE_ICU_CHARDET
								"ICU "
#endif
#ifdef HAVE_PY_CHARDET
								"Python"
#endif
	" library");
	php_info_print_table_row(2, "URL", "http://devel.oops.org/");
	php_info_print_table_row(2, "Build version", CHARDET_VERSION);
#ifdef HAVE_ICU_CHARDET
	php_info_print_table_row(2, "ICU Library version", U_ICU_VERSION);
#endif
#ifdef HAVE_PY_CHARDET
	php_info_print_table_row(2, "Python Chardet Support", PY_CHARDET_VERSION);
#endif
	php_info_print_table_end();
}
/* }}} */

/* {{{ proto char chardet_version (void)
 *  print chardet extension build number */
PHP_FUNCTION(chardet_version)
{
	RETURN_STRING (CHARDET_VERSION, 1);
}
/* }}} */

#ifdef HAVE_MOZ_CHARDET
/* {{{ proto char chardet_moz_version (void)
 *  print chardet icu library version */
PHP_FUNCTION(chardet_moz_version)
{
	RETURN_STRING (LIBCHARDET_VER, 1);
}
/* }}} */
#endif

#ifdef HAVE_ICU_CHARDET
/* {{{ proto char chardet_icu_version (void)
 *  print chardet icu library version */
PHP_FUNCTION(chardet_icu_version)
{
	RETURN_STRING (U_ICU_VERSION, 1);
}
/* }}} */
#endif

#ifdef HAVE_PY_CHARDET
/* {{{ proto char chardet_icu_version (void)
 *  print chardet icu library version */
PHP_FUNCTION(chardet_py_version)
{
	RETURN_STRING (PY_CHARDET_VERSION, 1);
}
/* }}} */
#endif

/* {{{ proto (resource) chardet_open (void)
 */
PHP_FUNCTION(chardet_open)
{
	zval      * object = getThis ();
	CharDetFP * fp = NULL;
	zend_error_handling error_handling;
#ifdef HAVE_ICU_CHARDET
	UErrorCode status = U_ZERO_ERROR;
#endif

	CHARDET_REPLACE_ERROR_HANDLING;

	if ( (fp = (CharDetFP *) emalloc (sizeof (CharDetFP))) == NULL ) {
		php_error (E_ERROR, "handle memory allocation failed.");
		CHARDET_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

#ifdef HAVE_MOZ_CHARDET
	fp->moz = detect_init ();
	if ( fp->moz == NULL ) {
		php_error (E_WARNING, "Mozilla chardet handle open failed.");
		chardet_fp_free (&fp);
		CHARDET_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	fp->moz_status = 1;
#endif

#ifdef HAVE_ICU_CHARDET
	fp->csd = ucsdet_open (&status);

	if ( status != U_ZERO_ERROR ) {
		php_error (E_WARNING, "ICU chardet handle open failed.");
		chardet_fp_free (&fp);
		CHARDET_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	fp->csd_status = 1;
#endif

#ifdef HAVE_PY_CHARDET
	if ( (fp->pMainModule = PyImport_AddModule ("__main__")) == NULL ) {
		chardet_fp_free (&fp);
		CHARDET_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	fp->pMainDictionary = PyModule_GetDict (fp->pMainModule);
	if ( PyRun_SimpleString ("import chardet.universaldetector\n") == -1 ) {
		php_error (E_WARNING, "Load failed python chardet module");
		chardet_fp_free (&fp);
		CHARDET_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	if ( PyRun_SimpleString ("detector = chardet.universaldetector.UniversalDetector()\n") == -1 ) {
		chardet_fp_free (&fp);
		CHARDET_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}
#endif

	fp->rsrc = ZEND_REGISTER_RESOURCE (
			object ? NULL : return_value,
			fp, le_chardet
	);

	if ( object ) {
		chardet_obj * obj;
		obj = (chardet_obj *) zend_object_store_get_object (object TSRMLS_CC);
		obj->u.fp = fp;
	}

	CHARDET_RESTORE_ERROR_HANDLING;
}
/* }}} */

/* {{{ proto (bool) chardet_close (resource link)
 */
PHP_FUNCTION(chardet_close)
{
	zval      * fp_link;
	zval      * object = getThis ();
	CharDetFP * fp;
	chardet_obj * obj;

	if ( object ) {
		obj = (chardet_obj *) zend_object_store_get_object (object TSRMLS_CC);
		if ( ! obj->u.fp )
			RETURN_TRUE;
		zend_list_delete (obj->u.fp->rsrc);
	} else {
		if ( zend_parse_parameters (ZEND_NUM_ARGS () TSRMLS_CC, "r", &fp_link) == FAILURE )
			return;

		ZEND_FETCH_RESOURCE (fp, CharDetFP *, &fp_link, -1, "Chardet link", le_chardet);
		zend_list_delete (Z_RESVAL_P (fp_link));
	}

	RETURN_TRUE;
}
// }}}

/* {{{ proto (object|false) chardet_detect (resource link, string buffer[, type method])
 *  resouce : stream
 *  string : string that checked encoding
 *  type   : CHARDET_ICU or CHARDET_MOZ or CHARDET_PY
 *  return object result or false
 *     -> encoding
 *     -> lang
 *     -> confidence
 *     -> status (if value is not 0, false)
 */
PHP_FUNCTION(chardet_detect)
{
	zval       * fp_link;
	char       * buf;
	int          buflen;
	CharDetFP  * fp;
	CharDetObj * obj    = NULL;
	const char * string = NULL;
#ifdef HAVE_MOZ_CHARDET
	int          type = CHARDET_MOZ;
#else
	int          type = CHARDET_ICU;
#endif
	short        r = 0;

	zval        * object = getThis ();
	chardet_obj * Obj;
	zend_error_handling error_handling;

	CHARDET_REPLACE_ERROR_HANDLING;

	if ( object ) {
		if ( zend_parse_parameters
				(ZEND_NUM_ARGS () TSRMLS_CC, "s|l", &buf, &buflen, &type) == FAILURE )
		{
			CHARDET_RESTORE_ERROR_HANDLING;
			return;
		}
	} else {
		if ( zend_parse_parameters
				(ZEND_NUM_ARGS () TSRMLS_CC, "rs|l", &fp_link, &buf, &buflen, &type) == FAILURE )
		{
			CHARDET_RESTORE_ERROR_HANDLING;
			return;
		}
	}

	if ( object ) {
		Obj = (chardet_obj *) zend_object_store_get_object (object TSRMLS_CC);
		fp = Obj->u.fp;
		if ( ! fp ) {
			php_error_docref (NULL TSRMLS_CC, E_WARNING, "No CHARDET object available");
			CHARDET_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
		}
	} else
		ZEND_FETCH_RESOURCE (fp, CharDetFP *, &fp_link, -1, "Chardet link", le_chardet);

	string = (const char *) buf;

	if ( chardet_obj_init (&obj) < 0 ) {
		php_error (E_ERROR, "Structure initialize failed on chardet ()");
		CHARDET_RESTORE_ERROR_HANDLING;
		RETURN_FALSE;
	}

	switch (type) {
		case CASE_CHARDET_MOZ :
#ifdef HAVE_MOZ_CHARDET
			r = moz_chardet (fp, string , &obj);
#else
			chardet_obj_free (&obj);
			php_error (E_ERROR, "Unsupport this rumtimes. Build with --enable-mod-chardet option");
			CHARDET_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
#endif
			break;
		case CASE_CHARDET_ICU :
#ifdef HAVE_ICU_CHARDET
			r = icu_chardet (fp, string , &obj);
#else
			chardet_obj_free (&obj);
			php_error (E_ERROR, "Unsupport this rumtimes. Build with --enable-icu-chardet option");
			CHARDET_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
#endif
			break;
		case CASE_CHARDET_PY :
#ifdef HAVE_PY_CHARDET
			r = py_chardet (fp, string, &obj);
#else
			chardet_obj_free (&obj);
			php_error (E_ERROR, "Unsupport this rumtimes. Build with --enable-py-chardet option");
			CHARDET_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
#endif
			break;
		default :
			chardet_obj_free (&obj);
			php_error (E_ERROR, "Unknown TYPE argument 3 on chardet (). Use CHARDET_MOZ or CHARDET_ICU or CHARDET_PY");
			CHARDET_RESTORE_ERROR_HANDLING;
			RETURN_FALSE;
	}

	object_init (return_value);
	add_property_string (return_value, "encoding", obj->encoding ? obj->encoding : "", 1);
	add_property_long (return_value, "confidence", obj->confidence);
	add_property_long (return_value, "status", obj->status);
	if ( type == CASE_CHARDET_ICU )
		add_property_string (return_value, "lang", obj->lang ? obj->lang : "", 1);

	chardet_obj_free (&obj);
	CHARDET_RESTORE_ERROR_HANDLING;
}
/* }}} */

void chardet_fp_free (CharDetFP ** fp) {
#ifdef HAVE_MOZ_CHARDET
	if ( (*fp)->moz_status )
		detect_destroy (&((*fp)->moz));
	(*fp)->moz_status = 0;
#endif
#ifdef HAVE_ICU_CHARDET
	if ( (*fp)->csd_status )
		ucsdet_close ((*fp)->csd);
	(*fp)->csd_status = 0;
#endif
}

short chardet_obj_init (CharDetObj ** obj) {
	*obj = (CharDetObj *) emalloc (sizeof (CharDetObj));

	if ( *obj == NULL )
		return -1;

	(*obj)->encoding = NULL;
	(*obj)->lang = NULL;
	(*obj)->confidence = 0;
	(*obj)->status = 0;

	return 0;
}

void chardet_obj_free (CharDetObj ** obj) {
	if ( *obj != NULL ) {
		SAFE_EFREE((*obj)->encoding)
		SAFE_EFREE((*obj)->lang)
		SAFE_EFREE(*obj)
	}
}

#ifdef HAVE_MOZ_CHARDET
short moz_chardet (CharDetFP * fp, const char * buf, CharDetObj ** obj) {
	DetectObj *mo;
	short r;

	mo = detect_obj_init ();
	if ( mo == NULL ) {
		(*obj)->status = CHARDET_MEM_ALLOCATED_FAIL;
		return -1;
	}

	detect_reset (&(fp->moz));
	r = detect_handledata (&(fp->moz), buf, &mo);
	(*obj)->status = r;

	if ( r == CHARDET_OUT_OF_MEMORY )
		return -1;

	if ( ! mo->encoding ) {
		(*obj)->confidence = 0;
		return -1;
	}

	(*obj)->encoding = estrdup (mo->encoding);
	(*obj)->confidence = mo->confidence * 100;

	return 0;
}
#endif

#ifdef HAVE_ICU_CHARDET
short icu_chardet (CharDetFP * fp, const char * buf, CharDetObj ** obj) {
	const UCharsetMatch * ucm;
	UErrorCode status = U_ZERO_ERROR;
	CharDetObj * ret;

	ucsdet_setText(fp->csd, buf, strlen (buf), &status);
	(*obj)->status = status;

	if ( status != U_ZERO_ERROR )
		return -1;

	ucm = ucsdet_detect(fp->csd, &status);
	(*obj)->status = status;

	if ( status != U_ZERO_ERROR )
		return -1;

	(*obj)->encoding = estrdup (ucsdet_getName (ucm, &status));
	(*obj)->lang = estrdup (ucsdet_getLanguage (ucm, &status));
	(*obj)->confidence = ucsdet_getConfidence (ucm, &status);

	return 0;
}
#endif

#ifdef HAVE_PY_CHARDET
short py_chardet (CharDetFP * fp, const char * buf, CharDetObj ** obj) {
	PyObject * pResult;
	PyObject * key, * value;
	char * pytmp = NULL;
	char * pybuf = NULL;
	size_t buflen;
	int pos = 0;

	if ( buf == NULL ) {
		(*obj)->status = PY_BUFNULL;
		return -1;
	}

	buflen = strlen (buf);
	pytmp = (char *) buf;

	/*
	 * replace python conflict charactors
	 */
	for ( pos=0; pos<buflen; pos++ ) {
		if ( pytmp[pos] == '\'' || pytmp[pos] == '"' || pytmp[pos] == '\r' || pytmp[pos] == '\n' )
			pytmp[pos] = ' ';
	}
	pos = 0;

	pybuf = (char *) emalloc (sizeof (char) * (20 + buflen + 1));

	if ( pybuf == NULL ) {
		(*obj)->status = PY_MEMLOC;
		return -1;
	}

	PyRun_SimpleString ("detector.reset()");

	sprintf (pybuf, "detector.feed(\"%s\")", pytmp);
	if ( PyRun_SimpleString (pybuf) == -1 ) {
		(*obj)->status = PY_DETECT_FAILURE;
		SAFE_EFREE(pybuf);
		return -1;
	}
	SAFE_EFREE(pybuf);

	PyRun_SimpleString ("detector.close()");
	if ( PyRun_SimpleString ("__retval__ = detector.result") == -1 ) {
		(*obj)->status = PY_DETECT_FAILURE;
		return -1;
	}

	pResult = PyDict_GetItemString (fp->pMainDictionary, "__retval__");
	Py_XINCREF (pResult);

	if ( pResult == NULL ) {
		(*obj)->status = PY_NO_RESULT;
		return -1;
	}

	while ( PyDict_Next (pResult, &pos, &key, &value) ) {
		switch (strcmp (PyString_AS_STRING (key), "encoding")) {
			case 0 :
				(*obj)->encoding = estrdup (PyString_AS_STRING (value));
				break;
			default :
				if ( PyInt_Check (value) )
					(*obj)->confidence = PyInt_AS_LONG (value) * 100;
				else
					(*obj)->confidence = PyFloat_AS_DOUBLE (value) * 100;
		}

		Py_DECREF (key);
		Py_DECREF (value);
	}

	return 0;
}
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */