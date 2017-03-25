#!/bin/bash

BUNDLE_TGZ=$1

cat - <<EOB
%define _unpackaged_files_terminate_build 0
%define _missing_doc_files_terminate_build 0

Name: mechanic
Version: 1
Release: 0
Summary: Simple server migration tool.
Source0: mechanic-sources.tar.gz
License: GNU General Public License, Version 3
Group: Tools
BuildArch: x86_64
BuildRoot: %{_tmppath}/%{name}-buildroot
Requires: python >= 2.7, tar >= 1.0, sqlite >= 3.14.0
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
echo "%__install -D -m 0644 .${file} %{buildroot}/${file}"
done)
echo "%__install -D -m 0755 usr/bin/mechanic %{buildroot}/usr/bin/mechanic"

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
