#!/bin/bash -x

PACKAGE_VERSION=$1
PACKAGE_DIR=/build/packaging/fedora-25
BUILD_DIR=/build/target/
PACKAGE_BUILD_DIR=/build/target/fedora-25
RPM_SPECS_DIR=$PACKAGE_BUILD_DIR/rpm/SPECS/
RPM_BUILD_DIR=$PACKAGE_BUILD_DIR/rpm/BUILD/
RPM_SOURCES_DIR=$PACKAGE_BUILD_DIR/rpm/SOURCES
RPM_TMP_DIR=$PACKAGE_BUILD_DIR/rpm/tmp
SRC_DIR=$PACKAGE_DIR/src

rm -rf $PACKAGE_BUILD_DIR && mkdir -p $PACKAGE_BUILD_DIR/rpm/{BUILD,RPMS,SOURCES,SPECS,SRPMS,tmp}

cat > $HOME/.rpmmacros <<EOB
%packager YOUR_NAME
%_topdir $PACKAGE_BUILD_DIR/rpm
%_tmppath $RPM_TMP_DIR
EOB

cp $SRC_DIR/RPM/mechanic.spec $RPM_SPECS_DIR/
sed -i "s,Version: .*,Version: $PACKAGE_VERSION," $RPM_SPECS_DIR/mechanic.spec

cd /build
tar cv src Makefile --exclude=.git --exclude=target | gzip -c > $RPM_SOURCES_DIR/mechanic-sources.tar.gz

cp -R $SRC_DIR/* $RPM_BUILD_DIR
tar xvfz $BUILD_DIR/bundle.tgz -C $RPM_BUILD_DIR
cd $PACKAGE_BUILD_DIR/rpm
rpmbuild -bb SPECS/mechanic.spec
cp $PACKAGE_BUILD_DIR/rpm/RPMS/x86_64/mechanic-${PACKAGE_VERSION}-0.x86_64.rpm $BUILD_DIR

chown ${FIX_OWNER_UID} -R $PACKAGE_BUILD_DIR $BUILD_DIR/mechanic-${PACKAGE_VERSION}-0.x86_64.rpm

rpm --install $BUILD_DIR/mechanic-${PACKAGE_VERSION}-0.x86_64.rpm

/usr/sbin/mechanic version


