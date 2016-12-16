PACKAGE_VERSION := 0.1
SCM_VERSION = $(shell git rev-parse HEAD)
 
all:	clean build integration-tests

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
integration-tests:
	@echo "Running integration tests..."; \
	${PWD}/scripts/run-integration-tests.sh

packages:	ubuntu-xenial centos-7

ubuntu-xenial:
	${PWD}/scripts/build-package.sh ubuntu-xenial $(PACKAGE_VERSION)-0ubuntu1

centos-7:
	${PWD}/scripts/build-package.sh centos-7 $(PACKAGE_VERSION)

