SUBDIRS = \
lowcolor \
highcolor \
.

EXTRA_DIST = \
tipday.jpg \
startuplogo.jpg \
cross.png

BIBLETIME_PICS = \
tipday.jpg \
startuplogo.jpg \
cross.png 

install-data-local:
	$(mkinstalldirs) $(kde_datadir)/bibletime/pics/
	chmod 755 $(kde_datadir)/bibletime/pics/;
	@for file in $(BIBLETIME_PICS); do \
	echo Installing $$file; \
	$(INSTALL_DATA) $$file $(kde_datadir)/bibletime/pics/$$file; \
	chown root:root $(kde_datadir)/bibletime/pics/$$file; \
	chmod 644 $(kde_datadir)/bibletime/pics/$$file; \
	done

uninstall-local:
	@for file in $(BIBLETIME_PICS); \
	do \
	echo Removing $(kde_datadir)/bibletime/pics/$$file; \
	rm -f $(kde_datadir)/bibletime/pics/$$file; \
	done

KDE_ICON = AUTO
