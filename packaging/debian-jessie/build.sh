#!/bin/bash

PACKAGE_VERSION=$1
BUILD_DIR=/build/target/
PACKAGE_BUILD_DIR=/build/target/debian-jessie
PACKAGE_FILE=$BUILD_DIR/mechanic_${PACKAGE_VERSION}_all.deb
PACKAGE_COMMON_SRC_DIR=/build/packaging/common/src
PACKAGE_SRC_DIR=/build/packaging/debian-jessie/src

rm -rf $PACKAGE_BUILD_DIR && mkdir -p $PACKAGE_BUILD_DIR

tar xvfz $BUILD_DIR/bundle.tgz -C $PACKAGE_BUILD_DIR
cp -R $PACKAGE_SRC_DIR/* $PACKAGE_BUILD_DIR
chown root.root -R $PACKAGE_BUILD_DIR/*
INSTALLED_SIZE=$(du -sb $PACKAGE_BUILD_DIR | cut -f 1)

sed -i "s,Version: .*,Version: $PACKAGE_VERSION," $PACKAGE_BUILD_DIR/DEBIAN/control
sed -i "s,Installed-Size: .*,Installed-Size: $INSTALLED_SIZE," $PACKAGE_BUILD_DIR/DEBIAN/control

dpkg -b $PACKAGE_BUILD_DIR $PACKAGE_FILE

chown ${FIX_OWNER_UID} -R $PACKAGE_BUILD_DIR $PACKAGE_FILE

dpkg --install $PACKAGE_FILE
/usr/sbin/mechanic version 
/usr/sbin/mechanic -v migrate -- /bin/true

