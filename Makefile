#
# Copyright (c) 2016 the Server Mechanic Team.
# Server Mechanic (http://server-mechanic.org)
#
# This file is part of Server Mechanic.
#
# Server Mechanic is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#

PACKAGE_VERSION := 0.3
BUILD_NUMBER := 2
SCM_VERSION = $(shell git rev-parse HEAD)

default:	clean generate compile tests bundle integration-tests

prerequisites:
	if [ -f /etc/fedora-release ]; then \
		sudo dnf -y install splint make gcc sqlite-devel; \
	elif [ -f /etc/debian-version ]; then \
		sudo apt-get update && \
		apt-get install -y git gcc splint build-essential sqlite3 libsqlite3-dev; \
	fi

all:	clean generate compile tests bundle integration-tests coverage packages

.PHONY:	clean
clean:
	@echo "Cleaning up..."; \
	rm -rf target/ ${PWD}/src/*.o

generate:
	@echo "Generating metadata..."; \
	cat ${PWD}/src/include/metadata.h.in \
	| sed -r 's,^(.*VERSION\s*\")([^"]*)(\".*)$$,\1$(PACKAGE_VERSION)-$(BUILD_NUMBER)\3,g' \
	| sed -r 's,^(.*SCM_VERSION\s*\")([^"]*)(\".*)$$,\1$(SCM_VERSION)\3,g' \
	> ${PWD}/src/include/metadata.h
	
compile:	
	@mkdir -p ${PWD}/target/bin/ && \
	cd src && \
	echo "Compiling..." && \
	gcc -Wall -Wpedantic -I ./include/ -c *.c && \
	echo "Linking..." && \
	gcc $$(find . -name "*.o" | grep -v _test.o) -lm -lsqlite3 -o ../target/bin/mechanic && \
	for i in $$(find . -name "*_test.c"); do \
		echo "Linking Test $$(basename $$i .c)..." && \
		gcc $$(basename $$i .c).o $$(find . -name "*.o" | grep -v _test.o | grep -v mechanic.o) -lm -lsqlite3 -o ../target/bin/$$(basename $$i .c); \
	done

tests:
	@echo "Running tests..."; \
	for i in $$(find ${PWD}/target/bin/ -name "*_test"); do \
		echo $$(basename $$i); \
		$$i; \
	done

bundle:
	@echo "Preparing bundle..." && \
	cp -R ${PWD}/src/bundle ${PWD}/target/ && \
	mkdir -p ${PWD}/src/bundle/usr/sbin/ && \
	cp ${PWD}/target/bin/mechanic ${PWD}/target/bundle/usr/sbin && \
	cd ${PWD}/target/bundle && tar czf ../bundle.tgz .

.PHONY:	integration-tests
integration-tests:
	@echo "Running integration tests..."; \
	${PWD}/scripts/run-integration-tests.sh

coverage:
	splint -weak -f ${PWD}/splintrc -I ${PWD}/include/ $$(find ${PWD}/cli/src -name "*.c" | grep -v _test.c | grep -v log.c)

packages:	debian-jessie ubuntu-xenial ubuntu-yakkety fedora-25 centos-7

debian-jessie:
	${PWD}/scripts/build-package.sh debian-jessie $(PACKAGE_VERSION)-${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-xenial:
	${PWD}/scripts/build-package.sh ubuntu-xenial $(PACKAGE_VERSION)-0ubuntu-xenial${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-yakkety:
	${PWD}/scripts/build-package.sh ubuntu-yakkety $(PACKAGE_VERSION)-0ubuntu-yakkety${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

centos-7:
	${PWD}/scripts/build-package.sh centos-7 $(PACKAGE_VERSION).centos7.${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

fedora-25:
	${PWD}/scripts/build-package.sh fedora-25 $(PACKAGE_VERSION).fedora25.${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}


