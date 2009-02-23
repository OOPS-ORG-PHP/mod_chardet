#!/usr/bin/php
<?
if ( ! extension_loaded ('chardet') )
	dl ('chardet.so');

$strings = array (
	'�ȳ��ϼ��� abc�� ������, �����ٴ� �ѱ� �Դϴ�.',
	'�ȳ�',
	'�ȳ��ϼ���',
	'���ݴ� ��� ���� ���ϴ�. ��� ������ �ʿ��ұ��? ��ȣ��.. ���� �������',
);

$fp = chardet_open ();

$i=0;
foreach ( $strings as $s ) {
	#
	# proto object chardet_detect (database handle, string[, mode])
	#   database handle : return value of chardet_open () API
	#   string          : strings for character set detecting
	#   mode            : chardet library mode (CHARDET_ICU or CHARDET_MZO)
	#                     default value is CHARDET_ICU
	#
	#                     CHARDET_ICU -> detect with ICU library (default mode)
	#                     CHARDET_PY  -> detect with Mozilla Universal Chardet library
	#                                    with Python C API (use python chardet)
	#
	#                     If you give Python chardet option on buildtime, CHARDET_PY
	#                     value is set 1. If value of CHARDET_PY is -2, chardet
	#                     extension don't support CHARDET_PY mode
	#
	#  return value type : object -> encoding    : detecting charset
	#                                langs       : charset language name (don't support
	#                                              CHARDET_PY mode)
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
