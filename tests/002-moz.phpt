--TEST--
Check for chardet presence
--SKIPIF--
<?php
if ( ! extension_loaded ('chardet') ) {
	print 'skip';
}
if ( CHARDET_MOZ == -1 )
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
$moz = chardet_detect ($fp, $strings[0]);
echo $moz->encoding . "\n";
$moz = chardet_detect ($fp, $strings[0], CHARDET_ICU);
echo $moz->encoding;
chardet_close ($fp);
?>
--EXPECT--
EUC-KR
EUC-KR
