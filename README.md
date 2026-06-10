# competitive-programming

Archive of competitive programming problems.

- [`lib/`](lib/) — reusable templates (e.g. [`dsu_rollback.hh`](lib/dsu_rollback.hh))
- [`codeforces/dsu_rollback/README.md`](codeforces/dsu_rollback/README.md) — rollback DSU catalog (CF 2222F, 763E, 603E, 1386C, EDU 3A, …)

## Library tests (Buck2)

C++ code under `lib/` and the Rust crate [`kinetic_data_structures/`](kinetic_data_structures/) share one [Buck2](https://buck2.build/) graph. The first `make test` bootstraps Buck2 and fetches googletest v1.15.2 into `deps/googletest/` (gitignored). Rust uses the system `rustc` from your PATH.

```bash
make test                              # C++ gtests + Rust kinetic tests
make build-tests                       # build only
./tools/buck2 test //tests/...         # C++ only
./tools/buck2 test //kinetic_data_structures/...   # Rust only
```

Add a new test target in [`tests/BUCK`](tests/BUCK):

```python
cxx_test(
    name = "my_test",
    srcs = ["my_test.cc"],
    include_directories = [".", ".."],
    deps = ["//deps:gtest", "//deps:gtest_main"],
)
```

Add Rust integration tests in [`kinetic_data_structures/BUCK`](kinetic_data_structures/BUCK) (`rust_test` + `deps = [":lib"]`). `cargo test` still works for local Rust-only iteration.
