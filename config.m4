dnl $Id$
dnl config.m4 for extension mutils

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(mutils, for mutils support,
Make sure that the comment is aligned:
[  --with-mutils             Include mutils support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(mutils, whether to enable mutils support,
dnl Make sure that the comment is aligned:
dnl [  --enable-mutils           Enable mutils support])

if test "$PHP_MUTILS" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-mutils -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/mutils.h"  # you most likely want to change this
  dnl if test -r $PHP_MUTILS/$SEARCH_FOR; then # path given as parameter
  dnl   MUTILS_DIR=$PHP_MUTILS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for mutils files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MUTILS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MUTILS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the mutils distribution])
  dnl fi

  dnl # --with-mutils -> add include path
  dnl PHP_ADD_INCLUDE($MUTILS_DIR/include)

  dnl # --with-mutils -> check for lib and symbol presence
  dnl LIBNAME=mutils # you may want to change this
  dnl LIBSYMBOL=mutils # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MUTILS_DIR/$PHP_LIBDIR, MUTILS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MUTILSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong mutils lib version or lib not found])
  dnl ],[
  dnl   -L$MUTILS_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(MUTILS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(mutils, 
    mutils.c                \
    m_ratelimit.c           \
    alloc/mmap.c,
    $ext_shared)
  PHP_ADD_BUILD_DIR([$ext_builddir/alloc])
fi
