--TEST--
Check for chardet classfy operation
--SKIPIF--
<?php
if ( ! extension_loaded ('chardet') ) {
	if ( version_compare(PHP_VERSION, '5.1.0', '<') ) {
		dl ('chardet.so');
		if ( ! extension_loaded ('chardet') )
			print 'skip';
	} else
		print 'skip';
}
# don't support class under PHP 5
if ( version_compare(PHP_VERSION, '5.0.0', '<') )
	print 'skip';
?>
--POST--
--GET--
--INI--
--FILE--
<?php
if ( version_compare(PHP_VERSION, '5.1.0', '<') )
    dl ('chardet.so');

$cd = new CHARDET;

$strings = array (
	'�ȳ��ϼ��� abc�� ������, �����ٴ� �ѱ� �Դϴ�.',
	'�ȳ�',
	'�ȳ��ϼ���',
	'���ݴ� ��� ���� ���ϴ�. ��� ������ �ʿ��ұ��? ��ȣ��.. ���� �������',
);

if ( CHARDET_MOZ == -1 )
	echo "EUC-KR\nEUC-KR\n";
else {
	$r = $cd->detect ($strings[3]);
	echo $r->encoding . "\n";
	$r = $cd->detect ($strings[3], CHARDET_MOZ);
	echo $r->encoding . "\n";
}

if ( CHARDET_ICU == -1 )
	echo "EUC-KR\n";
else {
	$r = $cd->detect ($strings[3], CHARDET_ICU);
	echo $r->encoding . "\n";
}

if ( CHARDET_PY == -1 )
	echo "EUC-KR\n";
else {
	$r = $cd->detect ($strings[3], CHARDET_PY);
	echo $r->encoding;
}

?>
--EXPECT--
EUC-KR
EUC-KR
EUC-KR
EUC-KR
