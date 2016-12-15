#!/bin/bash

PROJECT_DIR=$(cd `dirname $0`/..; pwd)

pkg=$1
PACKAGE_VERSION=$2
echo "Building package $pkg"
cd ${PROJECT_DIR}/packaging/$pkg
docker build --tag mechanic-build-$pkg .
cd $PROJECT_DIR
docker run --rm \
	-e FIX_OWNER_UID=$UID \
	--name mechanic-build-$pkg \
	-v ${PROJECT_DIR}:/build \
	mechanic-build-$pkg /build/packaging/$pkg/build.sh $PACKAGE_VERSION
