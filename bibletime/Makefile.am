bibletime_SOURCES = \
bibletime_init.cpp \
bibletime_slots.cpp \
bibletime.cpp \
main.cpp

bibletime_LDADD   = \
./printing/libprinting.a \
./frontend/displaywindow/libdisplaywindow.a \
./frontend/display/libdisplay.a \
./frontend/keychooser/libkeychooser.a \
./frontend/mainindex/libmainindex.a \
./frontend/searchdialog/libsearchdialog.a \
./frontend/libfrontend.a \
./backend/libbackend.a \
./util/libutil.a \
-lXext \
$(LIB_QT) \
$(LIB_KDECORE) \
$(LIB_KDEUI) \
-lkdeprint \
$(LIB_KFILE) \
$(LIB_KHTML) \
$(LIB_X11) \
$(LIB_SWORD) \
$(LIBSOCKET)

SUBDIRS = backend printing frontend xml pics util

all_headers = bibletime.h \
whatsthisdef.h \
tooltipdef.h

EXTRA_DIST = $(bibletime_SOURCES) $(all_headers)

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

# set the include path for X, qt, KDE and Sword
INCLUDES = $(all_includes)

# claim, which subdirectories you want to install
# you can add here more. This one gets installed 
bin_PROGRAMS = bibletime 

bibletime_METASOURCES = AUTO

# the library search path. 
bibletime_LDFLAGS = $(BT_LDFLAGS) $(all_libraries) $(KDE_RPATH)

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(bibletime_METASOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
#messages:
#	$(XGETTEXT) -C -ki18n -x $(KDEDIR)/include/kde.pot `find . -name \*.h -o -name \*.cpp -o -name \*.cc` -o ../po/bibletime.pot

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(kde_confdir)

rpm: dist
	


