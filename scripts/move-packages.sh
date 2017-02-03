#!/bin/bash

PACKAGES_PROJECT_DIR=$1
if [ ! -d "$PACKAGES_PROJECT_DIR/apt" ]; then
  echo "Wrong packages project path."
  exit 1
fi

MECHANIC_PROJECT_DIR=$(cd `dirname $0`/..; pwd)
if [ ! -d "$MECHANIC_PROJECT_DIR/target/" ]; then
  echo "No mechanic build dir target/ found."
  exit 1
fi

mv $MECHANIC_PROJECT_DIR/target/*ubuntu-yakkety* $PACKAGES_PROJECT_DIR/apt/ubuntu/dists/yakkety/unstable/binary-all/
mv $MECHANIC_PROJECT_DIR/target/*ubuntu-xenial* $PACKAGES_PROJECT_DIR/apt/ubuntu/dists/xenial/unstable/binary-all/
mv $MECHANIC_PROJECT_DIR/target/*wheezy* $PACKAGES_PROJECT_DIR/apt/debian/dists/wheezy/unstable/binary-all/
mv $MECHANIC_PROJECT_DIR/target/*jessie* $PACKAGES_PROJECT_DIR/apt/debian/dists/jessie/unstable/binary-all/
mv $MECHANIC_PROJECT_DIR/target/*sid* $PACKAGES_PROJECT_DIR/apt/debian/dists/sid/unstable/binary-all/

