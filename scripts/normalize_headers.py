#!/usr/bin/env python3
"""Normalize solution includes to bits/stdc++.h + using namespace std."""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

SKIP_DIRS = {"third_party", "vendor", ".git"}

KEEP_ANGLE = {"race.h", "grader.h", "grader.cpp"}

STDLIB_HEADERS = {
    "algorithm",
    "array",
    "bitset",
    "cassert",
    "cctype",
    "cerrno",
    "cfenv",
    "cfloat",
    "chrono",
    "climits",
    "cmath",
    "complex",
    "csetjmp",
    "cstdarg",
    "cstddef",
    "cstdint",
    "cstdio",
    "cstdlib",
    "cstring",
    "ctime",
    "cwchar",
    "cwctype",
    "deque",
    "exception",
    "fstream",
    "functional",
    "iomanip",
    "ios",
    "iosfwd",
    "iostream",
    "istream",
    "iterator",
    "limits",
    "list",
    "locale",
    "map",
    "memory",
    "new",
    "numeric",
    "ostream",
    "queue",
    "random",
    "set",
    "sstream",
    "stack",
    "stdexcept",
    "streambuf",
    "string",
    "tuple",
    "typeinfo",
    "type_traits",
    "unordered_map",
    "unordered_set",
    "utility",
    "valarray",
    "vector",
    "clocale",
    "forward_list",
    "regex",
    "mutex",
    "thread",
    "future",
    "condition_variable",
    "optional",
    "variant",
    "any",
    "charconv",
    "filesystem",
    "scoped_allocator",
    "shared_mutex",
}

INCLUDE_ANGLE_RE = re.compile(r"^#include\s+<([^>]+)>\s*$")
INCLUDE_QUOTED_RE = re.compile(r'^#include\s+"([^"]+)"\s*$')
USING_STD_RE = re.compile(r"^using\s+namespace\s+std\s*;\s*$")


def should_skip(path: Path) -> bool:
    rel = path.relative_to(ROOT).as_posix()
    if rel == "bits/stdc++.h":
        return True
    if any(part in SKIP_DIRS for part in path.parts):
        return True
    if rel.startswith("lib/") and path.suffix in {".hh", ".h"}:
        return True
    if path.name in KEEP_ANGLE:
        return True
    return False


def keep_angle(name: str) -> bool:
    if name in KEEP_ANGLE:
        return True
    if name in STDLIB_HEADERS:
        return False
    if name.endswith(".hh") or (name.endswith(".h") and "/" not in name):
        return True
    return False


def is_code_line(line: str) -> bool:
    s = line.strip()
    if not s:
        return False
    if s.startswith("//"):
        return False
    if s.startswith("/*") or s.startswith("*") or s.endswith("*/"):
        return False
    if s.startswith("#"):
        return False
    return True


def normalize_text(text: str) -> str:
    text = text.replace("\r\n", "\n")
    if text.startswith("#ifdef ONLINE_JUDGE\n#include <bits/stdc++.h>\n#endif\n"):
        text = text[len("#ifdef ONLINE_JUDGE\n#include <bits/stdc++.h>\n#endif\n") :]

    lines = text.split("\n")
    i = 0
    comments: list[str] = []

    while i < len(lines):
        s = lines[i].strip()
        if not s or s.startswith("//"):
            comments.append(lines[i])
            i += 1
            continue
        if s.startswith("/*"):
            while i < len(lines):
                comments.append(lines[i])
                if "*/" in lines[i]:
                    i += 1
                    break
                i += 1
            continue
        break

    quoted: list[str] = []
    angle_keep: list[str] = []
    prelude: list[str] = []

    while i < len(lines):
        s = lines[i].strip()
        if not s:
            i += 1
            continue
        m = INCLUDE_QUOTED_RE.match(s)
        if m:
            quoted.append(lines[i])
            i += 1
            continue
        m = INCLUDE_ANGLE_RE.match(s)
        if m:
            name = m.group(1)
            if name == "bits/stdc++.h":
                i += 1
                continue
            if keep_angle(name):
                angle_keep.append(lines[i])
            i += 1
            continue
        if USING_STD_RE.match(s):
            i += 1
            continue
        if s.startswith("#if") or s.startswith("#endif") or s.startswith("#else"):
            i += 1
            continue
        if is_code_line(lines[i]):
            break
        prelude.append(lines[i])
        i += 1

    body = lines[i:]
    while body and not body[0].strip():
        body.pop(0)

    out: list[str] = []
    if comments:
        out.extend(comments)
        if comments[-1].strip():
            out.append("")
    out.extend(quoted)
    out.extend(angle_keep)
    out.append("#include <bits/stdc++.h>")
    out.append("using namespace std;")
    if prelude:
        out.append("")
        out.extend(prelude)
    if body:
        if out and out[-1].strip():
            out.append("")
        out.extend(body)

    result = "\n".join(out)
    if not result.endswith("\n"):
        result += "\n"
    return result


def normalize_file(path: Path, dry_run: bool) -> bool:
    original = path.read_text(encoding="utf-8", errors="replace")
    updated = normalize_text(original)
    if updated != original and not original.endswith("\n") and updated == normalize_text(original + "\n"):
        pass
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
            if normalize_file(path, args.dry_run):
                changed += 1
    print(f"{'would change' if args.dry_run else 'changed'} {changed} files", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
