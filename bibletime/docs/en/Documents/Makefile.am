SUBDIRS = common HTML
LANG = en
INDEX = index.docbook

EXTRA_DIST = \
hdbk_idx.docbook \
hdbk_intro.docbook \
hdbk_operation.docbook \
hdbk_start.docbook \
hdbk_toolbars.docbook \
help_first_startup.docbook \
help_first_time_search.docbook \
help_idx.docbook \
help_initbackend_failed.docbook \
help_module_encrypted.docbook \
help_no_mods_d.docbook \
help_no_modules.docbook \
index.docbook \
inst_bin.docbook \
inst_dvel.docbook \
inst_idx.docbook \
inst_preq.docbook \
inst_sources.docbook \
inst_user.docbook \
readme.txt


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

