%{expand:%%define buildfor %(A=$(awk '{print $4}' /etc/mandrake-release); if [ "$A" = 9.0 ]; then echo 9.0; else echo 9.1; fi)}
%{expand:%%define buildfor9_0 %(A=$(awk '{print $4}' /etc/mandrake-release); if [ "$A" = 9.0 ]; then echo 1; else echo 0; fi)}
%{expand:%%define buildfor9_1 %(A=$(awk '{print $4}' /etc/mandrake-release); if [ "$A" = 9.1 ]; then echo 1; else echo 0; fi)}

%if %buildfor9_1
%define 		unstable 	0
%else
%define 		unstable 	0
%endif

%define         name 		bibletime
%define         version 	1.3beta4

# This should not be changed but set this to static or dynamic.
# Sword should always be set to 1 if version is wanted that
# deos not require sword to be installed.
%define			swordlibs	0
%define			kdelibs		0
%define			buildwizard	0

# this needs to be changed depending on build number 
# and weather or not it is static
%define			release 	6mdk%{buildfor}

# This is your cpu i486, i586, i686, ppc, sparc, alfa, etc.
%define			buildarch 	i586

# This the RPM group on your system that this will installed into.
# Graphical desktop/KDE, X11/apps, etc.
%define			rpmgroup 	Graphical desktop/KDE

# this is were the sword modules will be installed on your system.
# /usr/local/sword, /usr/share/sword, /opt/local/sword, etc.
# default is /usr/share/sword
%define     	SwordPath 	/usr/share/sword
%define	    	ModsPath 	%{SwordPath}/mods.d

# This for Mandrake menus
%define			_menudir 	/usr/lib/menu 
# Nothing else should need to be changed.
# Please do not edit below this line unless you know what you are doing.

Summary:        BibleTime %{version} is an easy to use Bible study tool for KDE2.
Name:			%{name}
Version:        %{version}
Release:        %{release}
Icon:			bibletime.xpm
Copyright:      GPL
Vendor:         The team of BibleTime <info@bibletime.de>
Url:            http://www.bibletime.de
Packager:       Brook Humphrey <bah@webmedic.net>, Joachim Ansorg <jansorg@gmx.de>
Group:          %{rpmgroup}
BuildArch:      %{buildarch}
Source0:        %{name}-%{version}.tar.bz2

# For use with mandrake menu system. 
Source1:        bibletime_icons.tar.bz2

# This source comes from sword and will conflict woth sword if it is installed so we will make it a seperarte package.
Source2:		locales.d.tar.bz2

BuildRoot:      %{_tmppath}/%{name}-%{version}--rootbibletime
%if %swordlibs
	Provides:       sword-base sword = %{libversion}
%endif 
Prefix: 		%{_prefix} 

%description
BibleTime %{version}-%{release} is a free and easy to use bible study tool for UNIX systems.

BibleTime provides easy handling of digitalized texts (Bibles, commentaries and lexicons) and powerful features to work with these texts (search in texts, write own notes, save, print etc.). Bibletime is a frontend for the  SWORD Bible Framework.

This is  a relocatable package, you can install it on different target with
rpm -Uvh bibletime-xxx.rpm --prefix /usr/local/opt/apps/noncommercial
default is %{_prefix}

The SWORD Project is an effort to create an ever expanding software package for research and study of God and His Word.  The SWORD Bible Framework allows easy manipulation of Bible texts, commentaries, lexicons, dictionaries, etc.  Many frontends are build using this framework.  An installed module set may be shared between any frontend using the framework.

%prep
rm -rf $RPM_BUILD_ROOT
rm -rf $RPM_BUILD_DIR/%{name}-%{version}

# For use with mandrake menu system
mkdir -p $RPM_BUILD_ROOT%{_menudir}

mkdir -p $RPM_BUILD_ROOT/usr/share/config

# generate directories for sword if building static
%if %swordlibs
 mkdir -p $RPM_BUILD_ROOT%{SwordPath}
 mkdir -p $RPM_BUILD_ROOT%{ModsPath}
%endif 


%setup -q
#Icons for Mandrake menu system
tar jxvf %{SOURCE1}

%if %swordlibs
tar jxvf %{SOURCE2}
%endif 

%build
export QTDIR=%qtdir
export KDEDIR=%_prefix

export LD_LIBRARY_PATH="$QTDIR:/%_lib:$LD_LIBRARY_PATH"
export PATH="$KDEDIR/bin:$PATH"

# Search for qt/kde libraries in the right directories (avoid patch)
# NOTE: please don't regenerate configure scripts below
perl -pi -e "s@/lib(\"|\b[^/])@/%_lib\1@g if /(kde|qt)_(libdirs|libraries)=/" configure

%ifarch %ix86
export CFLAGS="%optflags" 
export CXXFLAGS="`echo %optflags |sed -e 's/-fomit-frame-pointer//'`" 
%else
export CFLAGS="%optflags" 
export CXXFLAGS="%optflags" 
%endif


./configure --prefix=%_prefix \
			--libdir=%_libdir \
			--sysconfdir=%_sysconfdir \
			--build=%_target_platform \
			--host=%_target_platform \
%if %unstable
			--enable-debug=full \
%else
			--disable-debug \
%endif	
			--enable-final \
%if %kdelibs
			--enable-static-linking \
%endif
			--enable-static \
			--disable-shared \
%if %swordlibs
			--enable-static-sword \
%endif    
			--disable-qt-embedded \
			--disable-palm-top \
			--disable-rpath \
			--with-gnu-ld \
			--with-pic \
			--with-xinerama \
			--x-includes=/usr/X11R6/include \
			--x-libraries=/usr/X11R6/%_lib


%make -j2

%install
make prefix=$RPM_BUILD_ROOT%{prefix} install

%if %swordlibs
# Copy Sword language.conf files to our install dir
mkdir -p $RPM_BUILD_ROOT%{SwordPath}/locales.d
cp locales.d/*.conf $RPM_BUILD_ROOT%{SwordPath}/locales.d/

# Create Sword globals.conf for using bibletime without sword installed.
cat << EOF >$RPM_BUILD_ROOT%{ModsPath}/globals.conf
 [Globals]
EOF
%endif

# For use with mandrake menu system 
cat << EOF > $RPM_BUILD_ROOT%{_menudir}/%{name}
?package(bibletime):command="/usr/bin/bibletime" icon="bibletime.png" \
                needs="X11" section="Applications/Bible Study/" title="Bibletime" \
                longtitle="An easy to use Bible study tool."
EOF


%if %buildwizard
# For use with mandrake menu system 
cat << EOF > $RPM_BUILD_ROOT%{_menudir}/%{name}-setupwizard
?package(bibletime):command="/usr/bin/btsetupwizard" icon="bibletime.png" \
                needs="X11" section="Applications/Bible Study/" title="Bibletime Setup Wizard" \
                longtitle="A setup tool for the easy to use Bible study tool."
EOF
%endif

# make README.RPM:

COMPILER="Compiler:               $(gcc -v 2>& 1|tail -1)"
HARDWARE="Hardware platform:      $(uname -m)"
LIBRARY="Library:                $(rpm -q glibc)"
OSVERSION="Linux Kernel:           $(uname -sr)"
PACKAGER="Packager:               %{packager}"
MDKRELEASE="Linux-Mandrake release: $(cat /etc/mandrake-release)"
RPMVERSION="RPM Version:            $(rpm -q rpm)"

cat <<EOF >>$RPM_BUILD_DIR/%{name}-%{version}/README.RPM
The pure-ftpd rpm packages were created in the following build environment:

$MDKRELEASE
$HARDWARE
$OSVERSION
$LIBRARY
$COMPILER
$RPMVERSION
$PACKAGER

EOF

# For use with mandrake menu system 
# icons: this is for the mandrake menu system
  mkdir -p $RPM_BUILD_ROOT%{_liconsdir}
  mkdir -p $RPM_BUILD_ROOT%{_iconsdir}
  mkdir -p $RPM_BUILD_ROOT%{_miconsdir}
  cd $RPM_BUILD_DIR/%{name}-%{version}
  install -m 644 hi16-app-%{name}.png $RPM_BUILD_ROOT%{_miconsdir}/%{name}.png
  install -m 644 hi32-app-%{name}.png $RPM_BUILD_ROOT%{_iconsdir}/%{name}.png
  install -m 644 hi48-app-%{name}.png $RPM_BUILD_ROOT%{_liconsdir}/%{name}.png
 

%post
%if %swordlibs
# this is for creation of the sword.conf for systems that are using bibletime without Sword installed.
if [ -f /etc/sword.conf ] ; then
         echo "sword.conf already exists skiping sword.conf creation." ;
else
cat << EOF >/etc/sword.conf
[Install]
DataPath=%{SwordPath}
EOF
fi
%endif

/sbin/ldconfig

# For use with mandrake menu system
%{update_menus}

%postun
# For use with mandrake menu system
%{clean_menus}

/sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT
rm -rf $RPM_BUILD_DIR/%{name}-%{version}


%files
%defattr(-,root,root)

%if %swordlibs
#Sword config files
%config %{ModsPath}/globals.conf
%config %{SwordPath}/locales.d/*.conf
%endif

# global BibleTime config files
#%config %{prefix}/share/config/bt-printing

#docs
%doc %{prefix}/share/doc/HTML/*
%doc README.RPM

%{prefix}/bin/*
%{prefix}/share/applnk/Applications/*
%{prefix}/share/apps/bibletime/*
%{prefix}/share/icons/*

# For use with mandrake menu system
%{_menudir}/*

%changelog
* Wed Jan 21 2003 Brook Humphrey <bah@webmedic.net> bibleitme-1.3.beta4-1mdk

- Massive clean up of spec file
- hopefully more readable and all arround usable. 

* Fri Dec 13 2002 Brook Humphrey <bah@webmedic.net> bibleitme-1.3.beta2-1mdk9.0

- Betabuild of bibletime for testing only 

* Tue Oct 9 2002 Brook Humphrey <bah@webmedic.net> bibleitme-1.2.2-cvs-1mdk9.0

- Build for mandrake 9.0/ Fixes for gcc 3.2 should allow it to build this time.

- Changed files somewhat as the build droped some and added some.

- Added mandrake menu for bibletime setup wizard.

- Changed the icons to the new hicolor png's for the mandrake menues.

* Fri Aug 2 2002 Brook Humphrey <bah@webmedic.net> bibleitme-1.2.1

- Build for Mandrake 9.0

* Mon Apr 8 2002 Brook Humphrey <bah@webmedic.net> bibletime-1.1

- Added to the spec to better handle mandrake menus during compile. Now should be compatable with all os's.

- added locals.d source to build. This removes dependecy on sword being installed. Will make seperate package for this later.


* Fri Mar 31 2002 Joachim Ansorg <jansorg@gmx.de>

- Made including the book translations working finally.

- Fixed SPEC files to work with SuSE


* Thu Feb 7 2002 Brook Humphrey <bah@webmedic.net> bibletime-1.0.2

- changed compile time options to mach bibletime spec included with source files

- compiled on mandrake 8.2


* Fri Sep 7 2001 Joachim Ansorg <jansorg@gmx.de>

- Added the bookname files in the file list

* Tue Jul 10 2001 Brook Humphrey <bah@webmedic.net> bibletime-1.0.1-Mandrake8.0.4mdk

- Recompile because of dependecy problems.  

* Tue Jul 10 2001 Brook Humphrey <bah@webmedic.net> bibletime-1.0.1-Mandrake8.0.1mdk

- bug fix release bibletime-1.0.1

- adda extra compile options to spec file.

- Merged in suse changes for compileing on SuSE

* Sun May 27 2001 Brook Humphrey <bah@webmedic.net>

- Trying different variations of static compiles again.

- Still using cvs sources while waiting for 1.0 release.

* Thu May 24 2001 Brook Humphrey <bah@webmedic.net>

- Trying for x static and whatever else we can get into it.

- No X for today Got libz to compile in but it causes segfault.

* Wed May 23 2001 Brook Humphrey <bah@webmedic.net>

- Yet another try at static qt.

- Qt static works now.

* Mon May 21 2001 Brook Humphrey <bah@webmedic.net>

- Static is working for kde and a few other libs but not qt yet.

- Testing another patch today to try to get qt compiled static.

* Sun May 20 2001 Brook Humphrey <bah@webmedic.net>

- Cleaned up spec some more(stupid spelling error's)

- static compile not working today so added option to compile kde and sword as static seperately

* Sat May 19 2001 Brook Humphrey <bah@webmedic.net>

- Cleaned up spec some more and added a few more comments

- Changed spec to automaticly do parts concerning static and Mandrake menus

- All info can be set in defines at top of spec file no other changes should need to be made

- Fixed patch to compile static should now compile static with kde libs 

- Test to compile with qt as static also

* Sun May 13 2001 Brook Humphrey <bah@webmedic.net>

- cleaned up spec file and added more comments for easier reading.

- changed sword.conf instelation to check for previous versions of sword.conf

- If found a new sword.conf is not created

- Made a patch to hopefully compile kde and other libs as atatic

- static compile didn't work for today.

* Tue May 8 2001 Brook Humphrey <bah@webmedic.net>

- Updated package to be relocatable

- added sword directories and sword.conf to install

* Mon Apr 30 2001 Brook Humphrey <bah@webmedic.net>

- Changed to build staticly

- added more mandrake macros to spec file

* Mon Apr 23 2001 Brook Humphrey <bah@webmedic.net>

- made changes to enable compile of cvs tree

* Wed Mar 14 2001 Brook Humphrey <bah@webmedic.net>

- updated to use mandrake menu sysem

