# make bibletime.pot
# the -x is for skipping messages already translated in kdelibs

messages:
	perl ./extractrc ../bibletime/xml/*.rc > ../bibletime/rc-dummy.cpp
	perl ./preparetips ../docs/tips > ../bibletime/tips-dummy.cpp
	$(XGETTEXT) -C -F -L C++ -ki18n -kI18N_NOOP -x `kde-config --prefix`/include/kde.pot `find .. -path ../setupwizard -prune -and -type f -or -name \*.h -or -name \*.cpp -or -name \*.cc` -o bibletime.pot
	recode iso8859-1..utf-8 bibletime.pot
