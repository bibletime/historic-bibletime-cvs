LDADD = \
../frontend/displaywindow/libdisplaywindow.a \
../frontend/display/libdisplay.a \
../frontend/keychooser/libkeychooser.a \
../frontend/mainindex/libmainindex.a \
../frontend/searchdialog/libsearchdialog.a \
../frontend/libfrontend.a \
../backend/libbackend.a \
../util/libutil.a \
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

INCLUDES = $(all_includes)

# Rules for the test code (use `make check` to execute)
TESTS = cppunittest
check_PROGRAMS = $(TESTS)
cppunittest_SOURCES = main.cpp cswordversekey_test.cpp
cppunittest_CXXFLAGS = $(CPPUNIT_CFLAGS)
cppunittest_LDFLAGS = -lcppunit -ldl
