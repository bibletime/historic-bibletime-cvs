SUBDIRS = common

EXTRA_DIST = \
bt_hdbk.docbook \
development_install.dockbook \
installation.dockbook \
intro.dockbook \
operation.dockbook \
read_me.txt \
toolbars.dockbook \
helpdialog.dockbook

install-data-local:
	$(mkinstalldirs) $(kde_htmldir)/en/bibletime
	cd HTML; \
 	for file in *.html; \
 	do \
		$(INSTALL_DATA) $$file $(kde_htmldir)/en/bibletime; \
		chmod a+r $(kde_htmldir)/en/bibletime/$$file; \
	done; \
	cp -Ra common $(kde_htmldir)/en/bibletime; \
	cd ..; \
	chmod a+r+X -R $(kde_htmldir)/en/bibletime/

uninstall-local:
	cd HTML; \
	@for f in *.html; do \
		echo Removing $(kde_htmldir)/en/bibletime/$$f \
		-rm -f $(kde_htmldir)/en/bibletime/$$f \
	done; \
	cd .. \
	echo Deleting $(kde_htmldir)/en/bibletime/common; \
	rm -Rf $(kde_htmldir)/en/bibletime/common;

handbook:
	test -d HTML.old && rm -Rf HTML.old; \
	test -d HTML && mv HTML HTML.old; \
	echo Creating handbook - please wait ...; \
	$(KDB2HTML) bt_hdbk.docbook > /dev/null; \
	test -d HTML/common || mkdir -p HTML/common; \
	test -d common && cp common/*.png HTML/common/; \
	test -d HTML.old && test -d HTML.old/CVS && mv HTML.old/CVS HTML/CVS; \
	test -d HTML.old && test -d HTML.old/common && test -d HTML.old/common/CVS && mv HTML.old/common/CVS HTML/common; \
	test -d HTML.old && rm -Rf HTML.old;
