# graphviz.spec.  Generated from graphviz.spec.in by configure.

Summary:        Graph Visualization Tools
Name:           graphviz
Version:        2.2.1
Release:        1
Group:          Applications/Graphics
License:        CPL (see COPYING)
URL:            http://www.graphviz.org/
Source:		http://www.graphviz.org/pub/graphviz/ARCHIVE/%{name}-%{version}.tar.gz
Packager:       John Ellson (ellson@graphviz.org)
Vendor:         http://www.graphviz.org/

# SuSE uses a different mechanism to generate BuildRequires
# norootforbuild
# neededforbuild  expat freetype2 freetype2-devel gcc libjpeg libpng-devel-packages tcl tcl-devel tk tk-devel x-devel-packages

BuildRequires: tcl tk expat expat-devel libjpeg libjpeg-devel libpng libpng-devel zlib zlib-devel
%{?fc3:BuildRequires: freetype >= 2 freetype-devel fontconfig fontconfig-devel tcl-devel tk-devel xorg-x11-devel} 
%{?fc2:BuildRequires: freetype >= 2 freetype-devel fontconfig fontconfig-devel tcl-devel tk-devel XFree86-devel}
%{?fc1:BuildRequires: freetype >= 2 freetype-devel fontconfig fontconfig-devel tcl-devel tk-devel XFree86-devel}
%{?el3:BuildRequires: freetype >= 2 freetype-devel fontconfig fontconfig-devel tcl-devel tk-devel XFree86-devel}
%{?rh9:BuildRequires: freetype >= 2 freetype-devel XFree86-devel}
%{?rh8:BuildRequires: freetype >= 2 freetype-devel XFree86-devel}
%{?rh7:BuildRequires: freetype >= 2 freetype-devel XFree86-devel}
%{?el2:BuildRequires: freetype >= 2 freetype-devel XFree86-devel}
%{?rh6:BuildRequires: freetype >= 2 freetype-devel XFree86-devel}

BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-buildroot
Prefix:         %{_prefix}

%description
A collection of tools and tcl packages for the manipulation and layout
of graphs (as in nodes and edges, not as in barcharts).

%package graphs
Group:		Applications/Graphics
Summary:	Demo graphs for %{name}

%description graphs
This package provides some example graphs for %{name}.

%package tcl
Group:		Applications/Graphics
Summary:	Tcl extension tools for version %{version} of %{name}
Requires:	%{name} = %{version}-%{release}

%description tcl
The %{name}-tcl package contains the various tcl packages (extensions)
for version %{version} of the %{name} tools.

%package devel
Group:		Development/Libraries
Summary:	Development tools for using the %{name} libraries.
Requires:	%{name} = %{version}-%{release} %{name}-tcl = %{version}-%{release}
# The following doesn't work on RH9 or RHE where there is no tcl-devel
#Requires: tcl-devel tk-devel

%description devel
The %{name}-devel package contains the header files
and man3 pages necessary for developing programs
using version %{version} of the %{name} libraries.

%package doc
Group:          Documentation
Summary:        PDF and HTML documents for %{name}

%description doc
The %{name}-doc package provides some additional PDF and HTML documentation for %{name}.

%prep
%setup -q

%build
%ifarch %ix86
%define optflags -DNDEBUG -O3 -march=%{_target_cpu} -mcpu=i686 -ffast-math
%endif
# <percent>configure is broken in RH7.3 rpmbuild - even in comments!
#<percent>configure --with-x
./configure \
      --prefix=%{_prefix} \
      --bindir=%{_bindir} \
      --libdir=%{_libdir} \
      --includedir=%{_includedir} \
      --datadir=%{_datadir} \
      --mandir=%{_mandir} \
      --with-x \
      --with-mylibgd
# need gd-2.0.29 commonly available in rpms before removing --enable-mylibgd
%__make %{?_smp_mflags}

%install
make DESTDIR="$RPM_BUILD_ROOT" install

%files
%defattr(-,root,root)
%doc AUTHORS COPYING ChangeLog NEWS
%{_bindir}/*
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/lefty
%{_mandir}/man1/*
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/lib*.so.*
%exclude %{_libdir}/%{name}/lib*tcl*.so.*
%exclude %{_libdir}/%{name}/libtk*.so.*

%files graphs
%defattr(-,root,root)
%{_datadir}/%{name}/graphs

%files tcl
%defattr(-,root,root)
%doc AUTHORS COPYING ChangeLog
%{_libdir}/%{name}/lib*tcl*.so.*.*.*
%{_libdir}/%{name}/libtk*.so.*.*.*
%{_libdir}/%{name}/pkgIndex.tcl
%{_datadir}/%{name}/demo
%{_mandir}/mann/* 

%files devel
%defattr(-,root,root)
%doc AUTHORS COPYING
%{_includedir}/%{name}
%{_libdir}/%{name}/*.*a
%{_libdir}/%{name}/*.so
%dir %{_libdir}/%{name}/pkgconfig
%{_libdir}/%{name}/pkgconfig/*.pc
%{_mandir}/man3/*
%exclude %{_libdir}/%{name}/lib*tcl*.*
%exclude %{_libdir}/%{name}/libtk*.*

%files doc
%defattr(-,root,root)
%doc AUTHORS COPYING
%doc cpl1.0.txt
%{_datadir}/%{name}/doc

%clean
%__rm -rf "$RPM_BUILD_ROOT"
