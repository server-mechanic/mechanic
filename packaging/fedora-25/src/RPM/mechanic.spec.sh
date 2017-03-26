#!/bin/bash

BUNDLE_TGZ=$1
PACKAGE_VERSION=$2
PACKAGE_RELEASE=$3

cat - <<EOB
%define _unpackaged_files_terminate_build 0
%define _missing_doc_files_terminate_build 0

Name: mechanic
Version: ${PACKAGE_VERSION}.fedora25 
Release: ${PACKAGE_RELEASE}
Packager: Cornelius Buschka <cbuschka@gmail.com>
Summary: Simple server migration tool.
Source0: mechanic-sources.tar.gz
License: GNU General Public License, Version 3
Group: Tools
BuildArch: noarch
BuildRoot: %{_tmppath}/%{name}-buildroot
Requires: python >= 2.7, sqlite >= 3.14.0
%description
Simple server migration tool.
%prep
%build
%install
EOB

tar tfz $BUNDLE_TGZ | sed -r 's#\./(.*)$#/\1#g' | grep mechanic | grep -G '^.*/$' | (while read file; do 
echo "%__install -D -m 0755 -d %{buildroot}${file}"
done)

tar tfz $BUNDLE_TGZ | sed -r 's#\./(.*)$#/\1#g' | grep mechanic | grep -v -G '^.*/$' | grep -v usr/bin/mechanic | (while read file; do 
echo "%__install -D -m 0644 .${file} %{buildroot}${file}"
done)
echo "%__install -D -m 0755 ./usr/bin/mechanic %{buildroot}/usr/bin/mechanic"

cat - <<EOB
%clean
%post
%files
EOB

tar tfz $BUNDLE_TGZ | sed -r 's#\./(.*)$#/\1#g' | grep mechanic | grep -v -G '^/$' | grep -v share | grep -v -G '^/usr/s?bin/$' | grep -G '^.*/$' | (while read file; do 
echo "%dir ${file}"
done)
tar tfz $BUNDLE_TGZ | sed -r 's#\./(.*)$#/\1#g' | grep mechanic | grep -v share | grep -v -G '^.*/$' | (while read file; do 
echo "${file}"
done)
