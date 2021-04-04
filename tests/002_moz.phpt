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
	'안녕하세요 abc는 영어고요, 가나다는 한글 입니다.',
	'안녕',
	'안녕하세요',
	'조금더 길게 적어 봅니다. 어느 정도가 필요할까요? 오호라.. 점점 길어지네',
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
