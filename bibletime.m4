##	-*- autoconf -*-

dnl ----------------------------------------------------------------------
dnl		Check wheter to use static linking, first parameter is the result
dnl ----------------------------------------------------------------------
AC_DEFUN(AC_CHECK_STATIC_LINKING,
[
$1="NO"

AC_MSG_CHECKING(whether to use static linking)
AC_ARG_ENABLE(static-linking,
      [  --enable-static-linking	use static linking],
      USE_STATIC_LINKING=$enableval, USE_STATIC_LINKING="no")

if test "$USE_STATIC_LINKING" = "yes"; then
	BT_LDFLAGS="-all-static -Wl,-Bstatic";
	AC_SUBST(BT_LDFLAGS)
	$1="YES";
else
	$1="NO";
fi

AC_MSG_RESULT($USE_STATIC_LINKING)
])
