/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2006 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: JoungKyun.Kim <http://oops.org>                              |
  +----------------------------------------------------------------------+

  $Id: php_chardet.c,v 1.6 2009-02-23 03:52:04 oops Exp $
*/

/*
 * PHP5 Charaset Detect library module "chardet"
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <unistd.h>

#include <unicode/ucsdet.h>

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

/* {{{ chardet_functions[]
 *
 * Every user visible function must have an entry in chardet_functions[].
 */
function_entry chardet_functions[] = {
	PHP_FE(chardet_version,			NULL)
	PHP_FE(chardet_icu_version,		NULL)
#ifdef HAVE_PY_CHARDET
	PHP_FE(chardet_py_version,		NULL)
#endif
	PHP_FE(chardet_open,			NULL)
	PHP_FE(chardet_detect,			NULL)
	PHP_FE(chardet_close,			NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ chardet_module_entry
 */
zend_module_entry chardet_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
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

	if ( fp->csd_status != 0 ) {
		ucsdet_close (fp->csd);
		fp->csd_status = 0;
	}

#ifdef HAVE_PY_CHARDET
	fp->pMainModule = NULL;
	fp->pMainDictionary = NULL;
#endif

	SAFE_EFREE(fp);
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(chardet)
{
	le_chardet = zend_register_list_destructors_ex (_close_chardet_link, NULL, "Chardet link", module_number);
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
	php_info_print_table_row(2, "Summary",
#ifdef HAVE_PY_CHARDET
								"Charset detect with ICU/Mozilla library"
#else
								"Charset detect with ICU library"
#endif
	);
	php_info_print_table_row(2, "URL", "http://devel.oops.org/");
	php_info_print_table_row(2, "Build version", CHARDET_VERSION);
	php_info_print_table_row(2, "ICU Library version", U_ICU_VERSION);
#ifdef HAVE_PY_CHARDET
	php_info_print_table_row(2, "Python Chardet Support", PY_CHARDET_VERSION);
#endif
	php_info_print_table_end();
}
/* }}} */

/* {{{ proto char chardet_buildver (void)
 *  print chardet extension build number */
PHP_FUNCTION(chardet_version)
{
	RETURN_STRING (CHARDET_VERSION, 1);
}
/* }}} */

/* {{{ proto char chardet_icu_buildver (void)
 *  print chardet icu library version */
PHP_FUNCTION(chardet_icu_version)
{
	RETURN_STRING (U_ICU_VERSION, 1);
}
/* }}} */

#ifdef HAVE_PY_CHARDET
/* {{{ proto char chardet_icu_buildver (void)
 *  print chardet icu library version */
PHP_FUNCTION(chardet_py_version)
{
	RETURN_STRING (PY_CHARDET_VERSION, 1);
}
/* }}} */
#endif

/* {{{ proto resource chardet_open (void)
 */
PHP_FUNCTION(chardet_open)
{
	CharDetFP * fp = NULL;
	UErrorCode status = U_ZERO_ERROR;

	if ( (fp = (CharDetFP *) emalloc (sizeof (CharDetFP))) == NULL )
		RETURN_FALSE;

	fp->csd = ucsdet_open (&status);

	if ( status != U_ZERO_ERROR )
		RETURN_FALSE;

	fp->csd_status = 1;

#ifdef HAVE_PY_CHARDET
	if ( (fp->pMainModule = PyImport_AddModule ("__main__")) == NULL ) {
		chardet_fp_free (&fp);
		RETURN_FALSE;
	}

	fp->pMainDictionary = PyModule_GetDict (fp->pMainModule);
	if ( PyRun_SimpleString ("import chardet.universaldetector\n") == -1 ) {
		php_error (E_WARNING, "Load failed chardet module");
		chardet_fp_free (&fp);
		RETURN_FALSE;
	}

	if ( PyRun_SimpleString ("detector = chardet.universaldetector.UniversalDetector()\n") == -1 ) {
		chardet_fp_free (&fp);
		RETURN_FALSE;
	}
#endif

	ZEND_REGISTER_RESOURCE (return_value, fp, le_chardet);
}
/* }}} */

/* {{{ proto void chardet_close (resource)
 */
PHP_FUNCTION(chardet_close)
{
	zval ** fp_link;
	CharDetFP * fp;

	switch (ZEND_NUM_ARGS ()) {
		case 1:
			if ( zend_get_parameters_ex (1, &fp_link) == FAILURE )
				WRONG_PARAM_COUNT;
			break;
		default :
				WRONG_PARAM_COUNT;
	}

	ZEND_FETCH_RESOURCE (fp, CharDetFP *, fp_link, -1, "Chardet link", le_chardet);
	zend_list_delete (Z_RESVAL_PP (fp_link));
}

/* {{{ proto char chardet (resource, string, type)
 *  resouce : stream
 *  string : string that checked encoding
 *  type   : CHARDET_ICU or CHARDET_MOZ or CHARDET_PY
 *  return object result
 *     -> encoding
 *     -> lang
 *     -> confidence
 *     -> status (if value is not 0, false)
 */
PHP_FUNCTION(chardet_detect)
{
	zval ** fp_link, ** buf, ** mode;
	CharDetFP  * fp;
	CharDetObj * obj    = NULL;
	const char * string = NULL;
	short type          = CHARDET_ICU;
	short r             = 0;

	switch (ZEND_NUM_ARGS ()) {
		case 3:
			if ( zend_get_parameters_ex (3, &fp_link, &buf, &mode) == FAILURE )
				WRONG_PARAM_COUNT;

			convert_to_long_ex (mode);
			type = Z_LVAL_PP (mode);
			break;
		case 2:
			if ( zend_get_parameters_ex (2, &fp_link, &buf) == FAILURE )
				WRONG_PARAM_COUNT;
			break;
		default :
			WRONG_PARAM_COUNT;
	}

	ZEND_FETCH_RESOURCE (fp, CharDetFP *, fp_link, -1, "Chardet link", le_chardet);

	convert_to_string_ex (buf);
	string = Z_STRVAL_PP (buf);

	if ( chardet_obj_init (&obj) < 0 ) {
		php_error (E_ERROR, "Structure initialize failed on chardet ()");
		RETURN_FALSE;
	}

	switch (type) {
		case CHARDET_ICU :
			r = icu_chardet (fp, string , &obj);
			break;
#ifdef HAVE_PY_CHARDET
		case CHARDET_PY :
			r = py_chardet (fp, string, &obj);
			break;
#endif
		default :
			chardet_obj_free (&obj);
#ifdef HAVE_PY_CHARDET
			php_error (E_ERROR, "Unknown TYPE argument 3 on chardet (). Use CHARDET_ICU or CHARDET_PY");
#else
			if ( mode == CHARDET_PY )
				php_error (E_ERROR, "Don't support CHARDET_PY type in this compile time.");
			else
				php_error (E_ERROR, "Unknown TYPE argument 3 on chardet (). Use CHARDET_ICU");
#endif
			RETURN_FALSE;
	}

	object_init (return_value);
	add_property_string (return_value, "encoding", obj->encoding ? obj->encoding : "", 1);
	add_property_long (return_value, "confidence", obj->confidence);
	add_property_long (return_value, "status", obj->status);
	if ( type == CHARDET_ICU )
		add_property_string (return_value, "lang", obj->lang ? obj->lang : "", 1);

	chardet_obj_free (&obj);
}
/* }}} */

void chardet_fp_free (CharDetFP ** fp) {
		if ( (*fp)->csd_status )
			ucsdet_close ((*fp)->csd);
		(*fp)->csd_status = 0;
}

short chardet_obj_init (CharDetObj ** obj) {
	*obj = (CharDetObj *) emalloc (sizeof (CharDetObj));

	if ( *obj == NULL )
		return -1;

	(*obj)->encoding = NULL;
	(*obj)->lang = NULL;
	(*obj)->confidence = 0;
	(*obj)->status = U_ZERO_ERROR;

	return 0;
}

void chardet_obj_free (CharDetObj ** obj) {
	if ( *obj != NULL ) {
		SAFE_EFREE((*obj)->encoding)
		SAFE_EFREE((*obj)->lang)
		SAFE_EFREE(*obj)
	}
}

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
