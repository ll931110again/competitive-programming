#!/usr/bin/env python3
"""Replace long long / ll aliases with i64 across competitive-programming C++ sources."""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

SKIP_DIRS = {"third_party", "vendor", ".git"}
SKIP_FILES = {"stdc++.h"}

CPP_SUFFIXES = {".cc", ".cpp", ".hh", ".h"}

LL_ALIAS_RE = re.compile(
    r"^\s*(?:using\s+ll\s*=\s*long\s+long|typedef\s+long\s+long\s+ll)\s*;\s*$",
    re.MULTILINE,
)
INT64_ALIAS_RE = re.compile(
    r"^\s*(?:using\s+int64\s*=\s*long\s+long|typedef\s+long\s+long\s+int64)\s*;\s*$",
    re.MULTILINE,
)
I64_ALIAS_RE = re.compile(r"^\s*using\s+i64\s*=\s*long\s+long\s*;\s*$", re.MULTILINE)
USING_STD_RE = re.compile(r"^using\s+namespace\s+std\s*;\s*$", re.MULTILINE)
UNSIGNED_LL = "__UNSIGNED_LONG_LONG__"


def should_skip(path: Path) -> bool:
    if path.name in SKIP_FILES:
        return True
    if any(part in SKIP_DIRS for part in path.parts):
        return True
    return False


def normalize_aliases(text: str) -> tuple[str, bool, bool]:
    had_ll = bool(LL_ALIAS_RE.search(text))
    had_int64 = bool(INT64_ALIAS_RE.search(text))
    text = LL_ALIAS_RE.sub("using i64 = long long;", text)
    text = INT64_ALIAS_RE.sub("using i64 = long long;", text)
    return text, had_ll, had_int64


def replace_ll_usages(text: str, had_ll: bool, had_int64: bool) -> str:
    if had_ll:
        text = re.sub(r"\bll\b", "i64", text)
    if had_int64:
        text = re.sub(r"\bint64\b", "i64", text)
    return text


def replace_long_long(text: str) -> str:
    text = text.replace("unsigned long long", UNSIGNED_LL)
    lines = text.splitlines(keepends=True)
    out: list[str] = []
    for line in lines:
        if I64_ALIAS_RE.match(line.rstrip("\n")):
            out.append(line)
        else:
            out.append(line.replace("long long", "i64"))
    text = "".join(out)
    return text.replace(UNSIGNED_LL, "unsigned long long")


def insert_i64_alias(text: str) -> str:
    if I64_ALIAS_RE.search(text):
        return text

    match = USING_STD_RE.search(text)
    if match:
        insert_at = match.end()
        return text[:insert_at] + "\n\nusing i64 = long long;" + text[insert_at:]

    lines = text.splitlines(keepends=True)
    last_include = -1
    for i, line in enumerate(lines):
        if line.lstrip().startswith("#include"):
            last_include = i
    if last_include >= 0:
        insert_at = sum(len(lines[i]) for i in range(last_include + 1))
        return text[:insert_at] + "\nusing i64 = long long;\n" + text[insert_at:]

    return "using i64 = long long;\n\n" + text


def transform(text: str) -> tuple[str, bool]:
    if not re.search(r"\blong long\b|\busing ll\b|\btypedef\s+long long\s+ll\b|\busing int64\b|\btypedef\s+long long\s+int64\b", text):
        return text, False

    original = text
    text, had_ll, had_int64 = normalize_aliases(text)
    text = replace_ll_usages(text, had_ll, had_int64)
    text = replace_long_long(text)

    if re.search(r"\bi64\b", text):
        text = insert_i64_alias(text)

    return text, text != original


def iter_cpp_files() -> list[Path]:
    files: list[Path] = []
    for path in ROOT.rglob("*"):
        if not path.is_file() or path.suffix not in CPP_SUFFIXES:
            continue
        if should_skip(path):
            continue
        files.append(path)
    return sorted(files)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("paths", nargs="*", help="Optional files or directories (default: repo)")
    parser.add_argument("--check", action="store_true", help="Exit 1 if any file would change")
    args = parser.parse_args()

    if args.paths:
        targets: list[Path] = []
        for raw in args.paths:
            path = Path(raw)
            if not path.is_absolute():
                path = ROOT / path
            if path.is_dir():
                targets.extend(
                    p
                    for p in path.rglob("*")
                    if p.is_file() and p.suffix in CPP_SUFFIXES and not should_skip(p)
                )
            elif path.is_file():
                targets.append(path)
    else:
        targets = iter_cpp_files()

    changed: list[Path] = []
    for path in sorted(set(targets)):
        text = path.read_text(encoding="utf-8")
        new_text, did_change = transform(text)
        if did_change:
            changed.append(path)
            if not args.check:
                path.write_text(new_text, encoding="utf-8")

    if args.check and changed:
        for path in changed:
            rel = path.relative_to(ROOT)
            print(f"would change: {rel}", file=sys.stderr)
        print(f"normalize_i64: {len(changed)} files need updates", file=sys.stderr)
        return 1

    print(f"normalize_i64: {len(changed)} files updated", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
