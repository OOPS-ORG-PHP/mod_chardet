--TEST--
Check for chardet presence
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

if ( CHARDET_MOZ != -1 )
	$moz = chardet_detect ($fp, $strings[0]);

if ( CHARDET_MOZ != -1 && $moz )
	printf ('chardet moz is available');

chardet_close ($fp);
?>
--EXPECT--
chardet moz is available
