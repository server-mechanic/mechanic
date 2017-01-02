PACKAGE_VERSION := 0.3
BUILD_NUMBER := 2
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
	rm -rf target/ ${PWD}/src/cli/*.o ${PWD}/src/libmechanic/*.o

generate:
	cat ${PWD}/include/mechanic/metadata.h.in > ${PWD}/include/mechanic/metadata.h \
	&& sed -ri 's,^(.*VERSION\s*\")([^"]*)(\".*)$$,\1$(PACKAGE_VERSION)-$(BUILD_NUMBER)\3,g' ${PWD}/include/mechanic/metadata.h \
	&& sed -ri 's,^(.*SCM_VERSION\s*\")([^"]*)(\".*)$$,\1$(SCM_VERSION)\3,g' ${PWD}/include/mechanic/metadata.h
	
compile:	compile_lib compile_cli

compile_lib:
	@mkdir -p ${PWD}/target/ && \
	cd src/libmechanic && \
	echo "Compiling Lib..." && \
	gcc -fPIC -Wall -g -I ../../include/ -c *.c && \
	echo "Linking Lib..." && \
	gcc -shared $$(find . -name "*.o" | grep -v _test.o) -lm -lsqlite3 -o ../../target/libmechanic.so.${PACKAGE_VERSION}.${BUILD_NUMBER} && \
	ln -s libmechanic.so.${PACKAGE_VERSION}.${BUILD_NUMBER} ../../target/libmechanic.so.${PACKAGE_VERSION} && \
	ln -s libmechanic.so.${PACKAGE_VERSION} ../../target/libmechanic.so && \
	for i in *_test.c; do \
		echo "Compiling and linking Lib-Test $$(basename $$i .c)..." && \
		gcc $$(basename $$i .c).o -L ../../target/ -lmechanic -o ../../target/$$(basename $$i .c); \
	done

compile_cli:
	@mkdir -p ${PWD}/target/ && \
	cd src/cli && \
	echo "Compiling CLI..." && \
	gcc -Wall -g -I ../../include/ -c *.c && \
	echo "Linking CLI..." && \
	gcc $$(find . -name "*.o" | grep -v _test.o) -L ../../target/ -lmechanic -lm -lsqlite3 -o ../../target/mechanic 
	#for i in *_test.c; do \
	#	echo "Compiling and linking CLI-Test $$(basename $$i .c)..." && \
	#	gcc $$(basename $$i .c).o -L ../../target/ -lmechanic -lm -lsqlite3 -o ../../target/$$(basename $$i .c); \
	#done

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


