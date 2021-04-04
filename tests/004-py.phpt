--TEST--
Check for chardet py presence
--SKIPIF--
<?php
if ( ! extension_loaded ('chardet') ) {
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
$strings = array (
	'안녕하세요 abc는 영어고요, 가나다는 한글 입니다.',
	'안녕',
	'안녕하세요',
	'조금더 길게 적어 봅니다. 어느 정도가 필요할까요? 오호라.. 점점 길어지네',
);

$fp = chardet_open ();
$py = chardet_detect ($fp, $strings[0], CHARDET_PY);
echo $py->encoding;
chardet_close ($fp);
?>
--EXPECT--
EUC-KR
