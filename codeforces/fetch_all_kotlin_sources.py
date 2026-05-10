#!/usr/bin/env python3
"""Fetch AC submission sources for all Codeforces ``Kotlin``-named contests.

Mirrors ``kotlin/<folder>/`` under the repo root. Covers Kotlin Heroes episodes
1–14 (including ICPC round 5), their practice rounds, and the Ecnerwala vs
Errichto Kotlin match. Uses ``fetch_user_contest_sources_api.py`` and
``codeforces/.cf_api.env``.

Example::

    python3 codeforces/fetch_all_kotlin_sources.py --handle ll931110

Before each contest, existing ``*.kt``, ``*.cc``, ``*.cpp``, and ``*.txt`` in
that output directory are removed so naming stays consistent with the API.
"""

from __future__ import annotations

import argparse
import glob
import os
import subprocess
import sys

# (directory under kotlin/, contest_id). Directory names match kotlin heroes
# episodes and parallel practice rounds (see Codeforces contest titles).
KOTLIN_CONTESTS: list[tuple[str, int]] = [
    ("kotlin1", 1170),
    ("practice1", 1171),
    ("kotlin2", 1211),
    ("practice2", 1212),
    ("kotlin3", 1297),
    ("practice3", 1298),
    ("kotlin4", 1346),
    ("practice4", 1347),
    ("kotlin5", 1431),
    ("practice5", 1432),
    ("kotlin6", 1488),
    ("practice6", 1489),
    ("practice7", 1532),
    ("kotlin7", 1533),
    ("practice8", 1570),
    ("kotlin8", 1571),
    ("kotlin_match", 1755),
    ("kotlin9", 1910),
    ("practice9", 1911),
    ("kotlin10", 1958),
    ("practice10", 1959),
    ("kotlin11", 2011),
    ("practice11", 2012),
    ("kotlin12", 2087),
    ("practice12", 2088),
    ("kotlin13", 2141),
    ("practice13", 2142),
    ("practice14", 2198),
    ("kotlin14", 2199),
]


def clear_sources(out_dir: str) -> None:
    for pattern in ("*.kt", "*.cc", "*.cpp", "*.txt"):
        for p in glob.glob(os.path.join(out_dir, pattern)):
            os.remove(p)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--handle", default="ll931110")
    ap.add_argument(
        "--root",
        default="kotlin",
        help="Parent directory for kotlin1, practice1, …",
    )
    ap.add_argument(
        "--api-env",
        default="codeforces/.cf_api.env",
        help="Passed through to fetch_user_contest_sources_api.py",
    )
    ap.add_argument(
        "--no-clean",
        action="store_true",
        help="Do not delete existing source files in each folder before fetch",
    )
    args = ap.parse_args()

    here = os.path.dirname(os.path.abspath(__file__))
    fetcher = os.path.join(here, "fetch_user_contest_sources_api.py")
    repo_root = os.path.dirname(here)

    for folder, cid in KOTLIN_CONTESTS:
        out = os.path.join(repo_root, args.root, folder)
        os.makedirs(out, exist_ok=True)
        if not args.no_clean:
            clear_sources(out)
        print(f"=== Kotlin contest {cid} -> {out} ===", flush=True)
        rc = subprocess.call(
            [
                sys.executable,
                fetcher,
                "--handle",
                args.handle,
                "--contest-id",
                str(cid),
                "--out-dir",
                out,
                "--api-env",
                os.path.join(repo_root, args.api_env),
            ],
            cwd=repo_root,
        )
        if rc != 0:
            return rc

    print("Done.", flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
