# KDE tags expanded automatically by am_edit - $Revision$ 
# Makefile.in generated automatically by automake 1.4-p5 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = .
top_srcdir = .

prefix = /usr/local/kde
exec_prefix = ${prefix}
#>- 
bindir = ${exec_prefix}/bin
#>+ 3
DEPDIR = .deps

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/bibletime
pkglibdir = $(libdir)/bibletime
pkgincludedir = $(includedir)/bibletime

top_builddir = .

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c -p
INSTALL_PROGRAM = ${INSTALL} $(INSTALL_STRIP_FLAG) $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_alias = i686-pc-linux-gnu
build_triplet = i686-pc-linux-gnu
host_alias = i686-pc-linux-gnu
host_triplet = i686-pc-linux-gnu
target_alias = i686-pc-linux-gnu
target_triplet = i686-pc-linux-gnu
ARTSCCONFIG = /opt/kde2/bin/artsc-config
AS = @AS@
AUTODIRS = 
BT_LDFLAGS = 
CC = gcc
CONF_FILES =  $(top_srcdir)/configure.in.in
CPP = gcc -E
CXX = g++
CXXCPP = g++ -E
DCOPIDL = /opt/kde2/bin/dcopidl
DCOPIDL2CPP = /opt/kde2/bin/dcopidl2cpp
DCOP_DEPENDENCIES = $(DCOPIDL)
DISTRIBUTION = SuSE
DISTRIBUTION_VERSION = 7.3
DLLTOOL = @DLLTOOL@
DPMSINC = @DPMSINC@
DPMSLIB = @DPMSLIB@
EXEEXT = 
GCJ = @GCJ@
GCJFLAGS = @GCJFLAGS@
GLINC = @GLINC@
GLLIB = @GLLIB@
GMSGFMT = /usr/bin/msgfmt
IDL = @IDL@
IDL_DEPENDENCIES = @IDL_DEPENDENCIES@
JAR = @JAR@
JAVAC = @JAVAC@
JAVAH = @JAVAH@
JVMLIBS = @JVMLIBS@
KDECONFIG = /opt/kde2/bin/kde-config
KDE_CXXFLAGS = 
KDE_EXTRA_RPATH = 
KDE_INCLUDES = -I/opt/kde2/include
KDE_LDFLAGS = -L/opt/kde2/lib
KDE_PLUGIN = -avoid-version -module -no-undefined $(KDE_RPATH)
KDE_RPATH = -R $(kde_libraries) -R $(qt_libraries) -R $(x_libraries)
KDE_USE_CLOSURE_FALSE = #
KDE_USE_CLOSURE_TRUE = 
KDE_USE_FINAL_FALSE = 
KDE_USE_FINAL_TRUE = #
KDE_XSL_STYLESHEET = /opt/kde2/share/apps/ksgmltools2/customization/kde-chunk.xsl
LIBCOMPAT = 
LIBCRYPT = -lcrypt
LIBDL = -ldl
LIBGEN = @LIBGEN@
LIBJPEG = -ljpeg
LIBMICO = @LIBMICO@
LIBOBJS = @LIBOBJS@
LIBPNG = -lpng -lz -lm
LIBPTHREAD = -lpthread
LIBPYTHON = @LIBPYTHON@
LIBQIMGIO = @LIBQIMGIO@
LIBRESOLV = -lresolv
LIBSHADOW = @LIBSHADOW@
LIBSM = -lSM -lICE
LIBSOCKET =  -lresolv
LIBTIFF = @LIBTIFF@
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIBUCB = 
LIBUTIL = -lutil
LIBXINERAMA = 
LIBZ = -lz
LIB_KAB = -lkab
LIB_KDECORE = -lkdecore
LIB_KDEUI = -lkdeui
LIB_KFILE = -lkfile
LIB_KFM = 
LIB_KFORMULA = -lkformula
LIB_KHTML = -lkhtml
LIB_KIMGIO = @LIB_KIMGIO@
LIB_KIO = -lkio
LIB_KPARTS = -lkparts
LIB_KSPELL = -lkspell
LIB_KSYCOCA = -lksycoca
LIB_KWRITE = -lkwrite
LIB_QT = -lqt $(LIBPNG) $(LIBJPEG) -lXext $(LIB_X11) $(LIBSM)
LIB_SMB = -lsmb
LIB_SWORD = -lsword
LIB_X11 = -lX11 $(LIBSOCKET)
LN_S = ln -s
MAKEINFO = makeinfo
MCOPIDL = /opt/kde2/bin/mcopidl
MEINPROC = /opt/kde2/bin/meinproc
MICO_INCLUDES = @MICO_INCLUDES@
MICO_LDFLAGS = @MICO_LDFLAGS@
MOC = /usr/lib/qt2//bin/moc
MSGFMT = /usr/bin/msgfmt
NOOPT_CXXFLAGS =  -fno-exceptions -fno-check-new
NOREPO = -fno-repo
OBJDUMP = @OBJDUMP@
OBJEXT = o
PACKAGE = bibletime
PAMLIBS = @PAMLIBS@
PASSWDLIBS = @PASSWDLIBS@
PYTHONINC = @PYTHONINC@
PYTHONLIB = @PYTHONLIB@
PYTHONMODDIR = @PYTHONMODDIR@
QT_INCLUDES = -I/usr/lib/qt2//include
QT_LDFLAGS = -L/usr/lib/qt2//lib
RANLIB = ranlib
REPO = -frepo
RPM_GROUP = X11/KDE/Applications
RPM_PREFIX = /opt/kde2
SETUIDFLAGS = @SETUIDFLAGS@
STRIP = strip
SWORD_INCLUDES = -I/usr/include/sword
SWORD_LIBRARY_PATH = 
TOPSUBDIRS =  bibletime
UIC = /usr/lib/qt2//bin/uic
USER_INCLUDES = 
USER_LDFLAGS = 
USE_EXCEPTIONS = -fexceptions
USE_RTTI = 
USE_THREADS =  -DPIC -fPIC
VERSION = 1.1beta1
XGETTEXT = /usr/bin/xgettext
XPMINC = @XPMINC@
XPMLIB = @XPMLIB@
X_EXTRA_LIBS =  -lresolv
X_INCLUDES = -I/usr/X11R6/include
X_LDFLAGS = -L/usr/X11R6/lib
X_PRE_LIBS = 
all_includes = -I/opt/kde2/include -I/usr/lib/qt2//include -I/usr/X11R6/include  -I/usr/include/sword
all_libraries = -L/usr/X11R6/lib -L/usr/lib/qt2//lib -L/opt/kde2/lib  -L/usr/lib
idldir = @idldir@
jni_includes = @jni_includes@
kde_appsdir = ${prefix}/share/applnk
kde_bindir = ${exec_prefix}/bin
kde_confdir = ${prefix}/share/config
kde_datadir = ${prefix}/share/apps
kde_htmldir = ${prefix}/share/doc/HTML
kde_icondir = ${prefix}/share/icons
kde_includes = /opt/kde2/include
kde_libraries = /opt/kde2/lib
kde_libs_htmldir = /opt/kde2//share/doc/HTML
kde_libs_prefix = /opt/kde2/
kde_locale = ${prefix}/share/locale
kde_mimedir = ${prefix}/share/mimelnk
kde_moduledir = ${exec_prefix}/lib/kde2
kde_servicesdir = ${prefix}/share/services
kde_servicetypesdir = ${prefix}/share/servicetypes
kde_sounddir = ${prefix}/share/sounds
kde_templatesdir = ${prefix}/share/templates
kde_wallpaperdir = ${prefix}/share/wallpapers
micodir = @micodir@
qt_includes = /usr/lib/qt2//include
qt_libraries = /usr/lib/qt2//lib
x_includes = /usr/X11R6/include
x_libraries = /usr/X11R6/lib

SUBDIRS = bibletime admin .

EXTRA_DIST =  bibletime.kdevprj COPYING ChangeLog bibletime.spec.in bibletime.spec README.in README TODO.in TODO INSTALL.in INSTALL bibletime.lsm.in bibletime.lsm configure.in.in configure.in bibletime.desktop.in bibletime.desktop NEWS.in NEWS Makefile.cvs bibletime.m4 sword.m4


# not a GNU package. You can remove this line, if 
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 

datadir = $(kde_appsdir)/Applications
data_DATA = bibletime.desktop

MAINTAINERCLEANFILES = configure.files subdirs 
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
mkinstalldirs = $(SHELL) $(top_srcdir)/admin/mkinstalldirs
CONFIG_HEADER = config.h
CONFIG_CLEAN_FILES =  NEWS README INSTALL TODO bibletime.lsm \
bibletime.spec bibletime.desktop
DATA =  $(data_DATA)

DIST_COMMON =  README ./stamp-h.in COPYING ChangeLog INSTALL INSTALL.in \
Makefile.am Makefile.in NEWS NEWS.in README.in TODO.in acinclude.m4 \
aclocal.m4 bibletime.desktop.in bibletime.lsm.in bibletime.spec.in \
config.guess config.h.in config.sub configure configure.in


#>- DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)
#>+ 4
KDE_DIST=preparetips bibletime.doxygen bibletime.start extractrc make-messages.sh 

DISTFILES= $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST) $(KDE_DIST)


TAR = tar
GZIP_ENV = --best
#>- all: all-redirect
#>+ 1
all: docs-am  all-redirect
.SUFFIXES:
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
#>- 	cd $(top_srcdir) && $(AUTOMAKE) --foreign ./Makefile
#>+ 2
	cd $(top_srcdir) && $(AUTOMAKE) --foreign ./Makefile
	cd $(top_srcdir) && perl admin/am_edit Makefile.in

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

$(ACLOCAL_M4):  configure.in  acinclude.m4
	cd $(srcdir) && $(ACLOCAL)

config.status: $(srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck
$(srcdir)/configure: $(srcdir)/configure.in $(ACLOCAL_M4) $(CONFIGURE_DEPENDENCIES)
	cd $(srcdir) && $(AUTOCONF)

config.h: stamp-h
	@if test ! -f $@; then \
		rm -f stamp-h; \
		$(MAKE) stamp-h; \
	else :; fi
stamp-h: $(srcdir)/config.h.in $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=config.h \
	     $(SHELL) ./config.status
	@echo timestamp > stamp-h 2> /dev/null
$(srcdir)/config.h.in: $(srcdir)/stamp-h.in
	@if test ! -f $@; then \
		rm -f $(srcdir)/stamp-h.in; \
		$(MAKE) $(srcdir)/stamp-h.in; \
	else :; fi
$(srcdir)/stamp-h.in: $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOHEADER)
	@echo timestamp > $(srcdir)/stamp-h.in 2> /dev/null

mostlyclean-hdr:

clean-hdr:

distclean-hdr:
	-rm -f config.h

maintainer-clean-hdr:
NEWS: $(top_builddir)/config.status NEWS.in
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status
README: $(top_builddir)/config.status README.in
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status
INSTALL: $(top_builddir)/config.status INSTALL.in
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status
TODO: $(top_builddir)/config.status TODO.in
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status
bibletime.lsm: $(top_builddir)/config.status bibletime.lsm.in
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status
bibletime.spec: $(top_builddir)/config.status bibletime.spec.in
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status
bibletime.desktop: $(top_builddir)/config.status bibletime.desktop.in
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

install-dataDATA: $(data_DATA)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(datadir)
	@list='$(data_DATA)'; for p in $$list; do \
	  if test -f $(srcdir)/$$p; then \
	    echo " $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(datadir)/$$p"; \
	    $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(datadir)/$$p; \
	  else if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(datadir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(datadir)/$$p; \
	  fi; fi; \
	done

uninstall-dataDATA:
	@$(NORMAL_UNINSTALL)
	list='$(data_DATA)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(datadir)/$$p; \
	done

# This directory's subdirectories are mostly independent; you can cd
# into them and run `make' without going through this Makefile.
# To change the values of `make' variables: instead of editing Makefiles,
# (1) if the variable is set in `config.status', edit `config.status'
#     (which will cause the Makefiles to be regenerated when you run `make');
# (2) otherwise, pass the desired values on the `make' command line.



all-recursive install-data-recursive install-exec-recursive \
installdirs-recursive install-recursive uninstall-recursive  \
check-recursive installcheck-recursive info-recursive dvi-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

mostlyclean-recursive clean-recursive distclean-recursive \
maintainer-clean-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	rev=''; list='$(SUBDIRS)'; for subdir in $$list; do \
	  rev="$$subdir $$rev"; \
	  test "$$subdir" != "." || dot_seen=yes; \
	done; \
	test "$$dot_seen" = "no" && rev=". $$rev"; \
	target=`echo $@ | sed s/-recursive//`; \
	for subdir in $$rev; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
tags-recursive:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) tags); \
	done

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS: tags-recursive $(HEADERS) $(SOURCES) config.h.in $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
   if test "$$subdir" = .; then :; else \
	    test -f $$subdir/TAGS && tags="$$tags -i $$here/$$subdir/TAGS"; \
   fi; \
	done; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)config.h.in$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags config.h.in $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(PACKAGE)-$(VERSION)
top_distdir = $(distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	-rm -rf $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) zxf $(distdir).tar.gz
	mkdir $(distdir)/=build
	mkdir $(distdir)/=inst
	dc_install_base=`cd $(distdir)/=inst && pwd`; \
	cd $(distdir)/=build \
	  && ../configure --srcdir=.. --prefix=$$dc_install_base \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) dist
	-rm -rf $(distdir)
	@banner="$(distdir).tar.gz is ready for distribution"; \
	dashes=`echo "$$banner" | sed s/./=/g`; \
	echo "$$dashes"; \
	echo "$$banner"; \
	echo "$$dashes"
dist: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
dist-all: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
distdir: $(DISTFILES)
	-rm -rf $(distdir)
	mkdir $(distdir)
	-chmod 777 $(distdir)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --foreign ./Makefile
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
	for subdir in $(SUBDIRS); do \
	  if test "$$subdir" = .; then :; else \
	    test -d $(distdir)/$$subdir \
	    || mkdir $(distdir)/$$subdir \
	    || exit 1; \
	    chmod 777 $(distdir)/$$subdir; \
	    (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) top_distdir=../$(distdir) distdir=../$(distdir)/$$subdir distdir) \
	      || exit 1; \
	  fi; \
	done
	$(MAKE) $(AM_MAKEFLAGS) top_distdir="$(top_distdir)" distdir="$(distdir)" dist-hook
info-am:
info: info-recursive
dvi-am:
dvi: dvi-recursive
check-am: all-am
check: check-recursive
installcheck-am:
installcheck: installcheck-recursive
all-recursive-am: config.h
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

install-exec-am:
install-exec: install-exec-recursive

install-data-am: install-dataDATA install-data-local
install-data: install-data-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-recursive
uninstall-am: uninstall-dataDATA
uninstall: uninstall-recursive
all-am: Makefile $(DATA) config.h
all-redirect: all-recursive-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs: installdirs-recursive
installdirs-am:
	$(mkinstalldirs)  $(DESTDIR)$(datadir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	-test -z "$(MAINTAINERCLEANFILES)" || rm -f $(MAINTAINERCLEANFILES)
mostlyclean-am:  mostlyclean-hdr mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-recursive

clean-am:  clean-hdr clean-tags clean-generic mostlyclean-am

#>- clean: clean-recursive
#>+ 1
clean: kde-rpo-clean  clean-recursive

distclean-am:  distclean-hdr distclean-tags distclean-generic clean-am
	-rm -f libtool

distclean: distclean-recursive
	-rm -f config.status

maintainer-clean-am:  maintainer-clean-hdr maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-recursive
	-rm -f config.status

.PHONY: mostlyclean-hdr distclean-hdr clean-hdr maintainer-clean-hdr \
uninstall-dataDATA install-dataDATA install-data-recursive \
uninstall-data-recursive install-exec-recursive \
uninstall-exec-recursive installdirs-recursive uninstalldirs-recursive \
all-recursive check-recursive installcheck-recursive info-recursive \
dvi-recursive mostlyclean-recursive distclean-recursive clean-recursive \
maintainer-clean-recursive tags tags-recursive mostlyclean-tags \
distclean-tags clean-tags maintainer-clean-tags distdir info-am info \
dvi-am dvi check check-am installcheck-am installcheck all-recursive-am \
install-exec-am install-exec install-data-local install-data-am \
install-data install-am install uninstall-am uninstall all-redirect \
all-am all installdirs-am installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in sword.m4 bibletime.m4
	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in sword.m4 bibletime.m4 > acinclude.m4
$(top_srcdir)/configure.in: $(top_srcdir)/configure.files $(shell test -f $(top_srcdir)/configure.files && cat $(top_srcdir)/configure.files)
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in
$(top_srcdir)/subdirs:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs
$(top_srcdir)/configure.files:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.files

install-data-local:
	chmod 755 $(DESTDIR)$(kde_datadir)/bibletime -R

dist-hook:
	cd $(top_distdir) && $(MAKE) -f Makefile.cvs

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:

#>+ 2
docs-am:

#>+ 5
force-reedit:
		cd $(top_srcdir) && $(AUTOMAKE) --foreign ./Makefile
	cd $(top_srcdir) && perl admin/am_edit Makefile.in


#>+ 2
final:
	$(MAKE) all-am
#>+ 2
no-final:
	$(MAKE) all-am
#>+ 3
cvs-clean:
	$(MAKE) -f $(top_srcdir)/admin/Makefile.common cvs-clean

#>+ 3
kde-rpo-clean:
	-rm -f *.rpo
