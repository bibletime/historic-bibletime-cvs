# make bibletime.pot
# the -x is for skipping messages already translated in kdelibs

messages:
	perl ../extractrc ../bibletime/xml/*.rc > ../bibletime/rc-dummy.cpp && \
	sed "s/#define//" ../bibletime/tooltipdef.h > ../bibletime/dummy.h && \
	sed "s/#define//" ../bibletime/whatsthisdef.h >> ../bibletime/dummy.h && \
	perl ../preparetips ../docs/tips > ../bibletime/tips-dummy.cpp && \
	$(XGETTEXT) -C -F -L C++ -ki18n -x $(KDEDIR)/include/kde.pot `find .. -name \*.h -o -name \*.cpp -o -name \*.cc` -o bibletime.pot
	recode iso8859-1..utf-8 bibletime.pot