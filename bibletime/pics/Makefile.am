SUBDIRS = \
lowcolor \
highcolor \
.

EXTRA_DIST = \
startuplogo.jpg \
cross.png

BIBLETIME_PICS = \
startuplogo.jpg \
cross.png 

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(kde_datadir)/bibletime/pics/
	chmod 755 $(DESTDIR)$(kde_datadir)/bibletime/pics/;
	@for file in $(BIBLETIME_PICS); do \
	echo Installing $$file; \
	$(INSTALL_DATA) $$file $(DESTDIR)$(kde_datadir)/bibletime/pics/$$file; \
	chown root:root $(DESTDIR)$(kde_datadir)/bibletime/pics/$$file; \
	chmod 644 $(DESTDIR)$(kde_datadir)/bibletime/pics/$$file; \
	done

uninstall-local:
	@for file in $(BIBLETIME_PICS); \
	do \
	echo Removing $(DESTDIR)$(kde_datadir)/bibletime/pics/$$file; \
	rm -f $(DESTDIR)$(kde_datadir)/bibletime/pics/$$file; \
	done

KDE_ICON = AUTO

