#!/usr/bin/env python3
"""Apply SWE code style to standalone competitive-programming solutions."""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

SKIP_DIRS = {"third_party", "vendor", ".git", "lib"}
SKIP_PREFIXES = (
    "topcoder-open/",
    "tco-marathon/",
    "atcoder/ahc065/tools/",
)
SKIP_FILES = {"bits/stdc++.h"}

MAIN_RE = re.compile(r"^int\s+main\s*\(")
STRUCT_LOWER_RE = re.compile(r"^(\s*)struct\s+([a-z][a-z0-9_]*)\s*\{")
FUNC_DEF_RE = re.compile(
    r"^(\s*)(?:static\s+)?(?:inline\s+)?"
    r"(?:const\s+)?(?:unsigned\s+)?(?:long\s+long|long\s+double|bool|int|void|char|"
    r"double|float|auto|ll|ld|size_t|string)\s+"
    r"([a-z][a-zA-Z0-9_]*)\s*\([^;]*\)\s*(?:const\s*)?\{"
)
DEFINE_MAXN_RE = re.compile(r"^#define\s+maxn\s+(\d+)\s*$", re.MULTILINE)
DEFINE_MAXV_RE = re.compile(r"^#define\s+maxv\s+(\d+)\s*$", re.MULTILINE)


def should_skip(path: Path) -> bool:
    rel = path.relative_to(ROOT).as_posix()
    if rel in SKIP_FILES:
        return True
    if any(part in SKIP_DIRS for part in path.parts):
        return True
    return rel.startswith(SKIP_PREFIXES)


def camel_to_snake(name: str) -> str:
    if name == "main" or "_" in name or name.islower():
        if not any(c.isupper() for c in name):
            return name
    s1 = re.sub(r"(.)([A-Z][a-z]+)", r"\1_\2", name)
    return re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", s1).lower()


def split_header_body(text: str) -> tuple[str, str]:
    lines = text.splitlines(keepends=True)
    i = 0
    header: list[str] = []
    while i < len(lines):
        s = lines[i].strip()
        if not s or s.startswith("//"):
            header.append(lines[i])
            i += 1
            continue
        if s.startswith("/*"):
            while i < len(lines):
                header.append(lines[i])
                if "*/" in lines[i]:
                    i += 1
                    break
                i += 1
            continue
        break

    while i < len(lines):
        s = lines[i].strip()
        if not s:
            header.append(lines[i])
            i += 1
            continue
        if s.startswith("#include") or USING_STD_RE.match(s):
            header.append(lines[i])
            i += 1
            continue
        if s.startswith("#define") or s.startswith("#pragma"):
            header.append(lines[i])
            i += 1
            continue
        if s.startswith("using "):
            header.append(lines[i])
            i += 1
            continue
        break

    return "".join(header), "".join(lines[i:])


USING_STD_RE = re.compile(r"^using\s+namespace\s+std\s*;\s*$")


def convert_defines(text: str) -> str:
    text = DEFINE_MAXN_RE.sub(r"constexpr int kMaxN = \1;", text)
    text = DEFINE_MAXV_RE.sub(r"constexpr int kMaxV = \1;", text)
    text = re.sub(r"\bmaxn\b", "kMaxN", text)
    text = re.sub(r"\bmaxv\b", "kMaxV", text)
    return text


CAMEL_IDENT_RE = re.compile(r"\b([a-z][a-zA-Z0-9]*)\s*\(")
CAMEL_WORD_RE = re.compile(r"\b([a-z][a-zA-Z0-9]*)\b")
SKIP_CAMEL_IDS = frozenset(
    {
        "begin",
        "end",
        "first",
        "second",
        "emplace",
        "push_back",
        "pop_back",
        "reserve",
        "size",
        "empty",
        "clear",
        "insert",
        "erase",
        "find",
        "count",
        "lower_bound",
        "upper_bound",
        "make_pair",
        "make_tuple",
        "make_shared",
        "make_unique",
        "getline",
        "substr",
        "append",
        "assign",
    }
)


def collect_camel_names(text: str) -> set[str]:
    names: set[str] = set()
    for m in CAMEL_WORD_RE.finditer(text):
        name = m.group(1)
        if any(c.isupper() for c in name) and name not in SKIP_CAMEL_IDS:
            names.add(name)
    return names


def apply_snake_names(text: str, names: set[str]) -> str:
    for name in sorted(names, key=len, reverse=True):
        new = camel_to_snake(name)
        if new == name:
            continue
        text = re.sub(rf"\b{re.escape(name)}\b", new, text)
    return text


def rename_camel_functions(text: str) -> str:
    names: set[str] = set()
    for line in text.splitlines():
        m = FUNC_DEF_RE.match(line)
        if m and any(c.isupper() for c in m.group(2)):
            names.add(m.group(2))
    for m in CAMEL_IDENT_RE.finditer(text):
        name = m.group(1)
        if any(c.isupper() for c in name):
            names.add(name)
    return apply_snake_names(text, names)


def rename_camel_variables(text: str) -> str:
    return apply_snake_names(text, collect_camel_names(text))


def rename_lowercase_structs(text: str) -> str:
    structs: set[str] = set()
    for line in text.splitlines():
        m = STRUCT_LOWER_RE.match(line)
        if m:
            structs.add(m.group(2))
    for name in sorted(structs, key=len, reverse=True):
        pascal = name[0].upper() + name[1:]
        text = re.sub(rf"\bstruct\s+{re.escape(name)}\b", f"struct {pascal}", text)
        text = re.sub(rf"<\s*{re.escape(name)}\s*>", f"<{pascal}>", text)
        text = re.sub(rf"\b{re.escape(name)}\s*\(", f"{pascal}(", text)
        text = re.sub(rf"\({re.escape(name)}\s+", f"({pascal} ", text)
        text = re.sub(rf",\s*{re.escape(name)}\s+", f", {pascal} ", text)
    return text


def remove_static_in_block(block: str) -> str:
    return re.sub(r"^(\s*)static\s+", r"\1", block, flags=re.MULTILINE)


def ensure_fast_io(main_block: str) -> str:
    if "sync_with_stdio" in main_block:
        return main_block
    lines = main_block.splitlines(keepends=True)
    out: list[str] = []
    inserted = False
    for line in lines:
        out.append(line)
        if not inserted and re.match(r"^int\s+main\s*\(", line.strip()):
            out.append("  ios::sync_with_stdio(false);\n")
            out.append("  cin.tie(nullptr);\n")
            out.append("\n")
            inserted = True
    return "".join(out)


def modernize_stdio_in_main(main_block: str) -> str:
    if "scanf" not in main_block and "printf" not in main_block:
        return main_block
    main_block = re.sub(
        r'printf\s*\(\s*"%I64d\\n"\s*,\s*([^)]+)\)',
        r"cout << \1 << '\\n'",
        main_block,
    )
    main_block = re.sub(
        r'printf\s*\(\s*"%lld\\n"\s*,\s*([^)]+)\)',
        r"cout << \1 << '\\n'",
        main_block,
    )
    main_block = re.sub(
        r'scanf\s*\(\s*"%d %d"\s*,\s*&([^,]+)\s*,\s*&([^)]+)\)',
        r"cin >> \1 >> \2",
        main_block,
    )
    main_block = re.sub(
        r'scanf\s*\(\s*"%d"\s*,\s*&([^)]+)\)',
        r"cin >> \1",
        main_block,
    )
    return main_block


def find_main_index(lines: list[str]) -> int | None:
    for i, line in enumerate(lines):
        if MAIN_RE.match(line.strip()):
            return i
    return None


def wrap_anonymous_namespace(body: str) -> str:
    if "namespace {" in body or "namespace\n{" in body:
        return remove_static_in_block(body)

    lines = body.splitlines(keepends=True)
    main_idx = find_main_index(lines)
    if main_idx is None:
        if body.strip():
            return "namespace {\n\n" + remove_static_in_block(body) + "\n}  // namespace\n"
        return body

    before = "".join(lines[:main_idx])
    main_and_after = "".join(lines[main_idx:])
    before = remove_static_in_block(before)
    if not before.strip():
        return main_and_after

    main_and_after = ensure_fast_io(main_and_after)
    main_and_after = modernize_stdio_in_main(main_and_after)
    return "namespace {\n\n" + before + "\n}  // namespace\n\n" + main_and_after


def strip_std_prefix_after_using(text: str) -> str:
    if "using namespace std;" not in text:
        return text
    # Keep std:: inside lib includes and explicit nested namespaces.
    if "namespace delaunay2d" in text or '#include "../lib/' in text:
        return text
    text = re.sub(r"\bstd::", "", text)
    return text


def refactor_text(text: str) -> str:
    text = convert_defines(text)
    header, body = split_header_body(text)
    body = rename_lowercase_structs(body)
    body = rename_camel_functions(body)
    body = rename_camel_variables(body)
    body = wrap_anonymous_namespace(body)
    result = header + body
    result = strip_std_prefix_after_using(result)
    if not result.endswith("\n"):
        result += "\n"
    return result


def refactor_file(path: Path, dry_run: bool) -> bool:
    original = path.read_text(encoding="utf-8", errors="replace")
    updated = refactor_text(original)
    if updated != original:
        if not dry_run:
            path.write_text(updated, encoding="utf-8")
        return True
    return False


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    changed = 0
    for ext in ("*.cc", "*.cpp"):
        for path in sorted(ROOT.rglob(ext)):
            if should_skip(path):
                continue
            if refactor_file(path, args.dry_run):
                changed += 1
                print(path.relative_to(ROOT))
    print(f"{'would change' if args.dry_run else 'changed'} {changed} files", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
