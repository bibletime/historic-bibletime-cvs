SUBDIRS = \
16x16 \
22x22 \
32x32 \
48x48 \
64x64 \
.

BIBLETIME_PICS = \
startuplogo.png


EXTRA_DIST = $(BIBLETIME_PICS)

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(kde_datadir)/bibletime/pics/
	chmod 755 $(DESTDIR)$(kde_datadir)/bibletime/pics/;
	@for file in $(BIBLETIME_PICS); do \
	echo Installing $$file; \
	$(INSTALL_DATA) $$file $(DESTDIR)$(kde_datadir)/bibletime/pics/$$file; \
	chmod 644 $(DESTDIR)$(kde_datadir)/bibletime/pics/$$file; \
	done

uninstall-local:
	@for file in $(BIBLETIME_PICS); \
	do \
	echo Removing $(DESTDIR)$(kde_datadir)/bibletime/pics/$$file; \
	rm -f $(DESTDIR)$(kde_datadir)/bibletime/pics/$$file; \
	done

KDE_ICON = AUTO

