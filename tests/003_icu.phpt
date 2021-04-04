--TEST--
Check for chardet icu presence
--SKIPIF--
<?php
if ( ! extension_loaded ('chardet') ) {
	if ( version_compare(PHP_VERSION, "5.1.0", "<") ) {
		dl ('chardet.so');
		if ( ! extension_loaded ('chardet') )
			print 'skip';
	} else
		print 'skip';
}
?>
--POST--
--GET--
--INI--
--FILE--
<?php
if ( version_compare(PHP_VERSION, "5.1.0", "<") )
    dl ('chardet.so');

$strings = array (
	'�ȳ��ϼ��� abc�� ������, �����ٴ� �ѱ� �Դϴ�.',
	'�ȳ�',
	'�ȳ��ϼ���',
	'���ݴ� ��� ���� ���ϴ�. ��� ������ �ʿ��ұ��? ��ȣ��.. ���� �������',
);

$fp = chardet_open ();

if ( CHARDET_ICU != -1 )
	$icu = chardet_detect ($fp, $strings[0], CHARDET_ICU);

if ( CHARDET_ICU != -1 && $icu )
	printf ('chardet icu is available');

chardet_close ($fp);
?>
--EXPECT--
chardet icu is available
