--TEST--
Check for chardet_version
--SKIPIF--
<?php
if ( ! extension_loaded ('chardet') ) {
	print 'skip';
}
?>
--POST--
--GET--
--INI--
--FILE--
<?php
if ( chardet_version () == CHARDET_VERSION )
	echo "yes\n";

if ( CHARDET_MOZ == -1 )
	echo "yes\n";
else if ( chardet_moz_version () == CHARDET_MOZ_VERSION )
	echo "yes\n";
else
	echo "no\n";

if ( CHARDET_ICU == -1 )
	echo "yes\n";
else if ( chardet_icu_version () == CHARDET_ICU_VERSION )
	echo "yes\n";
else
	echo "no\n";

if ( CHARDET_PY == -1 )
	echo "yes";
else if ( chardet_py_version () == CHARDET_PY_VERSION )
	echo "yes";
else
	echo "no";
?>
--EXPECT--
yes
yes
yes
yes
