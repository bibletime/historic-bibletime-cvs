%define         name 		bibletime
%define         version 	1.3beta2

# This should not be changed but set this to static or dynamic.
# Sword should always be set to static for these builds.
%define		swordlibs 	static

# this needs to be changed depending on build number 
# and weather or not it is static
%define		release 	1mdk9.0

# This should be set to your os.
# Possible values are  Mandrake, Red Hat, Turbolinux, Caldera, SuSE, Debian, etc.
%define		ostype 		Mandrake

# This should be set to the version of your OS (6.0, 6.1, 6.2, 7.0, 7.1, 7.2, 8.0, etc.)
%define		osversion 	9.0

# This is your cpu i486, i586, i686, ppc, sparc, alfa, etc.
%define		buildarch 	i586

# This the RPM group on your system that this will installed into.
# Graphical desktop/KDE, X11/apps, etc.
%define		rpmgroup 	Graphical desktop/KDE

# this is were the sword modules will be installed on your system.
# /usr/local/sword, /usr/share/sword, /opt/local/sword, etc.
# default is /usr/share/sword
%define     SwordPath 	/usr/share/sword
%define	    ModsPath 	%{SwordPath}/mods.d

# This for Mandrake menus if you have another system do not edit this line. it is for
# compatability with those systems.
%define		build_menus	1
%define		_menudir 	/usr/lib/menu 
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
%if %build_menus
Source1:        bibletime_icons.tar.bz2
%endif

# This source comes from sword and will conflict woth sword if it is installed so we will make it a seperarte package.
Source2:		locales.d.tar.bz2

BuildRoot:      %{_tmppath}/%{name}-%{version}--rootbibletime
Provides:       sword-base sword = %{libversion}
Prefix: 		%{_prefix} 

%description
BibleTime %{version}-%{release} is a free and easy to use bible study tool for UNIX systems.

BibleTime provides easy handling of digitalized texts (Bibles, commentaries and lexicons) and powerful features to work with these texts (search in texts, write own notes, save, print etc.). Bibletime is a frontend for the  SWORD Bible Framework.

This is  a relocatable package, you can install it on different target with
rpm -Uvh bibletime-xxx.rpm --prefix /usr/local/opt/apps/noncommercial
default is %{_prefix}

The SWORD Project is an effort to create an ever expanding software package for research and study of God and His Word.  The SWORD Bible Framework allows easy manipulation of Bible texts, commentaries, lexicons, dictionaries, etc.  Many frontends are build using this framework.  An installed module set may be shared between any frontend using the framework.

This rpm was compiled on a %{ostype} %{osversion} system for %{buildarch} class cpu's.

This RPM has these compiled options Sword is compiled %{swordlibs} and KDE/QT is compiled %{kdelibs}.

Although this binary no longer requires sword to run the source.rpm will not build correctly
without the sword-devel package installed.

%prep
rm -rf $RPM_BUILD_ROOT
rm -rf $RPM_BUILD_DIR/%{name}-%{version}

# For use with mandrake menu system
mkdir -p $RPM_BUILD_ROOT%{_menudir}

mkdir -p $RPM_BUILD_ROOT/usr/share/config

# generate directories for sword if building static
if [ %{swordlibs} = "static" ] ; then
 mkdir -p $RPM_BUILD_ROOT%{SwordPath}
 mkdir -p $RPM_BUILD_ROOT%{ModsPath}
fi


%setup -q
tar jxvf %{SOURCE2}
if [ %{ostype} = "Mandrake" ] ; then
 %if %build_menus
  #Icons for Mandrake menu system
  tar jxvf %{SOURCE1}
 %endif
 export KDEDIR=/usr QTDIR=/usr/lib/qt2
elif [%{ostype} = "SuSE"] ; then
 export KDEDIR=/opt/kde2 QTDIR=/usr/lib/qt2
else
	echo "Using $KDEDIR as KDE2 directory";
	echo "Using $QTDIR as QT 2.2.x directory";
	echo "If the directories are not correct please set them using \"export KDEDIR=<KDE2 directory> QTDIR=<QT2.2 directory>\""
	sleep 5
fi

# uncomment this to complie from cvs
#make -f Makefile.cvs

if [ %{swordlibs} = "static" ] ; then
        ./configure --enable-static-sword; #--enable-static-linking;
else
        ./configure;
fi

make -j2

%install
make prefix=$RPM_BUILD_ROOT%{prefix} install

# Copy Sword language.conf files to our install dir
mkdir -p $RPM_BUILD_ROOT%{SwordPath}/locales.d
cp locales.d/*.conf $RPM_BUILD_ROOT%{SwordPath}/locales.d/

# Create Sword globals.conf for using bibletime without sword installed.
cat << EOF >$RPM_BUILD_ROOT%{ModsPath}/globals.conf
 [Globals]
EOF

# For use with mandrake menu system 
cat << EOF > $RPM_BUILD_ROOT%{_menudir}/%{name}
?package(bibletime):command="/usr/bin/bibletime" icon="bibletime.png" \
                needs="X11" section="Applications/Bible Study/" title="Bibletime" \
                longtitle="An easy to use Bible study tool."
EOF

# For use with mandrake menu system 
cat << EOF > $RPM_BUILD_ROOT%{_menudir}/%{name}-setupwizard
?package(bibletime):command="/usr/bin/btsetupwizard" icon="bibletime.png" \
                needs="X11" section="Applications/Bible Study/" title="Bibletime Setup Wizard" \
                longtitle="A setup tool for the easy to use Bible study tool."
EOF

# For use with mandrake menu system 
# icons: this is for the mandrake menu system
%if %build_menus
  mkdir -p $RPM_BUILD_ROOT%{_liconsdir}
  mkdir -p $RPM_BUILD_ROOT%{_iconsdir}
  mkdir -p $RPM_BUILD_ROOT%{_miconsdir}
  cd $RPM_BUILD_DIR/%{name}-%{version}
  install -m 644 hi16-app-%{name}.png $RPM_BUILD_ROOT%{_miconsdir}/%{name}.png
  install -m 644 hi32-app-%{name}.png $RPM_BUILD_ROOT%{_iconsdir}/%{name}.png
  install -m 644 hi48-app-%{name}.png $RPM_BUILD_ROOT%{_liconsdir}/%{name}.png
%endif 

%post
# this is for creation of the sword.conf for systems that are using bibletime without Sword installed.
if [ -f /etc/sword.conf ] ; then
         echo "sword.conf already exists skiping sword.conf creation." ;
else
cat << EOF >/etc/sword.conf
[Install]
DataPath=%{SwordPath}
EOF
fi

/sbin/ldconfig

# For use with mandrake menu system
if [ %{ostype} = "Mandrake" ] ; then
 %{update_menus}
fi

%postun

# For use with mandrake menu system
if [ %{ostype} = "Mandrake" ] ; then
 %{clean_menus}
fi

/sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
#Sword config files
%config %{ModsPath}/globals.conf
%config %{SwordPath}/locales.d/*.conf
# global BibleTime config files
#%config %{prefix}/share/config/bt-printing
%doc %{prefix}/share/doc/HTML/*
%{prefix}/bin/*
%{prefix}/share/applnk/Applications/*
%{prefix}/share/apps/bibletime/*
%{prefix}/share/icons/*
# For use with mandrake menu system
%if %build_menus
%{_menudir}/%{name}
%{_menudir}/%{name}-setupwizard
%endif

%changelog
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

