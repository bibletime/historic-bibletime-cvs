SUBDIRS = helpdialog 

EXTRA_DIST = tipdatabase index.sgml 

install-data-local:
	$(mkinstalldirs) $(kde_htmldir)/de/bibletime/
	$(INSTALL_DATA) tipdatabase $(kde_htmldir)/de/bibletime/tipdatabase

uninstall-local:
	-rm -f $(kde_htmldir)/de/bibletime/tipdatabase
