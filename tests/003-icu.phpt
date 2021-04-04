--TEST--
Check for chardet icu presence
--SKIPIF--
<?php
if ( ! extension_loaded ('chardet') ) {
	print 'skip';
}
if ( CHARDET_ICU == -1 )
	print 'skip';
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
$icu = chardet_detect ($fp, $strings[0], CHARDET_ICU);
echo $icu->encoding;
chardet_close ($fp);
?>
--EXPECT--
EUC-KR
