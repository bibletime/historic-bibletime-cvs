
btsetupwizard_SOURCES = \
btsetupwizard.cpp \
main.cpp

btsetupwizard_LDADD   = \
../bibletime/frontend/libfrontend.a \
../bibletime/backend/libbackend.a \
-lXext \
$(LIB_QT) \
$(LIB_KDECORE) \
$(LIB_KDEUI) \
$(LIB_SWORD) \
$(LIBSOCKET)

all_headers = btsetupwizard.h

EXTRA_DIST = $(btsetupwizard_SOURCES) $(all_headers)

INCLUDES = $(all_includes)
bin_PROGRAMS = btsetupwizard
btsetupwizard_METASOURCES = AUTO
btsetupwizard_LDFLAGS = $(BT_LDFLAGS) $(all_libraries) $(KDE_RPATH)
DISTCLEANFILES = $(btsetupwizard_METASOURCES)