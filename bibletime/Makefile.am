bin_PROGRAMS = bibletime

bibletime_SOURCES = \
bibletime_init.cpp \
bibletime_slots.cpp \
bibletime.cpp \
main.cpp

bibletime_LDADD   = \
./printing/libprinting.a \
./frontend/optionsdialog/liboptionsdialog.a \
./frontend/presenters/libpresenters.a \
./frontend/keychooser/libkeychooser.a \
./frontend/groupmanager/libgroupmanager.a \
./frontend/searchdialog/libsearchdialog.a \
./frontend/libfrontend.a \
./frontend/thirdparty/qt3stuff/libqt3stuff.a \
./frontend/thirdparty/libthirdparty.a \
./backend/sword_backend/libswordbackend.a \
./backend/libbackend.a  \
-lpthread \
-lXext \
$(LIB_QT) \
$(LIB_KDECORE) \
$(LIB_KDEUI) \
$(LIB_KFILE) \
$(LIB_X11) \
$(LIBSOCKET)

SUBDIRS = backend frontend docs xml printing pics

EXTRA_DIST = \
main.cpp \
bibletime.cpp \
bibletime.h \
ressource.h \
bibletime_slots.cpp \
bibletime_init.cpp \
structdef.h \
whatsthisdef.h \
tooltipdef.h

# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
# you can add here more. This one gets installed 
bin_PROGRAMS = bibletime 

bibletime_METASOURCES = AUTO

# the library search path. 
bibletime_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(bibletime_METASOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -x $(KDEDIR)/include/kde.pot `find . -name \*.h -o -name \*.cpp -o -name \*.cc` -o ../po/bibletime.pot
