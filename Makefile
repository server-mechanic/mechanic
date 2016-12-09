all:	clean build integration-tests

.PHONY:	clean
clean:
	@echo "Cleaning up..."; \
	rm -rf target/

build:
	mkdir -p ${PWD}/target/
	GOPATH=${PWD} go build -o target/mechanic src/mechanic.go

.PHONY:	integration-tests
integration-tests:
	@echo "Running integration tests..."; \
	${PWD}/scripts/run-integration-tests.sh
