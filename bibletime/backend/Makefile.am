libbackend_a_METASOURCES = AUTO
noinst_LIBRARIES = libbackend.a

INCLUDES = $(all_includes)

libbackend_a_SOURCES = bt_gbfhtml.cpp bt_osishtml.cpp bt_plainhtml.cpp \
	bt_thmlhtml.cpp bt_thmlplain.cpp btfiltermgr.cpp btstringmgr.cpp cbookdisplay.cpp \
	cchapterdisplay.cpp cdisplayrendering.cpp cdisplaytemplatemgr.cpp centrydisplay.cpp \
	cfiltertool.cpp chtmlexportrendering.cpp clanguagemgr.cpp cplaintextexportrendering.cpp \
	creferencemanager.cpp cswordbackend.cpp cswordbiblemoduleinfo.cpp cswordbookmoduleinfo.cpp \
	cswordcommentarymoduleinfo.cpp cswordkey.cpp cswordldkey.cpp cswordlexiconmoduleinfo.cpp \
	cswordmoduleinfo.cpp cswordmodulesearch.cpp cswordtreekey.cpp cswordversekey.cpp \
	ctextrendering.cpp osismorphsegmentation.cpp

all_headers = cswordbiblemoduleinfo.h \
cswordcommentarymoduleinfo.h \
cswordlexiconmoduleinfo.h \
cswordkey.h \
cswordversekey.h \
cswordldkey.h \
cswordbackend.h \
cswordmoduleinfo.h \
cswordmodulesearch.h \
creferencemanager.h \
cswordbookmoduleinfo.h \
cswordtreekey.h \
bt_thmlhtml.h \
bt_osishtml.h \
bt_gbfhtml.h \
bt_plainhtml.h \
bt_thmlplain.h \
centrydisplay.h \
clanguagemgr.h \
cfiltertool.h \
cdisplaytemplatemgr.h \
ctextrendering.h \
btstringmgr.h \
cbookdisplay.h \
cchapterdisplay.h \
chtmlexportrendering.h \
cplaintextexportrendering.h \
cdisplayrendering.h \
osismorphsegmentation.h

EXTRA_DIST = $(libbackend_a_SOURCES) $(all_headers)
noinst_HEADERS = btfiltermgr.h
