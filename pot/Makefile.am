# make bibletime.pot
# the -x is for skipping messages already translated in kdelibs

messages:
	perl ./extractrc ../bibletime/xml/*.rc > ../bibletime/rc-dummy.cpp
	perl ./preparetips ../docs/tips > ../bibletime/tips-dummy.cpp
	$(XGETTEXT) -C -F -L C++ -ki18n -kI18N_NOOP -x `kde-config --prefix`/include/kde.pot `find .. -type f -not -name clanguagemgr.cpp -and -name \*.cpp -or -name \*.h -or -name \*.cc` -o bibletime.pot
	recode iso8859-1..utf-8 bibletime.pot
	cp ../bibletime/backend/clanguagemgr.cpp clanguagemgr.cpp
	$(XGETTEXT) --from-code=utf-8 --join-existing -C -F -L C++ -ki18n -kI18N_NOOP -x `kde-config --prefix`/include/kde.pot clanguagemgr.cpp -o bibletime.pot
	rm clanguagemgr.cpp

