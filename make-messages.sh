#!/bin/sh
myDir=/opt/kde2

perl ./extractrc bibletime/xml/*.rc > bibletime/rc-dummy.cpp && \
sed "s/#define//" bibletime/tooltipdef.h > bibletime/dummy.h && \
sed "s/#define//" bibletime/whatsthisdef.h >> bibletime/dummy.h && \
xgettext -C -x $myDir/include/kde.pot -ki18n -kI18N_NOOP -o po/bibletime.pot `find ./ -name \*.cpp` `find . -name \*.h` &&  \
cd po && make merge && \
rm ../bibletime/dummy.h && \
rm ../bibletime/rc-dummy.cpp


