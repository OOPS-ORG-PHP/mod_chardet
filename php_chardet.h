/*
 * Copyright 2022. JoungKyun.Kim All rights reserved
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


#ifndef PHP_CHARDET_H
#define PHP_CHARDET_H

#ifdef PHP_WIN32
#define PHP_CHARDET_API __declspec(dllexport)
#else
#define PHP_CHARDET_API
#endif

#include "TSRM.h"

#if PHP_VERSION_ID >= 80000
#define TSRMLS_DC
#define TSRMLS_CC
#endif

#if PHP_VERSION_ID < 80000
#  define ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(a,b,c,d) ZEND_BEGIN_ARG_INFO_EX(a,0,b,c)
#  define ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(a,b,c,d,e) ZEND_ARG_TYPE_INFO(a,b,c,d)
#  ifdef ZEND_ARG_OBJ_INFO
#    undef ZEND_ARG_OBJ_INFO
#  endif
#  define ZEND_ARG_OBJ_INFO(a,b,c,d) ZEND_ARG_TYPE_INFO(a,b,IS_RESOURCE,d)
#endif

#if PHP_VERSION_ID < 70200
#  ifdef ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX
#    undef ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX
#  endif
#  define ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null) \
		static const zend_internal_arg_info name[] = { \
			{ (const char*)(zend_uintptr_t)(required_num_args), NULL, type, return_reference, allow_null, 0 },
#  define ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(a,b,c,d,e) ZEND_BEGIN_ARG_INFO_EX(a,0,b,c)
#endif


extern zend_module_entry chardet_module_entry;
#define chardet_module_ptr &chardet_module_entry

PHP_MINIT_FUNCTION(chardet);
PHP_MSHUTDOWN_FUNCTION(chardet);
PHP_RINIT_FUNCTION(chardet);
PHP_RSHUTDOWN_FUNCTION(chardet);
PHP_MINFO_FUNCTION(chardet);

PHP_FUNCTION(chardet_version);
#ifdef HAVE_MOZ_CHARDET
PHP_FUNCTION(chardet_moz_version);
#endif
#ifdef HAVE_ICU_CHARDET
PHP_FUNCTION(chardet_icu_version);
#endif
#ifdef HAVE_PY_CHARDET
PHP_FUNCTION(chardet_py_version);
#endif
PHP_FUNCTION(chardet_open);
PHP_FUNCTION(chardet_detect);
PHP_FUNCTION(chardet_close);

/* 
   Declare any global variables you may need between the BEGIN
   and END macros here:     

   ZEND_BEGIN_MODULE_GLOBALS(chardet)
   int   global_value;
   char * global_string;
   ZEND_END_MODULE_GLOBALS(chardet)
 */

/* In every utility function you add that needs to use variables 
   in php_nis_globals, call TSRM_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMG_CC
   after the last function argument and declare your utility function
   with TSRMG_DC after the last declared argument.  Always refer to
   the globals in your function as NIS_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
 */

#ifdef ZTS
#define CHARDET_G(v) TSRMG(chardet_globals_id, zend_chardet_globals *, v)
#else
#define CHARDET_G(v) (chardet_globals.v)
#endif

#define phpext_chardet_ptr chardet_module_ptr

typedef struct CharDet_FP {
	// for Class
	zend_resource * rsrc;
#ifdef HAVE_MOZ_CHARDET
	Detect *moz;
	short moz_status;
#endif
#ifdef HAVE_ICU_CHARDET
	UCharsetDetector * csd;
	short csd_status;
#endif
#ifdef HAVE_PY_CHARDET
	PyObject * pMainDictionary;
	PyObject * pMainModule;
#endif
} CharDetFP;

#ifndef HAVE_ICU_CHARDET
#define UErrorCode short
#endif

typedef struct CharDet_Obj {
	char * encoding;
	char * lang;
	int32_t confidence;
	UErrorCode status;
} CharDetObj;

#ifdef HAVE_MOZ_CHARDET
#define CHARDET_MOZ 0
#else
#define CHARDET_MOZ -1
#endif
#define CASE_CHARDET_MOZ 0

#ifdef HAVE_ICU_CHARDET
#define CHARDET_ICU 1
#else
#define CHARDET_ICU -1
#endif
#define CASE_CHARDET_ICU 1

#ifdef HAVE_PY_CHARDET
#define CHARDET_PY 2
#else
#define CHARDET_PY -1
#endif
#define CASE_CHARDET_PY 2

#ifdef HAVE_PY_CHARDET
#define PY_BUFNULL			1	// Checking String is NULL
#define PY_MOD_MAINLOAD		2	// Failed load __main__ module
#define PY_MOD_CHARDETLOAD	3	// Failed load chardet module
#define PY_MEMLOC			4	// memory allocation failed
#define PY_DETECT_FAILURE	5	// failed detect string encoding
#define PY_NO_RESULT		6	// can't find __retval__ key on dictionary
#endif

void chardet_fp_free (CharDetFP **);
short chardet_obj_init (CharDetObj **);
void chardet_obj_free (CharDetObj **);
#ifdef HAVE_MOZ_CHARDET
short moz_chardet (CharDetFP *, zend_string *, CharDetObj **);
#endif
#ifdef HAVE_ICU_CHARDET
short icu_chardet (CharDetFP *, zend_string *, CharDetObj **);
#endif
#ifdef HAVE_PY_CHARDET
short py_chardet (CharDetFP *, zend_string *, CharDetObj **);
#endif

#ifndef SAFE_EFREE
#define SAFE_EFREE(p) do { if(p) { efree(p); (p) = NULL; } } while(0)
#endif

#define CHARDET_FETCH_RESOURCE(a,b,c,d,e) \
	if ( (a = (b) zend_fetch_resource_ex (c, d, e)) == NULL ) \
		RETURN_FALSE

#endif	/* PHP_CHARDET_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
