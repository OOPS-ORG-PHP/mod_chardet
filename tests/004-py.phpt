--TEST--
Check for chardet py presence
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
if ( CHARDET_PY == -1 )
	print 'skip';
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
$py = chardet_detect ($fp, $strings[0], CHARDET_PY);
echo $py->encoding;
chardet_close ($fp);
?>
--EXPECT--
EUC-KR
