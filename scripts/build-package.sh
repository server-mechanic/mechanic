#!/bin/bash

PROJECT_DIR=$(cd `dirname $0`/..; pwd)

pkg=$1
PACKAGE_VERSION=$2
echo "Building package $pkg"
cd ${PROJECT_DIR}/packaging/$pkg
docker build --tag mechanic-build-$pkg .
cd ../..
docker run --rm \
	-e FIX_OWNER_UID=$UID \
	-e PACKAGE_VERSION=${PACKAGE_VERSION} \
	--name mechanic-build-$pkg \
	-v ${PWD}/target:/target \
	mechanic-build-$pkg
