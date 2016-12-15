all:	clean build integration-tests

.PHONY:	clean
clean:
	@echo "Cleaning up..."; \
	rm -rf target/

build:
	mkdir -p ${PWD}/target/
	CGO_ENABLED=1 GOPATH=${PWD} go get -compiler gccgo -gccgoflags '-static-libgo -lpthread -pthread -lsqlite3' github.com/mattn/go-sqlite3/
	CGO_ENABLED=1 GOPATH=${PWD} go build -compiler gccgo -gccgoflags '-static-libgo -lpthread -pthread -lsqlite3' -o target/mechanic src/mechanic.go

.PHONY:	integration-tests
integration-tests:
	@echo "Running integration tests..."; \
	${PWD}/scripts/run-integration-tests.sh
