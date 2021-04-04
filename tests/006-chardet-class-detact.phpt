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
