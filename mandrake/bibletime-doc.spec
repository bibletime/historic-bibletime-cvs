%define         name 		bibletime-doc
%define         version 	1.1
%define		release 	1mdk8.2

# This should be set to your os.
# Possible values are  Mandrake, Red Hat, Turbolinux, Caldera, SuSE, Debian, etc.
%define         ostype          Mandrake

# This should be set to the version of your OS (6.0, 6.1, 6.2, 7.0, 7.1, 7.2, 8
%define         osversion       8.2

# This is your cpu i486, i586, i686, ppc, sparc, alfa, etc.
%define         buildarch       noarch

# This the RPM group on your system that this will installed into.
# Graphical desktop/KDE, X11/apps, etc.
%define         rpmgroup        Graphical desktop/KDE

Summary:        BibleTime %{version} is an easy to use Bible study tool for KDE2.
Name:		%{name}
Version:        %{version}
Release:        %{release}
#Icon:		bibletime.xpm
Copyright:      GPL
Vendor:         The team of BibleTime <info@bibletime.de>
Url:            http://www.bibletime.de
Packager:       Brook Humphrey <bah@webmedic.net>, Joachim Ansorg <jansorg@gmx.de>
Group:          %{rpmgroup}
BuildArch:      %{buildarch}
Source0:        %{name}-%{version}.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}--rootbibletime
Prefix: 	/usr

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



%setup -q
make -f Makefile.cvs
./configure;
make

%install
make prefix=$RPM_BUILD_ROOT%{prefix} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
#Sword config files
# global BibleTime config files
%{prefix}/share/doc/HTML/*/bibletime/*
%{prefix}/share/locale/*/LC_MESSAGES/bibletime*


