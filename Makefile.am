KDE_OPTIONS = noautodist
SUBDIRS = bibletime pot docs .

EXTRA_DIST = \
BibleTime.kdevelop \
LICENSE \
ChangeLog \
bibletime.spec.in bibletime.spec \
README.in README \
INSTALL.in INSTALL\
bibletime.lsm.in bibletime.lsm \
configure.in.in configure.in \
bibletime.desktop.in bibletime.desktop \
Makefile.cvs autogen.sh \
bibletime.m4 sword.m4 clucene.m4

btadmindir = $(top_srcdir)/admin

# EXTRA_DIST += $(btadmindir)/ChangeLog
EXTRA_DIST += $(btadmindir)/compile
EXTRA_DIST += $(btadmindir)/config.guess
EXTRA_DIST += $(btadmindir)/config.pl
EXTRA_DIST += $(btadmindir)/config.sub
EXTRA_DIST += $(btadmindir)/configure.in.bot.end
EXTRA_DIST += $(btadmindir)/configure.in.min
EXTRA_DIST += $(btadmindir)/cvs-clean.pl
EXTRA_DIST += $(btadmindir)/install-sh
EXTRA_DIST += $(btadmindir)/ltmain.sh
EXTRA_DIST += $(btadmindir)/missing
EXTRA_DIST += $(btadmindir)/mkinstalldirs
EXTRA_DIST += $(btadmindir)/ylwrap
EXTRA_DIST += $(btadmindir)/Makefile.common
EXTRA_DIST += $(btadmindir)/acinclude.m4.in
EXTRA_DIST += $(btadmindir)/am_edit
EXTRA_DIST += $(btadmindir)/conf.change.pl
EXTRA_DIST += $(btadmindir)/debianrules
EXTRA_DIST += $(btadmindir)/depcomp
EXTRA_DIST += $(btadmindir)/detect-autoconf.sh
EXTRA_DIST += $(btadmindir)/libtool.m4.in
EXTRA_DIST += $(btadmindir)/nmcheck
EXTRA_DIST += $(btadmindir)/cvs.sh


# not a GNU package. You can remove this line, if 
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = 1.6 foreign dist-bzip2

$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in sword.m4 bibletime.m4 clucene.m4
	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in sword.m4 bibletime.m4 clucene.m4 > acinclude.m4
$(top_srcdir)/configure.in: $(top_srcdir)/configure.files $(shell test -f $(top_srcdir)/configure.files && cat $(top_srcdir)/configure.files)
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in
$(top_srcdir)/subdirs:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs
$(top_srcdir)/configure.files:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.files

install-data-local:
	chmod 755 $(DESTDIR)$(kde_datadir)/bibletime -R
	$(mkinstalldirs) $(kde_appsdir)/Applications
	$(INSTALL_DATA) $(top_builddir)/bibletime.desktop $(kde_appsdir)/Applications/bibletime.desktop

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/bibletime.desktop

MAINTAINERCLEANFILES = configure.files subdirs 

dist-hook:	
	rm -f $(distdir)/bibletime-*.tar.gz $(distdir)/bibletime-*.tar.bz2
	rm -rf $(find $(distdir) -name tags -o -name TAGS -o -name \*.xcf -o -name \*.xcf.bz2)
#	rm -f $(distdir)/admin/Changelog $(distdir)/make-messages.sh \
#		$(distdir)/extractrc $(distdir)/preparetips \
#		$(distdir)/bibletime.doxygen $(distdir)/bibletime.kdevses \
#		$(distdir)/bibletime.start $(distdir)/configure.files \
#		$(distdir)/bibletime/pics/gen_am.sh
	rm -rf $(distdir)/autom4te.cache
	rm -rf $(distdir)/BibleTime.kdevelop.pcs
	rm -rf $(distdir)/docs/handbook/html/*.html
	rm -rf $(distdir)/docs/howto/html/*.html
#	rm -rf $(distdir)/BibleTime.tag
# $(distdir)/BibleTime.kdevses
