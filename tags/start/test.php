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
	$icu = chardet_detect ($fp, $s);
	#
	# If you give Python chardet option on buildtime,
	# CHARDET_MOZ value set 1. If value of CHARDET_MOZ is 1,
	# chardet extension don't support CHARDET_MOZ mode
	#
	if ( CHARDET_MOZ != -1 )
		$moz = chardet_detect ($fp, $s, CHARDET_MOZ);

	echo "$s\n";
	echo "  ICU : Encoding -> " . $icu->encoding . " Confidence -> " . $icu->confidence . "\n";
	if ( CHARDET_MOZ != -1 )
		echo "  MOZ : Encoding -> " . $moz->encoding . " Confidence -> " . $moz->confidence . "\n";
	echo "\n";

	$i++;
}

chardet_close ($fp);
?>
