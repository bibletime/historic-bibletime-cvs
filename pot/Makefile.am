# make messages.pot
# the -x is for skipping messages already translated in kdelibs

messages:
	echo Preparing documentation files handbook.pot and howto.pot...
	mkdir -p tmp/
	for XML_FILE in `cd ..; find docs -name "*.docbook"`; do\
		echo " creating temporary POT file tmp/$$XML_FILE.pot";\
		(cd tmp; mkdir -p `dirname $$XML_FILE.pot`; xml2pot ../../$$XML_FILE > $$XML_FILE.pot;)\
	done
	echo merging POT files...
	msgcat --force-po -o handbook.pot `find tmp/docs/handbook -name "*.pot"`
	msgcat --force-po -o howto.pot `find tmp/docs/howto/ -name "*.pot"`
	rm -r ./tmp/
	echo Finished creating handbook.pot and howto.pot.\n
	echo Preparing program messages file messages.pot...
	perl ./extractrc ../bibletime/xml/*.rc > ../bibletime/rc-dummy.cpp
	perl ./preparetips ../docs/tips > ../bibletime/tips-dummy.cpp
	$(XGETTEXT) -C -F -L C++ -ki18n -kI18N_NOOP -x `kde-config --prefix`/include/kde/kde.pot `find .. -type f -not -name clanguagemgr.cpp -and -name \*.cpp -or -name \*.h -or -name \*.cc` -o messages.pot
	recode iso8859-1..utf-8 messages.pot
	cp ../bibletime/backend/clanguagemgr.cpp clanguagemgr.cpp
	$(XGETTEXT) --from-code=utf-8 --join-existing -C -F -L C++ -ki18n -kI18N_NOOP -x `kde-config --prefix`/include/kde/kde.pot clanguagemgr.cpp -o messages.pot
	rm clanguagemgr.cpp
	echo Finished creating messages.pot.



