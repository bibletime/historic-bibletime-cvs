KDE_CXXFLAGS=-fno-rtti -fno-exceptions
SUBDIRS = qt3stuff
INCLUDES = $(all_includes)
libthirdparty_a_METASOURCES = AUTO
noinst_LIBRARIES = libthirdparty.a
libthirdparty_a_SOURCES = dummy.cpp
EXTRA_DIST = $(libthirdparty_a_SOURCES)

dummy.cpp:
	touch dummy.cpp
