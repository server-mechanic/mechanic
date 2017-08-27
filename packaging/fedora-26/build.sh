#!/bin/bash -x

PACKAGE_VERSION=$1
PACKAGE_MAJOR_VERSION=$2
PACKAGE_BUILD_NUMBER=$3
PACKAGE_DIR=/build/packaging/fedora-26
BUILD_DIR=/build/target/
PACKAGE_BUILD_DIR=/build/target/fedora-26
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

$SRC_DIR/RPM/mechanic.spec.sh $BUILD_DIR/bundle.tgz ${PACKAGE_MAJOR_VERSION} ${PACKAGE_BUILD_NUMBER} >> $RPM_SPECS_DIR/mechanic.spec
cat $RPM_SPECS_DIR/mechanic.spec

cd /build
tar cv src Makefile --exclude=.git --exclude=target | gzip -c > $RPM_SOURCES_DIR/mechanic-sources.tar.gz

cp -R $SRC_DIR/* $RPM_BUILD_DIR
echo "Extracting bundle to $RPM_BUILD_DIR"
tar xvfz $BUILD_DIR/bundle.tgz -C $RPM_BUILD_DIR
cd $PACKAGE_BUILD_DIR/rpm
rpmbuild --target=noarch -bb SPECS/mechanic.spec
cp $PACKAGE_BUILD_DIR/rpm/RPMS/noarch/mechanic-${PACKAGE_VERSION}.noarch.rpm $BUILD_DIR

chown ${FIX_OWNER_UID} -R $PACKAGE_BUILD_DIR $BUILD_DIR/mechanic-${PACKAGE_VERSION}.noarch.rpm

rpm --install $BUILD_DIR/mechanic-${PACKAGE_VERSION}.noarch.rpm

/usr/bin/mechanic version


