# make bibletime.pot
# the -x is for skipping messages already translated in kdelibs

messages:
	$(XGETTEXT) -C -ki18n -x $(KDEDIR)/include/kde.pot `find .. -name \*.h -o -name \*.cpp -o -name \*.cc` -o messages.pot
