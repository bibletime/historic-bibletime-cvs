#!/bin/bash
#
# gen_am.sh script
#
# (C) 2001 The BibleTime team
# released under the GPL
#
# This script creates the Makefile.am files in the
# bibletime/docs/* directories and subdirectories.

# change to the dir the script is located in
cd `dirname $0`

export HEADER=\
"# this Makefile.am was automatically generated by bibletime/maintenance/gen_am.sh\n"\
"# please do not edit this file but rather run the script to update\n\n"

###########################
# create Makefile.am in /bibletime/docs/
###########################

. gen_am.dat

#echo generating Makefile.am
#echo -e $HEADER"SUBDIRS = "$DOC_LANGUAGES > Makefile.am

###########################
# create Makefile.am in /bibletime/docs/ directory
###########################
cd ../docs;

echo generating Makefile.am
( # output to Makefile.am
	echo -e $HEADER

	echo -n "SUBDIRS = "

	for PART in $DOC_PARTS; do
		if test -d $PART; then
			if test "$PART" = "howto"; then
				echo -n "\$(HOWTODIR) "
			else
				echo -n "$PART "
			fi
		fi
	done

	echo; echo

	if test -e tips; then
		echo "EXTRA_DIST =  tips"
		echo

		echo 'rcdir = $(kde_datadir)/bibletime';
		echo 'rc_DATA = tips';
	fi
) > Makefile.am

###########################
# create Makefile.am in /bibletime/docs/PART directories
###########################
for PART in $DOC_PARTS; do

	###################################
	#create $PART/Makefile.am
	###################################

	export I2=$PART
	echo generating $I2/Makefile.am
	( # output to Makefile.am
		echo -e $HEADER
		echo -n "SUBDIRS = unicode html"
		echo
		echo 'html-files:'
		echo '	if test -e unicode/index.docbook; then \'
		echo '		olddir=$$PWD; \'
		echo '		cd html;\'
		echo '		echo "Validating the XML files..."; \'
		echo '		if meinproc ../unicode/index.docbook; then\'
		echo '			echo "HTML files created successfully."; \'
		echo '		fi; cd $$olddir; \'
		echo '	fi;'
		echo
	) > $I2/Makefile.am

	#####################################
	#create $PART/????/Makefile.am
	#####################################

	for SECTION in unicode html; do
		I2=$PART/$SECTION
		echo generating $I2/Makefile.am
		( # output to Makefile.am
			echo -e $HEADER
			echo "KDE_OPTIONS = noautodist"
			echo -n "EXTRA_DIST = "

			FILES=`ls -d $I2/*.png $I2/*.html $I2/*.docbook 2>/dev/null`;
			if test $SECTION = "html"; then
				FILES=`ls -d $I2/*.png 2>/dev/null`;
			fi;

			HAS_FILES="NO"
			for I3 in $FILES; do
				echo -n `basename $I3`" "
				BASE_FILES="$BASE_FILES `basename $I3`";
				HAS_FILES="YES"
			done
			FILES=$BASE_FILES
			echo
			echo

			TARGET_ROOT='$(DESTDIR)$(kde_htmldir)en/bibletime'
			TARGET_DIR=$TARGET_ROOT/$PART

			if test $HAS_FILES = YES; then
				echo
				echo 'install-data-local:'
				echo '	mkdir -p '$TARGET_DIR'/;'  #mkdir -p creates all directories leaing to $TARGET_DIR
				echo '	chmod -R a+r+X  '$TARGET_ROOT';'
				echo '	for file in '$FILES'; do \'
				echo '	  $(INSTALL_DATA) $(srcdir)/$$file '$TARGET_DIR'; \'
				echo '	  chmod a+r '$TARGET_DIR'/$$file; \'
				echo '	done;'
				echo '	ln -sf $(DESTDIR)$(kde_htmldir)en/common '$TARGET_DIR'/common;\'
				echo '	chmod -R a+r+X '$TARGET_ROOT
				echo
				echo 'uninstall-local:'
				echo '	for file in '$FILES'; do \'
				echo '	  echo Removing '$TARGET_DIR'/$$file; \'
				echo '	  rm -f '$TARGET_DIR'/$$file; \'
				echo '	done;'
				echo '	echo Removing '$TARGET_DIR'/common;'
				echo '	rm -f '$TARGET_DIR'/common;'
			fi;
		) > $I2/Makefile.am
	done
done

#####################################################
cd ../maintenance;

###########################
# create Makefile.am in bibletime-i18n/po directory
###########################

echo generating ../../bibletime-i18n/po/Makefile.am
( # output to Makefile.am
	echo -e $HEADER
	echo
	echo "SUBDIRS = handbook \$(HOWTODIR)"
	echo
	echo -n "POFILES = "

	for I1 in $FOREIGN_PO_LANGUAGES; do
		echo -n "$I1.po "
	done

	echo

) > ../../bibletime-i18n/po/Makefile.am

cat >> ../../bibletime-i18n/po/Makefile.am <<'EOF'

GMOFILES = $(POFILES:.po=.gmo)

localedir = $(DESTDIR)$(kde_locale)
MAINTAINERCLEANFILES = $(GMOFILES)

GMSGFMT = @GMSGFMT@
MSGFMT = @MSGFMT@
XGETTEXT = @XGETTEXT@
EXTRA_DIST = $(POFILES) $(GMOFILES)

all: all-yes
install-data-local: install-yes

SUFFIXES = .po .gmo

.po.mo:
	$(MSGFMT) -o $@ $<

.po.gmo:
	file=$(srcdir)/`echo $* | sed 's,.*/,,'`.gmo \
	&& rm -f $$file && $(GMSGFMT) -o $$file $<

all-yes: $(GMOFILES)
all-no:

install-yes: all
	@catalogs='$(GMOFILES)'; \
	for cat in $$catalogs; do \
		destdir=$(localedir); \
		lang=`echo $$cat | sed 's/\.gmo//'`; \
		name=bibletime.mo \
		dir=$$destdir/$$lang/LC_MESSAGES; \
		$(mkinstalldirs) $$dir; \
		$(INSTALL_DATA) $$cat $$dir/$$name; \
	  echo "installing $$cat as $$dir/$$name"; \
	done

install-no:

uninstall-local:
	@catalogs='$(GMOFILES)'; \
	for cat in $$catalogs; do \
		destdir=$(localedir); \
		lang=`echo $$cat | sed 's/\.gmo//'`; \
		name=bibletime.mo; \
		dir=$$destdir/$$lang/LC_MESSAGES; \
		rm -f $$cat $$dir/$$name; \
		echo "removing $$dir/$$name" ; \
	done

merge:
	for cat in *.po; do \
		name=../../bibletime/pot/messages.pot ; \
		echo $$cat $$name; \
		msgmerge $$cat $$name > $$cat.new ; \
		mv $$cat.new $$cat ; \
	done

clean:
	rm -f *.gmo

EOF

###########################
# create Makefile.am in bibletime-i18n/po/[handbook howto] directory
###########################
for DOC_PO_PART in handbook howto; do

	echo generating ../../bibletime-i18n/po/$DOC_PO_PART/Makefile.am
	( # output to Makefile.am, removed --no-fuzzy-matching from msgcat
		echo -e $HEADER
		echo
		echo 'KDE_OPTIONS = foreign'
		echo
		echo 'merge:'
		echo '	for cat in *.po; do \'
		echo '		name=../../../bibletime/pot/'$DOC_PO_PART'.pot ; \'
		echo '		lang=`echo $$cat | sed s/\.po//`; \'
		echo '		echo $$cat $$lang $$name; \'
		echo '		if test -d ../../../bibletime-website/$$lang; then \'
		echo '			echo Trying to merge from the website po files; \'
		echo '			msgcat --force-po --no-wrap -o $$cat.temp ../../../bibletime-website/$$lang/po/full.po $$cat; \'
		echo '			mv $$cat.temp $$cat; \'
		echo '		else \'
		echo '			echo No merging from the website files possible.; \'
		echo '		fi; \'
		echo '		msgmerge $$cat $$name > $$cat.new; \'
		echo '		mv $$cat.new $$cat ; \'
		echo '	done;'

		echo 'clean:'
		echo '	rm -f *.gmo'
		echo

	) > ../../bibletime-i18n/po/$DOC_PO_PART/Makefile.am

done

	###########################
	# create Makefile.am in bibletime-i18n/docs directory
	###########################

	echo generating ../../bibletime-i18n/docs/Makefile.am
	( # output to Makefile.am
		echo -e $HEADER

		echo "SUBDIRS = $FOREIGN_DOC_LANGUAGES"
		echo
	) > ../../bibletime-i18n/docs/Makefile.am


for I1 in $FOREIGN_DOC_LANGUAGES; do

	###########################
	# create Makefile.am's in bibletime-i18n/docs/?? directories
	###########################

	echo generating ../../bibletime-i18n/docs/$I1/Makefile.am
	( # output to Makefile.am
		echo -e $HEADER

		echo -n "SUBDIRS = "

		for PART in $DOC_PARTS; do
			if test -d ../../bibletime-i18n/docs/$I1/$PART; then
				if test "$PART" = "howto"; then
					echo -n "\$(HOWTODIR) "
				else
					echo -n "$PART "
				fi
			fi
		done
		echo
		echo
	) > ../../bibletime-i18n/docs/$I1/Makefile.am


	###########################
	# create Makefile.am's in bibletime-i18n/docs/??/$PART directories
	###########################
	for PART in $DOC_PARTS; do

		if test -d ../../bibletime-i18n/docs/$I1/$PART; then

			###################################
			#create $PART/Makefile.am
			###################################

			export I2=../../bibletime-i18n/docs/$I1/$PART
			export ENGDIR=../../../../bibletime/docs
			echo generating $I2/Makefile.am
			( # output to Makefile.am
				echo -e $HEADER
				echo -n "SUBDIRS = "

				#if test -d $I2/html; then
				#	echo -n "html "
				#fi
				if test -d $I2/unicode; then
					echo -n "unicode "
				fi

				echo
				echo 'unicode-files:'
				echo '	for f in `ls '$ENGDIR'/'$PART'/unicode/*.docbook`; do \'
				echo '		newfile=`echo $$f | sed s/"..\/..\/..\/..\/bibletime\/docs\/'$PART'\/unicode\/"//`; \'
				echo '		echo "Calling po2xml $$f ../../../po/'$PART'/'$I1'.po  > unicode/$$newfile"; \'
				echo '		po2xml $$f ../../../po/'$PART'/'$I1'.po > unicode/$$newfile; \'
				echo '	done;'
				echo
				echo 'html-files: unicode-files'
				echo '	if test -e unicode/index.docbook; then \'
				echo '		olddir=$$PWD; \'
				echo '		cd html;\'
				echo '		echo "Validating the XML files..."; \'
				echo '		if meinproc ../unicode/index.docbook; then\'
				echo '			echo "HTML files created successfully."; \'
				echo '		fi; cd $$olddir; \'
				echo '	fi;'
				echo
			) > $I2/Makefile.am
			#####################################
			#create bibletime-i18n/docs/??/$PART/html/Makefile.am
			#####################################
			for SECTION in unicode; do

				I2=../../bibletime-i18n/docs/$I1/$PART/$SECTION
				I2_EN=../docs/$PART/$SECTION

				if test -d $I2; then

					echo generating $I2/Makefile.am
					( # output to Makefile.am
						echo -e $HEADER
						echo -n "EXTRA_DIST = "
						FILES=`ls -d $I2/*.png $I2/*.html $I2/*.docbook 2>/dev/null`;
						if test $SECTION = "html"; then
							FILES=`ls -d $I2/*.png 2>/dev/null`;
						fi;

						for I3 in $FILES; do
							echo -n `basename $I3`" "
						done
						echo

						TARGET_ROOT='$(DESTDIR)$(kde_htmldir)'$I1'/bibletime'
						TARGET_DIR=$TARGET_ROOT/$PART

						EN_FILES=""
						HAS_FILES="NO"

						for FILE in `ls -d $I2_EN/*.png $I2_EN/*.docbook 2>/dev/null`; do
							EN_FILES="$EN_FILES `basename $FILE`";
							HAS_FILES="YES"
						done

						if test $HAS_FILES = YES; then
							echo
							echo 'install-data-local:'
							echo '	mkdir -p '$TARGET_DIR'/;'  #mkdir -p creates all directories leading to $TARGET_DIR
							echo '	chmod -R a+r+X  '$TARGET_ROOT';'
							echo '	for file in '$EN_FILES'; do \'
							echo '	  if test -e $$file; then \' ## in this directory
							echo '	    $(INSTALL_DATA) $(srcdir)/$$file '$TARGET_DIR'; \'
							echo '	    chmod a+r '$TARGET_DIR'/$$file; \'
							echo '	  else \'
							echo '	    ln -sf '$TARGET_DIR'/../../../en/bibletime/'$PART'/$$file '$TARGET_DIR/'$$file; \'
							echo '	  fi; \'
							echo '	done;'
							echo '	if test -d '$TARGET_DIR'/../../common; then \'
							echo '	  ln -sf '$TARGET_DIR'../../common '$TARGET_DIR'/common;\'
							echo '	else \'
							############## Use the english css data ###################
							echo '	  ln -sf '$TARGET_DIR'/../../../en/common '$TARGET_DIR'/common;\'
							echo '	fi;'
							echo '	chmod -R a+r+X '$TARGET_ROOT
							echo
							echo 'uninstall-local:'
							echo '	for file in '$EN_FILES'; do \'
							echo '	  echo Removing '$TARGET_DIR'/$$file; \'
							echo '	  rm -f '$TARGET_DIR'/$$file; \'
							echo '	done;'
							echo '	echo Removing '$TARGET_DIR'/common;'
							echo '	rm -f '$TARGET_DIR'/common;'
						fi;
					) > $I2/Makefile.am
				fi # html
			done
		fi  # PART exists?
	done ### PART
done ### FOREIGN_DOC_LANGUAGE


