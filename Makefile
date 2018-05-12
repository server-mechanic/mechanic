PACKAGE_VERSION := 0.7
BUILD_NUMBER := 2
SCM_VERSION = $(shell git rev-parse HEAD)

default:	bundle

bundle:	clean build tests integration-tests

all:	patch default packages

.PHONY:	clean
clean:
	@echo "Cleaning up..."; \
	rm -rf target/ $$(find . -name "*.pyc" -or -name "*.pyo")

patch:
	echo "Adding metadata to code..." && \
	perl -i -pe 's#(packageVersion\s*\=\s*)\"[^\"]+\"(.*$$)#\1\"$(PACKAGE_VERSION)-$(BUILD_NUMBER)\"\2#g' ${PWD}/src/python/mechanic/version_info.py && \
	perl -i -pe 's#(scmVersion\s*\=\s*)\"[^\"]+\"(.*$$)#\1\"$(SCM_VERSION)\"\2#g' ${PWD}/src/python/mechanic/version_info.py

build:
	@echo "Compiling python files..." && \
	python -m compileall ${PWD}/src/python/mechanic/ && \
	mkdir -p ${PWD}/target/ && \
	echo "Copying..." && \
	cp -R ${PWD}/src/bundle ${PWD}/target/bundle && \
	mkdir -p ${PWD}/target/bundle/usr/bin && \
	cp -R ${PWD}/src/bash/* ${PWD}/target/bundle/ && \
	mkdir -p ${PWD}/target/bundle/usr/lib/python2.7/site-packages/ && \
	mkdir -p ${PWD}/target/bundle/etc/mechanic/pre-migration.d && \
	mkdir -p ${PWD}/target/bundle/etc/mechanic/migration.d && \
	mkdir -p ${PWD}/target/bundle/etc/mechanic/post-migration.d && \
	mkdir -p ${PWD}/target/bundle/var/lib/mechanic/pre-migration.d && \
	mkdir -p ${PWD}/target/bundle/var/lib/mechanic/migration.d && \
	mkdir -p ${PWD}/target/bundle/var/lib/mechanic/post-migration.d && \
	mkdir -p ${PWD}/target/bundle/var/lib/mechanic/state && \
	mkdir -p ${PWD}/target/bundle/var/lib/mechanic/tmp && \
	cp -R ${PWD}/src/python/* ${PWD}/target/bundle/usr/lib/python2.7/site-packages/ && \
	cd ${PWD}/target/bundle && tar czf ../bundle.tgz --owner=0 --group=0 --exclude='*__pycache__*' --exclude='*.pyc' --exclude='*.pyo' .

tests:
	@echo "Running tests..."; \
	PYTHONPATH=${PWD}/src/python/ coverage run -m unittest discover -s ${PWD}/src/python/ -v -p '*_test.py'

coverage: tests
	coverage html

.PHONY:	integration-tests
integration-tests:
	@echo "Running integration tests..."; \
	${PWD}/scripts/run-integration-tests.sh

packages:	bash-installer debian-wheezy debian-jessie debian-stretch debian-sid ubuntu-xenial ubuntu-yakkety ubuntu-zesty ubuntu-artful ubuntu-bionic fedora-26 centos-7 fedora-27 fedora-28

bash-installer:	bundle
	${PWD}/scripts/build-package.sh bash-installer $(PACKAGE_VERSION)_${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

debian-wheezy:	bundle
	${PWD}/scripts/build-package.sh debian-wheezy $(PACKAGE_VERSION)-0wheezy${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

debian-sid:	bundle
	${PWD}/scripts/build-package.sh debian-sid $(PACKAGE_VERSION)-0sid${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

debian-jessie:	bundle
	${PWD}/scripts/build-package.sh debian-jessie $(PACKAGE_VERSION)-0jessie${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

debian-stretch:	bundle
	${PWD}/scripts/build-package.sh debian-stretch $(PACKAGE_VERSION)-0stretch${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-xenial:	bundle
	${PWD}/scripts/build-package.sh ubuntu-xenial $(PACKAGE_VERSION)-0ubuntu-xenial${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-yakkety:	bundle
	${PWD}/scripts/build-package.sh ubuntu-yakkety $(PACKAGE_VERSION)-0ubuntu-yakkety${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-zesty:	bundle
	${PWD}/scripts/build-package.sh ubuntu-zesty $(PACKAGE_VERSION)-0ubuntu-zesty${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-artful:	bundle
	${PWD}/scripts/build-package.sh ubuntu-artful $(PACKAGE_VERSION)-0ubuntu-artful${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-bionic:	bundle
	${PWD}/scripts/build-package.sh ubuntu-bionic $(PACKAGE_VERSION)-0ubuntu-bionic${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

centos-7:	bundle
	${PWD}/scripts/build-package.sh centos-7 $(PACKAGE_VERSION).centos7-${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

fedora-26:	bundle
	${PWD}/scripts/build-package.sh fedora-26 $(PACKAGE_VERSION).fedora26-${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

fedora-27:	bundle
	${PWD}/scripts/build-package.sh fedora-27 $(PACKAGE_VERSION).fedora27-${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

fedora-28:	bundle
	${PWD}/scripts/build-package.sh fedora-28 $(PACKAGE_VERSION).fedora28-${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

