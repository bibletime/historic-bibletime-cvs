LANG=sk

install-data-local:
	$(mkinstalldirs) $(kde_htmldir)/${LANG}/bibletime; \
 	for file in *.html; do \
		$(INSTALL_DATA) $$file $(kde_htmldir)/${LANG}/bibletime && \
		chmod a+r $(kde_htmldir)/${LANG}/bibletime/$$file; \
	done; \
	cp -Ra common $(kde_htmldir)/${LANG}/bibletime; \
	cd ..; \
	chmod a+r+X -R $(kde_htmldir)/${LANG}/bibletime/;

uninstall-local:
	@for f in *.html; do \
		echo Removing $(kde_htmldir)/${LANG}/bibletime/$$f \
		-rm -f $(kde_htmldir)/${LANG}/bibletime/$$f \
	done; \
	cd .. \
	echo Deleting $(kde_htmldir)/${LANG}/bibletime/common;
	rm -Rf $(kde_htmldir)/${LANG}/bibletime/common;

EXTRA_DIST =


