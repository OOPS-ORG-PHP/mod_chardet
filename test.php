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
	$icu = chardet_detect ($fp, $s);
	if ( CHARDET_PY != -2 )
		$moz = chardet_detect ($fp, $s, CHARDET_PY);

	echo "$s\n";
	echo "  ICU : Encoding -> " . $icu->encoding . " Confidence -> " . $icu->confidence . "\n";
	if ( CHARDET_PY != -2 )
		echo "  PY  : Encoding -> " . $moz->encoding . " Confidence -> " . $moz->confidence . "\n";
	echo "\n";

	$i++;
}

chardet_close ($fp);
?>
