Name: mechanic
Version: 1
Release: 0
Summary: Simple server migration tool.
Source0: mechanic-sources.tar.gz
License: GPL
Group: Rahul
BuildArch: x86_64
BuildRoot: %{_tmppath}/%{name}-buildroot
%description
Simple server migration tool.
%prep
%build
%install
install -m 0755 -d $RPM_BUILD_ROOT/etc/mechanic
install -m 0755 -d $RPM_BUILD_ROOT/etc/mechanic/migration.d
install -m 0755 -d $RPM_BUILD_ROOT/usr/sbin/
install -m 0755 $RPM_BUILD_ROOT/../../BUILD/mechanic $RPM_BUILD_ROOT/usr/sbin/mechanic 
%clean
rm -rf $RPM_BUILD_ROOT
%post
%files
%dir /etc/mechanic
%dir /etc/mechanic/migration.d
/usr/sbin/mechanic
