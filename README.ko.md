mod_chardet php extension
====

## License

&copy; 2021. JoungKyun.Kim all right reserved.

이 프로그램은 MPL 1.1 또는 GLP v2의 영향을 받습니다.

## 개요

mod_chardet 확장은 [Mozilla Universal Charset Detection C/C++ library](http://lxr.mozilla.org/seamonkey/source/extensions/universalchardet/) 알고리즘을 이용하여 입력된 데이터의 문자셋을 결정 합니다.

이 확장 프로그램은 [libchardet](https://github.com/joubgkyun/libchardet)의 PHP frontend 입니다.

[libchardet](https://github.com/joubgkyun/libchardet)은 [Mozilla Universal Charset Detection C/C++ library](http://lxr.mozilla.org/seamonkey/source/extensions/universalchardet/) 알고리즘을 기반으로 데이터의 문자셋을 탐지 합니다.

이 확장 프로그램은 C-binding으로 제작이 되어, PHP code로 작성된 다른 chardet 패키지들 보다 훨씬 빠르고 정확한 탐지를 합니다.

mod_chardet extension supports three method for detecting charset. Supporting method and required library is as follow:
mod_chardet 확장은 문자셋을 탐지하기 위하여 3가지 알고리즘을 사용할 수 있습니다. 이 3가지 알고리즘은 다음의 library에 의해서 지원이 됩니다.

 * libchardet - Mozilla Universal Charset Detect C/C++ library
 * ICU - IBM International Components for Unicode
 * python-chardet - Mozilla Universal Charset Detect with pure python

CJKV(Chinese, Japanese, Korean, Vitenams) 언어의 문자셋 탐지를 위해서라면, Mozilla Universal Charset Detect 알고리즘을 사용하는 것을 권장 합니다. ICU 라이브러리 보다 정확도가 훨씬 좋습니다.

python-chardet 모드는 python chardet 모듈의 결과와 비교를 하기 위한 디버깅 목적의 기능 입니다. 또한 성능이 매우 좋지 않기 때문에 사용하는 것을 권장하지 않습니다. (빌드시에 기본으로 포함되지 않습니다.)

이 확장의 기능에 대한 더 많은 정보는 [Reference](https://github.com/OOPS-ORG-PHP/mod_chardet/blob/master/Reference) 문서를 참고 하십시오.

## 다운로드
 * https://github.com/OOPS-ORG-PHP/mod_chardet/releases
 * Inforamtion of latest version
  * PHP 7 이후: [1.0.2](https://github.com/OOPS-ORG-PHP/mod_chardet/releases/tag/1.0.2)
  * PHP 7 이전: [0.0.5](https://github.com/OOPS-ORG-PHP/mod_chardet/releases/tag/0.0.5)
  * 두 버전간의 기능적 차이는 없습니다. 모듈 구조의 변경 때문에 branch가 나뉘게 되었습니다.

## 설치

### 1. 요구사항

 * mod_chardet 버전
   * PHP 7 and after : mod_chardet >= 1.0.0
   * PHP 5 ans before : mod_chardet < 1.0.0
 * PHP >= 4.1
 * [libchardet](https://github.com/joubgkyun/libchardet) >= 1.0.5
 * [libicu](http://site.icu-project.org/) (optional)
 * [python-chardet](https://pypi.python.org/pypi/chardet) (optional)


### 2. 빌드

먼저 사용할 알고리즘에 대한 다음의 라이브러리들이 시스템에 설치되어 있어야 합니다.

 * [libchardet](https://github.com/joubgkyun/libchardet)
 * [libicu](http://site.icu-project.org/)
 * [python-chardet](https://pypi.python.org/pypi/chardet).

mod_chardet 확장을 사용하기 위해서는 [libchardet](https://github.com/joubgkyun/libchardet) 또는 [libicu](http://site.icu-project.org/) 중 하나가 설치 되어 있어야 합니다.

python-chardet 모드는 python chardet 모듈의 결과와 비교를 하기 위한 디버깅 목적의 기능 입니다. 또한 성능이 매우 좋지 않기 때문에 사용하는 것을 권장하지 않습니다. (빌드시에 기본으로 포함되지 않습니다.)

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

### 3. 설정

php.ini에 mod_chardet 모듈 로딩 설정을 다음과 같이 추가해 주십시오.

```ini
extension = chardet.so
```

### 사용법

Repository의 샘플 스크립트를 참조 하십시오.
 * https://github.com/OOPS-ORG-PHP/mod_chardet/blob/master/sample.php
 * https://github.com/OOPS-ORG-PHP/mod_chardet/blob/master/sample-oop.php
