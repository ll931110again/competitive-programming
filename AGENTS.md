# Agent notes

Contest C++ solutions in this repo follow **`.cursor/rules/cpp-style.mdc`**. Read it before writing or editing `.cc` / `.cpp` files.

Highlights:

- Self-contained solutions with a file-header **sketch** (algorithm + complexity).
- Professional SWE structure: anonymous namespace, `PascalCase` types, `snake_case` names, `int64_t`.
- Parameters on one line when they fit; if wrapping, **one parameter per line** (no bin-packing).
- Do not include `lib/` from contest submissions; inline what you need.
- Format with `.clang-format` (LLVM, 2-space, 100 columns).
