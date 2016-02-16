#!/bin/sh
# $Id$

if [ -f Makefile ]; then
	make distclean
fi
rm -rf autom4te.cache/ build/ include/ modules/ tags
rm -f Makefile.global ac*.m4 config.guess config.h* config.nice config.sub configure*
rm -f install-sh ltmain.sh missing mkinstalldirs scan_makefile_in.awk run-tests.php .deps
rm -f tests/*.{diff,exp,log,out,php,sh,mem}
