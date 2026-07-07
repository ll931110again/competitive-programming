#!/usr/bin/env python3
"""Download official AtCoder Heuristic Contest local tools for one or more contests."""

from __future__ import annotations

import argparse
import re
import shutil
import subprocess
import sys
import time
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ZIP_RE = re.compile(
    r"https://img\.atcoder\.jp/(ahc\d{3})/[A-Za-z0-9_]+\.zip"
)


def contest_id(num: int) -> str:
    return f"ahc{num:03d}"


def fetch_tools_zip_urls(contest: str) -> list[str]:
    task_url = f"https://atcoder.jp/contests/{contest}/tasks/{contest}_a"
    req = urllib.request.Request(
        task_url,
        headers={"User-Agent": "competitive-programming/1.0"},
    )
    try:
        with urllib.request.urlopen(req, timeout=60) as resp:
            html = resp.read().decode("utf-8", errors="replace")
    except Exception as exc:  # noqa: BLE001
        print(f"[fail] {contest}: cannot fetch task page ({exc})", file=sys.stderr)
        return None
    urls = [u for u in ZIP_RE.findall(html) if "windows" not in u]
    # findall returns contest ids; re-find full URLs
    full = re.findall(
        r"https://img\.atcoder\.jp/" + contest + r"/[A-Za-z0-9_]+\.zip", html
    )
    full = [u for u in full if "windows" not in u.lower()]
    return full


def download_file(url: str, dest: Path, retries: int = 3) -> None:
    last_err: Exception | None = None
    for attempt in range(retries):
        try:
            req = urllib.request.Request(url, headers={"User-Agent": "competitive-programming/1.0"})
            with urllib.request.urlopen(req, timeout=120) as resp:
                data = resp.read()
            if len(data) < 1024:
                raise OSError(f"response too small ({len(data)} bytes)")
            dest.write_bytes(data)
            return
        except Exception as exc:  # noqa: BLE001
            last_err = exc
            if attempt + 1 < retries:
                time.sleep(1.5 * (attempt + 1))
    raise last_err  # type: ignore[misc]


def download_and_extract(contest: str, dest: Path, force: bool) -> bool:
    tools_dir = dest / "tools"
    if tools_dir.exists() and (tools_dir / "Cargo.toml").exists() and not force:
        print(f"[skip] {contest}: tools already present at {tools_dir}")
        return True

    urls = fetch_tools_zip_urls(contest)
    if not urls:
        print(f"[fail] {contest}: no tools zip URL found", file=sys.stderr)
        return False

    dest.mkdir(parents=True, exist_ok=True)
    zip_path = dest / "tools.zip"
    unpack = dest / "_unpack"
    best: Path | None = None
    best_score = -1

    for url in urls:
        if "seeds.zip" in url:
            continue
        print(f"[fetch] {contest}: {url}")
        try:
            download_file(url, zip_path)
            if unpack.exists():
                shutil.rmtree(unpack)
            subprocess.run(
                ["unzip", "-o", "-q", str(zip_path), "-d", str(unpack)],
                check=True,
            )
        except (OSError, subprocess.CalledProcessError) as exc:
            print(f"[warn] {contest}: {url} failed ({exc})", file=sys.stderr)
            zip_path.unlink(missing_ok=True)
            shutil.rmtree(unpack, ignore_errors=True)
            continue
        finally:
            zip_path.unlink(missing_ok=True)

        inner = unpack / "tools"
        candidate = inner if inner.is_dir() else unpack
        score = 0
        if (candidate / "Cargo.toml").exists():
            score += 10
        if (candidate / "src").is_dir():
            score += 5
        if (candidate / "in").is_dir():
            score += 1
        if score > best_score:
            best_score = score
            staging = dest / "_best"
            if staging.exists():
                shutil.rmtree(staging)
            shutil.copytree(candidate, staging)
        shutil.rmtree(unpack, ignore_errors=True)

    if best_score < 0:
        print(f"[fail] {contest}: no usable zip among {len(urls)} candidates", file=sys.stderr)
        return False

    staging = dest / "_best"
    if tools_dir.exists():
        shutil.rmtree(tools_dir)
    staging.rename(tools_dir)
    print(f"[ok] {contest}: extracted to {tools_dir}")
    return True


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "contests",
        nargs="+",
        help="Contest numbers (e.g. 1 65 67) or ranges like 1-67",
    )
    parser.add_argument(
        "--force",
        action="store_true",
        help="Re-download even if tools/ is non-empty",
    )
    args = parser.parse_args()

    nums: list[int] = []
    for item in args.contests:
        s = str(item)
        if "-" in s:
            lo, hi = map(int, s.split("-", 1))
            nums.extend(range(lo, hi + 1))
        else:
            nums.append(int(s))

    ok = True
    for num in nums:
        contest = contest_id(num)
        dest = ROOT / "atcoder" / contest
        if not download_and_extract(contest, dest, args.force):
            ok = False
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
