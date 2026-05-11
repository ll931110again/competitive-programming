#!/usr/bin/env python3
"""Fetch AC submission sources for CodeTON rounds 1–9 via the signed API.

Writes under ``codeton/codeton{N}/`` (creates directories). Uses the same
credentials as ``fetch_user_contest_sources_api.py`` (``codeforces/.cf_api.env``).

Example::

    python3 codeforces/fetch_all_codeton_sources.py --handle ll931110

Empty folders mean there were no submissions from that handle for that contest
(e.g. virtual/absent round).
"""

from __future__ import annotations

import argparse
import os
import subprocess
import sys

# Round number -> Codeforces contest id (regular rated CodeTON div1+2 rounds).
CODETON_CONTEST_IDS: dict[int, int] = {
    1: 1656,
    2: 1704,
    3: 1750,
    4: 1810,
    5: 1842,
    6: 1870,
    7: 1896,
    8: 1942,
    9: 2039,
}


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--handle", default="ll931110")
    ap.add_argument(
        "--root",
        default="codeton",
        help="Parent directory for codeton1 … codeton9",
    )
    ap.add_argument(
        "--api-env",
        default="codeforces/.cf_api.env",
        help="Passed through to fetch_user_contest_sources_api.py",
    )
    args = ap.parse_args()

    here = os.path.dirname(os.path.abspath(__file__))
    fetcher = os.path.join(here, "fetch_user_contest_sources_api.py")
    repo_root = os.path.dirname(here)

    for rnd, cid in sorted(CODETON_CONTEST_IDS.items()):
        out = os.path.join(repo_root, args.root, f"codeton{rnd}")
        os.makedirs(out, exist_ok=True)
        print(f"=== CodeTON Round {rnd} (contest {cid}) -> {out} ===", flush=True)
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
