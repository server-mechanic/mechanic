PACKAGE_VERSION := 0.7
BUILD_NUMBER := 4
SCM_VERSION = $(shell git rev-parse HEAD)
PROJECT_DIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))

default:	bundle

bundle:	clean build tests integration-tests

all:	patch default packages

init:
	@echo "Project dir is $(PROJECT_DIR)."

install-prerequesites:	init
	@pip install -r requirements.txt -r requirements-test.txt -r requirements-dev.txt

.PHONY:	clean
clean:	init
	@echo "Cleaning up..."; \
	rm -rf target/ $$(find . -name "*.pyc" -or -name "*.pyo" -or -name "__pycache__")

patch:	init
	echo "Adding metadata to code..." && \
	perl -i -pe 's#(packageVersion\s*\=\s*)\"[^\"]+\"(.*$$)#\1\"$(PACKAGE_VERSION)-$(BUILD_NUMBER)\"\2#g' $(PROJECT_DIR)/src/python/mechanic/version_info.py && \
	perl -i -pe 's#(scmVersion\s*\=\s*)\"[^\"]+\"(.*$$)#\1\"$(SCM_VERSION)\"\2#g' $(PROJECT_DIR)/src/python/mechanic/version_info.py

build:	init
	@echo "Compiling python files..." && \
	python -m compileall $(PROJECT_DIR)/src/python/mechanic/ && \
	mkdir -p $(PROJECT_DIR)/target/ && \
	echo "Copying..." && \
	cp -R $(PROJECT_DIR)/src/bundle $(PROJECT_DIR)/target/bundle && \
	mkdir -p $(PROJECT_DIR)/target/bundle/usr/bin && \
	cp -R $(PROJECT_DIR)/src/bash/* $(PROJECT_DIR)/target/bundle/ && \
	mkdir -p $(PROJECT_DIR)/target/bundle/usr/lib/python2.7/site-packages/ && \
	mkdir -p $(PROJECT_DIR)/target/bundle/etc/mechanic/pre-migration.d && \
	mkdir -p $(PROJECT_DIR)/target/bundle/etc/mechanic/migration.d && \
	mkdir -p $(PROJECT_DIR)/target/bundle/etc/mechanic/post-migration.d && \
	mkdir -p $(PROJECT_DIR)/target/bundle/var/lib/mechanic/pre-migration.d && \
	mkdir -p $(PROJECT_DIR)/target/bundle/var/lib/mechanic/migration.d && \
	mkdir -p $(PROJECT_DIR)/target/bundle/var/lib/mechanic/post-migration.d && \
	mkdir -p $(PROJECT_DIR)/target/bundle/var/lib/mechanic/state && \
	mkdir -p $(PROJECT_DIR)/target/bundle/var/lib/mechanic/tmp && \
	cp -R $(PROJECT_DIR)/src/python/* $(PROJECT_DIR)/target/bundle/usr/lib/python2.7/site-packages/ && \
	cd $(PROJECT_DIR)/target/bundle && tar czf ../bundle.tgz --owner=0 --group=0 --exclude='*__pycache__*' --exclude='*.pyc' --exclude='*.pyo' .

tests:	init
	@echo "Running tests..."; \
	PYTHONPATH=$(PROJECT_DIR)/src/python/ coverage run -m unittest discover -s $(PROJECT_DIR)/src/python/ -v -p '*_test.py'

coverage: tests
	coverage html

integration-tests-for-all-python-versions:	init
	@PY_VENV_VERSION=2.7 $(PROJECT_DIR)/scripts/run-integration-tests.sh && \
	PY_VENV_VERSION=3.6 $(PROJECT_DIR)/scripts/run-integration-tests.sh

.PHONY:	integration-tests
integration-tests:	init
	@echo "Running integration tests..."; \
	$(PROJECT_DIR)/scripts/run-integration-tests.sh

packages:	bash-installer debian-wheezy debian-jessie debian-stretch debian-sid ubuntu-xenial ubuntu-yakkety ubuntu-zesty ubuntu-artful ubuntu-bionic fedora-26 centos-7 fedora-27 fedora-28

bash-installer:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh bash-installer $(PACKAGE_VERSION)_${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

debian-wheezy:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh debian-wheezy $(PACKAGE_VERSION)-0wheezy${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

debian-sid:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh debian-sid $(PACKAGE_VERSION)-0sid${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

debian-jessie:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh debian-jessie $(PACKAGE_VERSION)-0jessie${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

debian-stretch:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh debian-stretch $(PACKAGE_VERSION)-0stretch${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-xenial:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh ubuntu-xenial $(PACKAGE_VERSION)-0ubuntu-xenial${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-yakkety:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh ubuntu-yakkety $(PACKAGE_VERSION)-0ubuntu-yakkety${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-zesty:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh ubuntu-zesty $(PACKAGE_VERSION)-0ubuntu-zesty${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-artful:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh ubuntu-artful $(PACKAGE_VERSION)-0ubuntu-artful${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

ubuntu-bionic:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh ubuntu-bionic $(PACKAGE_VERSION)-0ubuntu-bionic${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

centos-7:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh centos-7 $(PACKAGE_VERSION).centos7-${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

fedora-26:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh fedora-26 $(PACKAGE_VERSION).fedora26-${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

fedora-27:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh fedora-27 $(PACKAGE_VERSION).fedora27-${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

fedora-28:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh fedora-28 $(PACKAGE_VERSION).fedora28-${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}

fedora-29:	bundle
	$(PROJECT_DIR)/scripts/build-package.sh fedora-29 $(PACKAGE_VERSION).fedora29-${BUILD_NUMBER} $(PACKAGE_VERSION) ${BUILD_NUMBER}
