#TEMPLATES = Default.tmpl Simple.tmpl
templdir = $(top_srcdir)/bibletime/display-templates
TEMPLATES = $(templdir)/*.tmpl

EXTRA_DIST = template-init.cpp create_code.pl names.conf $(TEMPLATES)

all:
	perl $(templdir)/create_code.pl $(TEMPLATES)

template-init.cpp:
	perl $(templdir)/create_code.pl $(TEMPLATES)

CLEANFILES = template-init.cpp
