btsetupwizard_SOURCES = \
install.cpp \
remove.cpp \
btsetupwizard.cpp \
main.cpp

btsetupwizard_LDADD   = \
../bibletime/printing/libprinting.a \
../bibletime/frontend/displaywindow/libdisplaywindow.a \
../bibletime/frontend/display/libdisplay.a \
../bibletime/frontend/keychooser/libkeychooser.a \
../bibletime/frontend/mainindex/libmainindex.a \
../bibletime/frontend/searchdialog/libsearchdialog.a \
../bibletime/frontend/libfrontend.a \
../bibletime/backend/libbackend.a \
../bibletime/util/libutil.a \
-lXext \
$(LIB_QT) \
$(LIB_KDECORE) \
$(LIB_KDEUI) \
$(LIB_SWORD) \
$(LIBSOCKET)

all_headers = btsetupwizard.h

EXTRA_DIST = $(btsetupwizard_SOURCES) $(all_headers)

INCLUDES = $(all_includes) -I../bibletime/
bin_PROGRAMS = btsetupwizard
btsetupwizard_METASOURCES = AUTO
btsetupwizard_LDFLAGS = $(BT_LDFLAGS) $(all_libraries) $(KDE_RPATH)
DISTCLEANFILES = $(btsetupwizard_METASOURCES)
