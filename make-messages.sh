#!/bin/sh
myDir=/opt/kde2
target=../bibletime-doc

current=$PWD

perl ./extractrc bibletime/xml/*.rc > bibletime/rc-dummy.cpp && \
sed "s/#define//" bibletime/tooltipdef.h > bibletime/dummy.h && \
sed "s/#define//" bibletime/whatsthisdef.h >> bibletime/dummy.h && \
perl ./preparetips $target/docs/en/tips > bibletime/tips-dummy.cpp && \
xgettext -C -x $myDir/include/kde.pot -ki18n -kI18N_NOOP -o $target/po/bibletime.pot `find ./ -name \*.cpp` `find . -name \*.h` &&  \
cd $target/po && make merge && rm $current/bibletime/dummy.h && rm $current/bibletime/rc-dummy.cpp && rm $current/bibletime/tips-dummy.cpp


cd $current

