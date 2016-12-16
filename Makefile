PACKAGE_VERSION := 0.1
BUILD_NUMBER := 2
SCM_VERSION = $(shell git rev-parse HEAD)

default:	clean build test

all:	clean build test packages

.PHONY:	clean
clean:
	@echo "Cleaning up..."; \
	rm -rf target/

build:
	mkdir -p ${PWD}/target/
	CGO_ENABLED=1 GOPATH=${PWD} go get -compiler gccgo -gccgoflags '-static-libgo -lpthread -pthread -lsqlite3' github.com/mattn/go-sqlite3/ github.com/cbuschka/config4go
	sed -ri 's,^(.*PackageVersion:\s*\")([^"]*)(\".*)$$,\1$(PACKAGE_VERSION)\3,g' ${PWD}/src/mechanic/metadata.go
	sed -ri 's,^(.*ScmVersion:\s*\")([^"]*)(\".*)$$,\1$(SCM_VERSION)\3,g' ${PWD}/src/mechanic/metadata.go
	CGO_ENABLED=1 GOPATH=${PWD} go build -compiler gccgo -gccgoflags '-static-libgo -lpthread -pthread -lsqlite3' -o target/mechanic src/mechanic.go

.PHONY:	integration-tests
test:
	@echo "Running integration tests..."; \
	${PWD}/scripts/run-integration-tests.sh

packages:	ubuntu-xenial ubuntu-yakkety fedora-25 centos-7

ubuntu-xenial:
	${PWD}/scripts/build-package.sh ubuntu-xenial $(PACKAGE_VERSION)-0ubuntu-xenial${BUILD_NUMBER}

ubuntu-yakkety:
	${PWD}/scripts/build-package.sh ubuntu-yakkety $(PACKAGE_VERSION)-0ubuntu-yakkety${BUILD_NUMBER}

centos-7:
	${PWD}/scripts/build-package.sh centos-7 $(PACKAGE_VERSION).centos7.${BUILD_NUMBER}

fedora-25:
	${PWD}/scripts/build-package.sh fedora-25 $(PACKAGE_VERSION).fedora25.${BUILD_NUMBER}


