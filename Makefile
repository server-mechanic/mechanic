PACKAGE_VERSION := 0.2
BUILD_NUMBER := 3
SCM_VERSION = $(shell git rev-parse HEAD)

default:	clean generate compile tests integration-tests

prerequisites:
	if [ -f /etc/fedora-release ]; then \
		sudo dnf -y install splint make gcc sqlite-devel; \
	elif [ -f /etc/debian-version ]; then \
		sudo apt-get update && \
		apt-get install -y git gcc splint build-essential sqlite3 libsqlite3-dev; \
	fi

all:	clean generate compile tests integration-tests coverage packages

.PHONY:	clean
clean:
	@echo "Cleaning up..."; \
	rm -rf target/ ${PWD}/src/*.o

generate:
	sed -ri 's,^(.*PACKAGE_VERSION\s*\")([^"]*)(\".*)$$,\1$(PACKAGE_VERSION)\3,g' ${PWD}/include/mechanic/metadata.h
	sed -ri 's,^(.*SCM_VERSION\s*\")([^"]*)(\".*)$$,\1$(SCM_VERSION)\3,g' ${PWD}/include/mechanic/metadata.h
	
compile:	
	sqlite3 -version
	mkdir -p ${PWD}/target/
	cd src && \
	gcc -Wall -g -I ../include/ -c *.c && \
	gcc $$(find . -name "*.o" | grep -v _test.o) -lm -lsqlite3 -o ../target/mechanic && \
	for i in *_test.c; do \
		gcc $$(find . -name "*.o" | grep -v mechanic.o) -lm -lsqlite3 -o ../target/$$(basename $$i .c); \
	done

tests:
	@echo "Running tests..."; \
	for i in $$(find ${PWD}/target/ -name "*_test"); do \
		echo $$(basename $$i); \
	done

.PHONY:	integration-tests
integration-tests:
	@echo "Running integration tests..."; \
	${PWD}/scripts/run-integration-tests.sh

coverage:
	splint -weak -f ${PWD}/splintrc -I ${PWD}/include/ $$(find ${PWD}/src -name "*.c" | grep -v _test.c | grep -v log.c)

packages:	ubuntu-xenial ubuntu-yakkety fedora-25 centos-7

ubuntu-xenial:
	${PWD}/scripts/build-package.sh ubuntu-xenial $(PACKAGE_VERSION)-0ubuntu-xenial${BUILD_NUMBER}

ubuntu-yakkety:
	${PWD}/scripts/build-package.sh ubuntu-yakkety $(PACKAGE_VERSION)-0ubuntu-yakkety${BUILD_NUMBER}

centos-7:
	${PWD}/scripts/build-package.sh centos-7 $(PACKAGE_VERSION).centos7.${BUILD_NUMBER}

fedora-25:
	${PWD}/scripts/build-package.sh fedora-25 $(PACKAGE_VERSION).fedora25.${BUILD_NUMBER}


