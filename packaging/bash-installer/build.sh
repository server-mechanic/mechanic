#!/bin/bash

PACKAGE_VERSION=$1
BUILD_DIR=/build/target/
PACKAGE_BUILD_DIR=/build/target/bash-installer
PACKAGE_FILE=$BUILD_DIR/bash-installer-${PACKAGE_VERSION}.sh
PACKAGE_SRC_DIR=/build/packaging/bash-installer/src

rm -rf $PACKAGE_BUILD_DIR && mkdir -p $PACKAGE_BUILD_DIR

echo "Building bash installer..."
(while read line; do
  if [[ "BUNDLE_DATA_HERE" = "$line" ]]; then
    cat $BUILD_DIR/bundle.tgz | base64
  else
    echo $line
  fi
done)>$PACKAGE_FILE <$PACKAGE_SRC_DIR/installer.sh.template
chmod 755 $PACKAGE_FILE

echo "Installing via $PACKAGE_FILE..."
$PACKAGE_FILE

/usr/sbin/mechanic version

echo "Done."

exit 0
