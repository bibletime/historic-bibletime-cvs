LANG = en
INDEX = index.docbook
HTML_DIR = ../HTML

handbook:
	echo Creating handbook - please wait ...; \
	$(KDB2HTML) ${INDEX}; \
	test -d HTML/common || mkdir -p HTML/common; \
	cp -f $(HTML_DIR)/Makefile* HTML; \
	test -d $(HTML_DIR)/common && cp -R $(HTML_DIR)/common/* HTML/common/; \
	test -d $(HTML_DIR) && test -d $(HTML_DIR)/CVS && cp -Ra $(HTML_DIR)/CVS HTML/; \
	test -d $(HTML_DIR) && rm -Rf $(HTML_DIR); \
	test -d HTML && mv HTML $(HTML_DIR);
	echo Finished!;


