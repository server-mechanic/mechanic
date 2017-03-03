#!/bin/bash

PROJECT_DIR=$(cd `dirname $0`/..; pwd)

cd $PROJECT_DIR
docker build -t mechanic-build -f $PROJECT_DIR/build-container/Dockerfile $PROJECT_DIR/build-container
docker run --rm \
	-e FIX_UID=$(id --user $USER) \
	-e FIX_GID=$(id --group $USER) \
	-v $PROJECT_DIR:/build \
	mechanic-build \
	/bin/bash -xc "cd /build && make clean compile integration-tests && chown -R \$FIX_UID.\$FIX_GID /build"
