# Library tests (C++ Google Test + Rust kinetic structures) via Buck2.
#
#   make test          bootstrap buck2, fetch deps, run all tests
#   make build-tests   build test binaries only
#   make clean-tests   remove Buck2 output + fetched googletest

BUCK2 ?= ./tools/buck2
BUCK2_VERSION ?= latest
JOBS ?= $(shell sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)

.PHONY: test build-tests clean-tests bootstrap-buck2 fetch-googletest gen-kotlin-buck

bootstrap-buck2:
	@bash scripts/bootstrap_buck2.sh

fetch-googletest:
	@bash scripts/fetch_googletest.sh

gen-kotlin-buck:
	python3 scripts/gen_kotlin_buck.py

build-tests: bootstrap-buck2 fetch-googletest
	$(BUCK2) build //tests/... //kinetic_data_structures/... -j $(JOBS)

test: bootstrap-buck2 fetch-googletest
	$(BUCK2) test //tests/... //kinetic_data_structures/... -j $(JOBS)

clean-tests:
	rm -rf buck-out deps/googletest
