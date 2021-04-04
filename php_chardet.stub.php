<?php
/** @--generate-function-entries **/

function chardet_version (): string { }
function chardet_moz_version (): string { }
function chardet_icu_version (): string { }
function chardet_py_version (): string { }
function chardet_open (): CHARDET { }
function chardet_close (CHARDET $link): bool { }
function chardet_detect (CHARDET $link, string $buffer, int $type = CHARDET_MOZ): object|false { }

Class CHARDET {
	/*
	 * @return CHARDET
	 * @alias chardet_open
	 */
	public function __construct () { }
	/*
	 * @return bool
	 * @alias chardet_close
	 */
	public function close (): bool { }
	/*
	 * @return object|false
	 * @alias chardet_detect
	 */
	public function detect (string $buffer, int $type = CHARDET_MOZ): object|false { }
}
