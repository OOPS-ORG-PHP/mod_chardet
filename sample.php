#!/usr/bin/php
<?
if ( ! extension_loaded ('chardet') ) {
	if ( PHP_VERSION_ID > 50399 ) {
		fprintf (STDERR, "Chardet module not loaded!\n");
		exit (1);
	} else {
		if ( ini_get ('enable_dl') )
			dl ('chardet.so');
	}
}

$strings = array (
	'안녕하세요 abc는 영어고요, 가나다는 한글 입니다.',
	'안녕',
	'안녕하세요',
	'조금더 길게 적어 봅니다. 어느 정도가 필요할까요? 오호라.. 점점 길어지네',
);

$fp = chardet_open ();

$i=0;
foreach ( $strings as $s ) {
	#
	# proto object chardet_detect (stream handle, string[, mode])
	#   database handle : return value of chardet_open () API
	#   string          : strings for character set detecting
	#   mode            : optional
	#                     if support CHARDTE_MOZ, this value is
	#                     default, and don't support CHARDET_MOZ,
	#                     CHARDET_ICU is default.
	#
	#                     CHARDET_MOZ : libchardet library result
	#                     CHARDET_ICU : icu library result
	#                     CHARDET_PY  : python-chardet result
	#
	#                     if each CHARDET_(MOZ|ICU|PY) value is -1,
	#                     it means don't support each mode.
	#
	#  return value type : object -> encoding    : detecting charset name
	#                                langs       : charset language name
	#                                              Don't support on CHARDET_MOZ and CAHRDET_PY mode
	#                                confidence  : detecting confidence
	#                                status      : error code (0 is not error)
	#
	if ( CHARDET_MOZ != -1 )
		$moz = chardet_detect ($fp, $s);
	if ( CHARDET_ICU != -1 )
		$icu = chardet_detect ($fp, $s, CHARDET_ICU);
	if ( CHARDET_PY != -1 )
		$py = chardet_detect ($fp, $s, CHARDET_PY);

	echo "$s\n";
	if ( CHARDET_MOZ != -1 )
		printf ("MOZ : Encoding -> %-12s, Confidence -> %3d, Status -> %d\n",
				$moz->encoding, $moz->confidence, $moz->status);
	if ( CHARDET_ICU != -1 )
		printf ("ICU : Encoding -> %-12s, Confidence -> %3d, Status -> %d\n",
				$icu->encoding, $icu->confidence, $icu->status);
	if ( CHARDET_PY != -1 )
		printf ("PY  : Encoding -> %-12s, Confidence -> %3d, Status -> %d\n",
				$py->encoding, $py->confidence, $py->status);
	echo "\n";

	$i++;
}

chardet_close ($fp);
?>
