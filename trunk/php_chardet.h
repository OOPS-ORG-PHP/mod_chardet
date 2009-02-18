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

   $Id: php_chardet.h,v 1.1.1.1 2009-02-18 15:02:28 oops Exp $
 */

#ifndef PHP_CHARDET_H
#define PHP_CHARDET_H

#ifdef PHP_WIN32
#define PHP_CHARDET_API __declspec(dllexport)
#else
#define PHP_CHARDET_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

extern zend_module_entry chardet_module_entry;
#define chardet_module_ptr &chardet_module_entry

PHP_MINIT_FUNCTION(chardet);
PHP_MSHUTDOWN_FUNCTION(chardet);
PHP_RINIT_FUNCTION(chardet);
PHP_RSHUTDOWN_FUNCTION(chardet);
PHP_MINFO_FUNCTION(chardet);

PHP_FUNCTION(chardet_version);
PHP_FUNCTION(chardet_icu_version);
#ifdef HAVE_MOZ_CHARDET
PHP_FUNCTION(chardet_moz_version);
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
	UCharsetDetector * csd;
	short csd_status;
#ifdef HAVE_MOZ_CHARDET
	PyObject * pMainDictionary;
	PyObject * pMainModule;
#endif
} CharDetFP;

typedef struct CharDet_Obj {
	char * encoding;
	char * lang;
	int32_t confidence;
	UErrorCode status;
} CharDetObj;

#define CHARDET_ICU 0
#ifdef HAVE_MOZ_CHARDET
#define CHARDET_MOZ 1
#else
#define CHARDET_MOZ -1
#endif

#ifdef HAVE_MOZ_CHARDET
#define MOZ_BUFNULL			1	// Checking String is NULL
#define MOZ_MOD_MAINLOAD	2	// Failed load __main__ module
#define MOZ_MOD_CHARDETLOAD	3	// Failed load chardet module
#define MOZ_MEMLOC			4	// memory allocation failed
#define MOZ_DETECT_FAILURE	5	// failed detect string encoding
#define MOZ_NO_RESULT		6	// can't find __retval__ key on dictionary
#endif

void chardet_fp_free (CharDetFP **);
short chardet_obj_init (CharDetObj **);
void chardet_obj_free (CharDetObj **);
short icu_chardet (CharDetFP *, const char *, CharDetObj **);
short moz_chardet (CharDetFP *, const char *, CharDetObj **);

#endif	/* PHP_CHARDET_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
