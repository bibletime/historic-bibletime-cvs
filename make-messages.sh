#!/bin/sh
myDir=/opt/kde3/include/
targetLangDir=../bibletime-i18n-

current=$PWD

perl ./extractrc bibletime/xml/*.rc > bibletime/rc-dummy.cpp && \
sed "s/#define//" bibletime/tooltipdef.h > bibletime/dummy.h && \
sed "s/#define//" bibletime/whatsthisdef.h >> bibletime/dummy.h && \
perl ./preparetips docs/tips > bibletime/tips-dummy.cpp && \
xgettext -C -x $myDir/kde.pot -ki18n -kI18N_NOOP -o pot/bibletime.pot `find ./ -name \*.cpp` `find . -name \*.h`
#cd $target/po && make merge && rm $current/bibletime/dummy.h && rm $current/bibletime/rc-dummy.cpp && rm $current/bibletime/tips-dummy.cpp


cd $current



