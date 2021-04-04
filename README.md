mod_chardet php extension
====
[![License: MPL 1.1](https://img.shields.io/badge/License-MPL%201.1-brightgreen.svg)](https://opensource.org/licenses/MPL-1.1)
[![GitHub closed issues](https://img.shields.io/github/issues-closed-raw/OOPS-ORG-PHP/mod_chardet.svg)](https://github.com/OOPS-ORG-PHP/mod_chardet/issues?q=is%3Aissue+is%3Aclosed)
[![GitHub closed pull requests](https://img.shields.io/github/issues-pr-closed-raw/OOPS-ORG-PHP/mod_chardet.svg)](https://github.com/OOPS-ORG-PHP/mod_chardet/pulls?q=is%3Apr+is%3Aclosed)

**This branch is for PHP 5 or less.
For use in PHP 7 or later, see the [Master branch](https://github.com/oops-org-php/mod_chardet).**

## License

&copy; 2021. JoungKyun.Kim all right reserved.

This program is under MPL 1.1 or GPL v2

## Abstract

Determine the charset of the input data with [Mozilla Universal Charset Detection C/C++ library](http://lxr.mozilla.org/seamonkey/source/extensions/universalchardet/)

This is php extension that is [libchardet](https://github.com/joubgkyun/libchardet) PHP frontend.

[libchardet](https://github.com/joubgkyun/libchardet) is based on [Mozilla Universal Charset Detection C/C++ library](http://lxr.mozilla.org/seamonkey/source/extensions/universalchardet/) and, detects the character set used to encode data.

This module is a c-binding, is much faster than the other chardet packages taht is made by PHP code.

mod_chardet extension supports three method for detecting charset. Supporting method and required library is as follow:

 * libchardet - Mozilla Universal Charset Detect C/C++ library
 * ICU - IBM International Components for Unicode
 * python-chardet - Mozilla Universal Charset Detect with pure python

For CJKV(Chinese, Japanese, Korean, Vitenams) languages, recommended to use MUCD(Mozilla Universal Charset Detect). This method is best. And, about single byte languages, MUCD and ICU all best. 

In the case of python-chardet mode, even use the MUCD. However, the call performance is very not good. The mode is support for test, so when if you don't give configure options, this mode does not work basically. 

For more informations, see also [Reference](https://github.com/OOPS-ORG-PHP/mod_chardet/blob/master/Reference) document.

## Downloads
 * https://github.com/OOPS-ORG-PHP/mod_chardet/releases
 * Inforamtion of latest version
  * For PHP 7 and after: master branch, latest version is [1.0.4](https://github.com/OOPS-ORG-PHP/mod_chardet/releases/tag/1.0.4)
  * For PHP 5 and before: v0 branch, latest version is [0.0.6](https://github.com/OOPS-ORG-PHP/mod_chardet/releases/tag/0.0.6)

## Installation

### 1. Requires

 * mod_chardet versions
   * PHP 7 and after : mod_chardet >= 1.0.0
   * PHP 5 ans before : mod_chardet < 1.0.0
 * PHP >= 4.3
 * [libchardet](https://github.com/joubgkyun/libchardet) >= 1.0.5
 * [libicu](http://site.icu-project.org/) (optional)
 * [python-chardet](https://pypi.python.org/pypi/chardet) (optional)


### 2. Build

First, check libraries about [libchardet](https://github.com/joubgkyun/libchardet), [libicu](http://site.icu-project.org/) and [python-chardet](https://pypi.python.org/pypi/chardet).

You must install one of [libchardet](https://github.com/joubgkyun/libchardet) or [libicu](http://site.icu-project.org/).

The function of [python-chardet](https://pypi.python.org/pypi/chardet) is for checking result with [python-chardet](https://pypi.python.org/pypi/chardet). The performance of this feature is not very good and we don't recommand to use this feature.

```bash
[root@host mod_chardet]$ phpize
[root@host mod_chardet]$ ./configure --help
  ...
  --enable-moz-chardet    Support Mozilla chardet [default=yes]
  --enable-icu-chardet    Support ICU chardet [default=yes]
  --enable-py-chardet     Support python chardet [default=no]
  ...
[root@host mod_chardet]$ ./configure
[root@host mod_chardet]$ make && make install
```

### 3. Configurations

add DSO extension config to your php.ini

```ini
extension = chardet.so
```

### Usages

See also sample script of repository.
 * https://github.com/OOPS-ORG-PHP/mod_chardet/blob/v0/sample.php
 * https://github.com/OOPS-ORG-PHP/mod_chardet/blob/v0/sample-oop.php

