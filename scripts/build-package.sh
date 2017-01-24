#!/bin/bash
#
# Copyright (c) 2016,2017 the Server Mechanic Team.
# Server Mechanic (http://server-mechanic.org)
#
# This file is part of Server Mechanic.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#

PROJECT_DIR=$(cd `dirname $0`/..; pwd)

pkg=$1
PACKAGE_VERSION=$2
echo "Building package $pkg"
cd ${PROJECT_DIR}/packaging/$pkg
docker build --tag mechanic-build-$pkg .
cd $PROJECT_DIR
docker run --rm \
	-e FIX_OWNER_UID=$UID \
	-v ${PROJECT_DIR}:/build \
	mechanic-build-$pkg /bin/bash /build/packaging/$pkg/build.sh $PACKAGE_VERSION $3 $4
