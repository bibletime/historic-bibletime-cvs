SUBDIRS = bibletime po admin .

EXTRA_DIST = \
bibletime.kdevprj \
COPYING \
ChangeLog \
bibletime.spec.in \
README.in \
TODO.in \
INSTALL.in \
bibletime.lsm.in \
bibletime.lsm \
bibletime.spec \
NEWS \
README \
TODO \
INSTALL \
configure.in.in \
configure.in \
am_edit \
bibletime.desktop.in \
bibletime.desktop \
NEWS.in \
Makefile.cvs \
bibletime.m4 sword.m4

# not a GNU package. You can remove this line, if 
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 

datadir = $(kde_appsdir)/Applications
data_DATA = bibletime.desktop

$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in sword.m4 bibletime.m4
	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in sword.m4 bibletime.m4 > acinclude.m4
$(top_srcdir)/configure.in: $(top_srcdir)/configure.files $(shell test -f $(top_srcdir)/configure.files && cat $(top_srcdir)/configure.files)
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in
$(top_srcdir)/subdirs:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs
$(top_srcdir)/configure.files:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.files

install-data-local:
	chmod 755 $(kde_datadir)/bibletime -R

MAINTAINERCLEANFILES = configure.files subdirs 

dist-hook:
	cd $(top_distdir) && perl admin/am_edit -padmin
	cd $(top_distdir) && $(MAKE) -f admin/Makefile.common subdirs
