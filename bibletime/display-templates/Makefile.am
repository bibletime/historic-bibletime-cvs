#TEMPLATES = Default.tmpl Simple.tmpl
TEMPLATES = *.tmpl

EXTRA_DIST = template-init.cpp create_code.pl names.conf $(TEMPLATES)

all:
	perl ./create_code.pl $(TEMPLATES)

template-init.cpp:
	perl ./create_code.pl $(TEMPLATES)

CLEANFILES = template-init.cpp
