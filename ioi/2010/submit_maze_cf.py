#!/usr/bin/env python3
"""
Submit IOI 2010 Maze to Codeforces IOI archive.

Packages maze_outputs/ into maze_outputs.zip (01.out … 10.out) and uploads via
the contest submit form on ioi.contest.codeforces.com.

Requires a logged-in session cookie file:
  codeforces/.cf_cookie   (export from browser; see codeforces/.cf_cookie.example)

Usage:
  python3 pack_maze_submission.py
  python3 submit_maze_cf.py --contest-id 103757 --problem G
"""

from __future__ import annotations

import argparse
import http.cookiejar
import re
import ssl
import subprocess
import sys
import urllib.parse
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent
REPO = ROOT.parents[1]
DEFAULT_COOKIE = REPO / "codeforces" / ".cf_cookie"
DEFAULT_ZIP = ROOT / "maze_outputs.zip"
GROUP = "32KGsXgiKA"
SUBMIT_BASE = "https://ioi.contest.codeforces.com"


def load_cookies(path: Path) -> http.cookiejar.MozillaCookieJar:
    jar = http.cookiejar.MozillaCookieJar()
    if not path.is_file():
        raise SystemExit(
            f"Missing cookie file: {path}\n"
            "Log in to ioi.contest.codeforces.com, export cookies (Netscape format), "
            "and save as codeforces/.cf_cookie"
        )
    jar.load(str(path), ignore_discard=True, ignore_expires=True)
    return jar


def fetch_csrf(opener: urllib.request.OpenerDirector, contest_id: int) -> str:
    url = f"{SUBMIT_BASE}/group/{GROUP}/contest/{contest_id}/submit"
    with opener.open(url, timeout=60) as resp:
        html = resp.read().decode("utf-8", errors="replace")
    m = re.search(r'csrf[_-]?token["\']?\s*(?:value=|:\s*)["\']([a-f0-9]+)', html, re.I)
    if not m:
        m = re.search(r'name="csrf_token" value="([^"]+)"', html)
    if not m:
        raise SystemExit("Could not find CSRF token on submit page (are you logged in?)")
    return m.group(1)


def submit_zip(
    opener: urllib.request.OpenerDirector,
    contest_id: int,
    problem_index: str,
    zip_path: Path,
    language_id: str = "91",
) -> str:
    csrf = fetch_csrf(opener, contest_id)
    boundary = "----mazeSubmitBoundary7MA4YWxk"

    zip_bytes = zip_path.read_bytes()
    parts: list[bytes] = []

    def add_field(name: str, value: str) -> None:
        parts.append(
            f"--{boundary}\r\n"
            f'Content-Disposition: form-data; name="{name}"\r\n\r\n'
            f"{value}\r\n".encode()
        )

    add_field("csrf_token", csrf)
    add_field("action", "submitSolutionForm")
    add_field("submittedProblemIndex", problem_index)
    add_field("programTypeId", language_id)
    add_field("source", "")
    add_field("tabSize", "4")
    add_field("denySubmit", "")
    add_field("_tta", "176")

    parts.append(
        (
            f"--{boundary}\r\n"
            f'Content-Disposition: form-data; name="sourceFile"; '
            f'filename="{zip_path.name}"\r\n'
            f"Content-Type: application/zip\r\n\r\n"
        ).encode()
        + zip_bytes
        + b"\r\n"
    )
    parts.append(f"--{boundary}--\r\n".encode())
    body = b"".join(parts)

    url = f"{SUBMIT_BASE}/group/{GROUP}/contest/{contest_id}/submit"
    req = urllib.request.Request(
        url,
        data=body,
        method="POST",
        headers={
            "Content-Type": f"multipart/form-data; boundary={boundary}",
            "User-Agent": "Mozilla/5.0",
            "Referer": url,
        },
    )
    with opener.open(req, timeout=120) as resp:
        html = resp.read().decode("utf-8", errors="replace")
    if "Submission accepted" in html or "submitted successfully" in html.lower():
        return "Submission accepted"
    if "error" in html.lower() and "submit" in html.lower():
        snippet = html[:2000]
        raise SystemExit(f"Submit may have failed. Page snippet:\n{snippet}")
    return "POST completed (check contest submissions page)"


def main() -> None:
    ap = argparse.ArgumentParser(description="Submit maze_outputs.zip to CF IOI archive")
    ap.add_argument("--contest-id", type=int, default=103757)
    ap.add_argument("--problem", default="G", help="problem index (Maze is typically G)")
    ap.add_argument("--zip", type=Path, default=DEFAULT_ZIP)
    ap.add_argument("--cookie", type=Path, default=DEFAULT_COOKIE)
    ap.add_argument("--pack", action="store_true", help="run pack_maze_submission.py first")
    args = ap.parse_args()

    if args.pack or not args.zip.is_file():
        subprocess.run(
            [sys.executable, str(ROOT / "pack_maze_submission.py")],
            check=True,
            cwd=str(ROOT),
        )

    ctx = ssl.create_default_context()
    jar = load_cookies(args.cookie)
    opener = urllib.request.build_opener(
        urllib.request.HTTPCookieProcessor(jar),
        urllib.request.HTTPSHandler(context=ctx),
    )

    msg = submit_zip(opener, args.contest_id, args.problem, args.zip)
    print(msg)
    print(
        f"https://ioi.contest.codeforces.com/group/{GROUP}/contest/{args.contest_id}/my"
    )


if __name__ == "__main__":
    main()
