--TEST--
Check for chardet presence
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

if ( CHARDET_MOZ != -1 )
	$moz = chardet_detect ($fp, $strings[0]);

if ( CHARDET_MOZ != -1 && $moz )
	printf ('chardet moz is available');

chardet_close ($fp);
?>
--EXPECT--
chardet moz is available
