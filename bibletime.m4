##	-*- autoconf -*-

dnl -------------------------------------------------------------------------------
dnl		Check wheter to use static linking, first parameter is the result (YES/NO)
dnl -------------------------------------------------------------------------------
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
	CPPFLAGS="-DSTATIC_BUILD $CPPFLAGS";
	$1="YES";
else
	$1="NO";
fi

AC_MSG_RESULT($USE_STATIC_LINKING)
])

dnl -------------------------------------------------------------------------------
dnl		Checks the type of the distribution
dnl		Possible names for the distributions are SuSE, RedHat, Mandrake
dnl -------------------------------------------------------------------------------
AC_DEFUN(AC_CHECK_DISTRIBUTION,
[
AC_MSG_CHECKING([for the used Linux distribution])

DISTRIBUTION="unknown"
DISTRIBUTION_VERSION="unknown"

PREFIX="/usr"

RPM_GROUP="Graphical desktop/KDE"; #standard, change it below if it's not fitting
	
if test -f "/etc/SuSE-release"; then
	DISTRIBUTION="SuSE"
	DISTRIBUTION_VERSION=[`grep "VERSION =" /etc/SuSE-release | sed 's/VERSION = //'`];	
	PREFIX="/opt/kde2"
	RPM_GROUP="X11/KDE/Applications";
elif test -f "/etc/mandrake-release"; then #Mandrake
	DISTRIBUTION="Mandrake"
	DISTRIBUTION_VERSION=[`grep "Mandrake" /etc/mandrake-release | sed 's/[a-zA-Z()]*//g' | awk '{print $1}'`];
elif test -f "/etc/redhat-release"; then #Red Hat
	DISTRIBUTION="Red Hat"
	DISTRIBUTION_VERSION=[`grep "Red Hat" /etc/redhat-release | sed 's/[a-zA-Z()]*//g' | awk '{print $1}'`];
elif test -f "/etc/debian_version"; then #Debian
	DISTRIBUTION="Debian"
	DISTRIBUTION_VERSION=[`cat /etc/debian_version`];
fi
	

if test "$DIST_TYPE" != "unknown"; then
	AC_DEFINE_UNQUOTED(DISTRIBUTION, ${DISTRIBUTION}, [The type of your distribution])
fi
AC_SUBST(DISTRIBUTION)

if test "$DIST_VERSION" != "unknown"; then
	AC_DEFINE_UNQUOTED(DISTRIBUTION_VERSION, ${DISTRIBUTION_VERSION}, [The version of your distribution])
fi
AC_SUBST(DISTRIBUTION_VERSION)

AC_SUBST(RPM_GROUP)
AC_SUBST(PREFIX)

AC_MSG_RESULT([$DISTRIBUTION $DISTRIBUTION_VERSION])
])

