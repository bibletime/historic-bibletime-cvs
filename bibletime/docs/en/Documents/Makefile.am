SUBDIRS = common HTML
LANG = en
INDEX = index.docbook

install-data-local:
	$(mkinstalldirs) $(kde_htmldir)/${LANG}/bibletime
	cd HTML; \
 	for file in *.html; \
 	do \
		$(INSTALL_DATA) $$file $(kde_htmldir)/${LANG}/bibletime && \
		chmod a+r $(kde_htmldir)/${LANG}/bibletime/$$file; \
	done; \
	cp -Ra common $(kde_htmldir)/${LANG}/bibletime; \
	cd ..; \
	chmod a+r+X -R $(kde_htmldir)/${LANG}/bibletime/

uninstall-local:
	cd HTML; \
	@for f in *.html; do \
		echo Removing $(kde_htmldir)/${LANG}/bibletime/$$f \
		-rm -f $(kde_htmldir)/${LANG}/bibletime/$$f \
	done; \
	cd .. \
	echo Deleting $(kde_htmldir)/${LANG}/bibletime/common; \
	rm -Rf $(kde_htmldir)/${LANG}/bibletime/common;

handbook:
	test -d HTML.old && rm -Rf HTML.old; \
	test -d HTML && mv HTML HTML.old; \
	echo Creating handbook - please wait ...; \
	$(KDB2HTML) ${INDEX}; \
	test -d HTML/common || mkdir -p HTML/common; \
	test -d common && cp common/*.png HTML/common/; \
	test -d HTML.old && test -d HTML.old/CVS && mv HTML.old/CVS HTML/CVS; \
	test -d HTML.old && test -d HTML.old/common && test -d HTML.old/common/CVS && mv HTML.old/common/CVS HTML/common; \
	test -d HTML.old && rm -Rf HTML.old;

