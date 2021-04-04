/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 49912d9b18ac4f8a44c45ee3a5a57f9c989c5ded */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_chardet_version, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_chardet_moz_version arginfo_chardet_version

#define arginfo_chardet_icu_version arginfo_chardet_version

#define arginfo_chardet_py_version arginfo_chardet_version

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_chardet_open, 0, 0, CHARDET, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_chardet_close, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, link, CHARDET, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_chardet_detect, 0, 2, MAY_BE_OBJECT|MAY_BE_FALSE)
	ZEND_ARG_OBJ_INFO(0, link, CHARDET, 0)
	ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, type, IS_LONG, 0, "CHARDET_MOZ")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_CHARDET___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_CHARDET_close, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_CHARDET_detect, 0, 1, MAY_BE_OBJECT|MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, type, IS_LONG, 0, "CHARDET_MOZ")
ZEND_END_ARG_INFO()
