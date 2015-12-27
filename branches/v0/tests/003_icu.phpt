--TEST--
Check for chardet icu presence
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
