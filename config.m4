dnl $Id$
dnl config.m4 for extension chardet 

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(chardet, for charset detect support,
[  --with-chardet          Include charset detect support])

if test "x$PHP_EXECUTABLE" = "xNONE"; then
    PHP_EXECUTABLE="/usr/bin/php"
fi

if test "$PHP_CHARDET" != "no"; then
	PHP_ARG_ENABLE(moz-chardet, for Mozilla chardet support,
	[  --enable-moz-chardet    Support Mozilla chardet [[default=yes]]], [no])

	PHP_ARG_ENABLE(icu-chardet, for ICU chardet support,
	[  --enable-icu-chardet    Support ICU chardet [[default=yes]]], [no])

	PHP_ARG_ENABLE(py-chardet, for Python chardet support,
	[  --enable-py-chardet     Support python chardet [[default=no]]], [no], [no])

	AC_DEFINE(HAVE_CHARDET,1,[Support CHARDET Extension])
	AC_DEFINE_UNQUOTED(CHARDET_VERSION, "1.0.2", [Chardet extension version])

	PHP_SUBST(LDFLAGS)
	PHP_SUBST(CPPFLAGS)

	if test "$PHP_MOZ_CHARDET" = "no" -a "$PHP_ICU_CHARDET" = "no"; then
		AC_MSG_ERROR([mod_chardet is needed --enable-moz-chardet or --enable-icu-chardet.])
	fi

	if test "$PHP_MOZ_CHARDET" != "no"; then
		AC_MSG_CHECKING(the chardet-config path)
		MOZCONFIG=
		if test -f "$PHP_MOZ_CHARDET"; then
			MOZCONFIG=$PHP_MOZ_CHARDET
		else
			for i in /bin /sbin /usr/bin /usr/sbin /usr/local/bin /usr/local/sbin \
					/opt/bin /opt/sbin /opt/chardet/bin /opt/chardet/sbin \
					/opt/libchardet/bin /opt/libchardet/sbin \
					/usr/local/libchardet/bin /usr/local/libchardet/sbin \
					/usr/local/chardet/bin /usr/local/chardet/sbin \
					/usr/local/bin/libchardet/bin /usr/local/bin/libchardet/sbin \
					/usr/local/bin/chardet/bin /usr/local/bin/chardet/sbin
			do
				if test -f "$i/chardet-config"; then
					MOZCONFIG="$i/chardet-config"
					break
				fi
			done
		fi

		if test -n "$MOZCONFIG" ; then
			AC_MSG_RESULT([$MOZCONFIG])

			MOZ_VERSION=$($MOZCONFIG --version)
			MOZ_LIBS=$($MOZCONFIG --libs)
			MOZ_LIBDIR=
			CPPFLAGS="$CPPFLAGS $($MOZCONFIG --defs)"

			AC_MSG_CHECKING(MOZ version)
			AC_MSG_RESULT([$MOZ_VERSION])
			AC_DEFINE(HAVE_MOZ_CHARDET,1,[Mozilla Chardet support])

			OLD_LDFLAGS="${LDFLAGS}"
			LDFLAGS="${MOZ_LIBS} ${LDFLAGS}"
			AC_CHECK_LIB(chardet, detect_handledata_r, AC_DEFINE(ENABLE_DETECT_HANDLEDATA_R,1,[ENABLE DETCET_R API]), [])
			LDFLAGS="${OLD_LDFLAGS}"
		else
			AC_MSG_NOTICE([Can't find. specify --enable-moz-chardet=/path/chardet-config])
		fi
	fi

	if test "$PHP_ICU_CHARDET" != "no"; then
		AC_MSG_CHECKING(the icu-config path)
		ICUCONFIG=
		if test -f "$PHP_ICU_CHARDET"; then
			ICUCONFIG=$PHP_ICU_CHARDET
		else
			for i in /bin /sbin /usr/bin /usr/sbin /usr/local/bin /usr/local/sbin \
					/opt/bin /opt/sbin /opt/icu/bin /opt/icu/sbin \
					/usr/local/icu/bin /usr/local/icu/sbin \
					/usr/local/bin/icu/bin /usr/local/bin/icu/sbin
			do
				if test -f "$i/icu-config"; then
					ICUCONFIG="$i/icu-config"
					break
				fi
			done
		fi

		if test -n "$ICUCONFIG" ; then
			AC_MSG_RESULT([$ICUCONFIG])

			ICU_VERSION=$($ICUCONFIG --version)
			ICU_LIBS=$($ICUCONFIG --ldflags)
			ICU_LIBDIR=$($ICUCONFIG --ldflags-searchpath)
			CPPFLAGS="$CPPFLAGS $($ICUCONFIG --cppflags)"

			AC_MSG_CHECKING(ICU version)
			AC_MSG_RESULT([$ICU_VERSION])
			AC_DEFINE(HAVE_ICU_CHARDET,1,[ICU Chardet support])
		else
			pkg-config icu > /dev/null 2>&1
			if test $? -eq 0; then
				ICU_VERSION=$(pkg-config --modversion icu)
				ICU_LIBS=$(pkg-config --libs icu)
				ICU_LIBDIR=$(pkg-config --cflags-only-other icu)
				CPPFLAGS="$CPPFLAGS $(pkgconfig --cflags icu)"
			else
				if test -z "$MOZ_VERSION" ; then
					AC_MSG_NOTICE([can't find. specify --enable-icu-chardet=/path/icu-config])
					AC_MSG_ERROR([mod_chardet is needed libicu or libchardet.])
				else
					AC_MSG_NOTICE([can't find. specify --enable-icu-chardet=/path/icu-config])
				fi
			fi
		fi
	fi

	dnl Checks for header files
	dnl AC_HEADER_STDC
	dnl AC_CHECK_HEADERS(unistd.h)

	dnl
	dnl Python Chardet support
	dnl

	dnl
	dnl Maybe It has several problems. So, prevent building current:
	dnl PHP_PY_CHARDET="no"
	dnl
	if test "$PHP_PY_CHARDET" != "no"; then
		dnl Check Python version
		if test -f "$PHP_PY_CHARDET"; then
			PYEXEC=$PHP_PY_CHARDET
		fi

		if test -z "$PYEXEC"; then
			PYPATH="$PATH:/usr/sbin:/usr/local/bin:/usr/local/sbin"
			PYPATH="$PYPATH:/usr/python/bin:/usr/local/python/bin:/opt/python/bin"
			AC_PATH_PROG([PYEXEC], [python], , [$PYPATH])
			test -z "$PYEXEC" && AC_MSG_ERROR([specify --with-moz-chardet=/patch/python])
		else
			AC_MSG_CHECKING([for python])
			AC_MSG_RESULT([$PYEXEC])
		fi

		AC_MSG_CHECKING([python version])
		PYVER=`$PYEXEC -c "import sys; print '%d.%d' % (sys.version_info[[0]], sys.version_info[[1]])"`
		AC_MSG_RESULT([$PYVER])

		AC_MSG_CHECKING([python include path])
		PYINC=`$PYEXEC -c "import distutils.sysconfig; print distutils.sysconfig.get_python_inc()"`
		AC_MSG_RESULT([$PYINC])

		CPPFLAGS="$CPPFLAGS -I$PYINC"

		AC_MSG_CHECKING([python site-package path])
		PYLIB=`$PYEXEC -c "import distutils.sysconfig; print distutils.sysconfig.get_python_lib(1)"`
		AC_MSG_RESULT([$PYLIB])

		PYLIBNAME="python$PYVER"
		PYLIBOPT="-l$PYLIBNAME"
		PYPREFIX="`$PYEXEC -c \"import sys; print sys.prefix\"`"
		
		AC_MSG_CHECKING([for python C library])
		SEARCHPATH="lib64 lib local/lib64 local/lib"
		for i in $SEARCHPATH
		do
			if test -f $PYPREFIX/$i/lib$PYLIBNAME.$SHLIB_SUFFIX_NAME -o -f $PYPREFIX/$i/lib$PYLIBNAME.a ; then
				PYLIBDIR=$PYPREFIX/$i
				break
			fi
		done

		if test -n "$PYLIBDIR"; then
			PY_SHARED_LIBRARY="-L$PYLIBDIR $PYLIBOPT"
			AC_MSG_RESULT([$PY_SHARED_LIBRARY])

			AC_MSG_CHECKING([for python chardet])
			$PYEXEC -c "import chardet" >& /dev/null
			if test $? = 0 ; then
				PY_CHARDET_VERSION=`$PYEXEC -c "import chardet; print chardet.__version__" 2> /dev/null`
				AC_MSG_RESULT([$PY_CHARDET_VERSION support])
				AC_DEFINE(HAVE_PY_CHARDET,1,[Python Chardet support])
				AC_DEFINE_UNQUOTED(PY_CHARDET_VERSION,"$PY_CHARDET_VERSION", [Python Chardec version])
			else
				AC_MSG_RESULT([no support. Can't support Python Chardet])
			fi
		else
			AC_MSG_RESULT([not found. Can't support Python Chardet])
		fi
	fi

	CHARDET_SHARED_LIBADD="$MOZ_LIBS $ICU_LIBDIR $ICU_LIBS $PY_SHARED_LIBRARY"
	PHP_SUBST(CHARDET_SHARED_LIBADD)
	PHP_NEW_EXTENSION(chardet, php_chardet.c, $ext_shared)
fi
