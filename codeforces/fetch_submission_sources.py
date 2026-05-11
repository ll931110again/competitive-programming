#!/usr/bin/env python3
"""Download accepted submission sources from Codeforces contest pages.

The official JSON API does not expose source code. This script fetches the HTML
submission page and extracts <pre id="program-source-text">.

Cloudflare may block requests without a browser cookie. Copy the Cookie header
for https://codeforces.com from your logged-in browser (DevTools → Network →
any request → Request Headers → Cookie), then either:

  export CODEFORCES_COOKIE='cf_clearance=...; 39ce39...=...; ...'
  python3 codeforces/fetch_submission_sources.py codeton/codeton7/SOURCES.json

Or store the same string in a gitignored file (see --cookie-file).

Manifest format: see codeton/codeton7/SOURCES.json
"""

from __future__ import annotations

import argparse
import html
import json
import os
import re
import sys
import urllib.error
import urllib.request


def load_cookie() -> str | None:
    return os.environ.get("CODEFORCES_COOKIE", "").strip() or None


def fetch_html(url: str, cookie: str | None) -> str:
    req = urllib.request.Request(
        url,
        headers={
            "User-Agent": "Mozilla/5.0 (compatible; competitive-programming-repo)",
            **({"Cookie": cookie} if cookie else {}),
        },
    )
    with urllib.request.urlopen(req, timeout=60) as resp:
        return resp.read().decode("utf-8", errors="replace")


def extract_source(html_text: str) -> str | None:
    m = re.search(
        r'id="program-source-text"[^>]*>([\s\S]*?)</pre>',
        html_text,
        re.I,
    )
    if not m:
        return None
    return html.unescape(m.group(1)).replace("\r\n", "\n")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "manifest",
        help="Path to SOURCES.json (see codeton/codeton7/SOURCES.json)",
    )
    ap.add_argument(
        "--out-dir",
        default=None,
        help="Directory for .cc files (default: manifest directory)",
    )
    ap.add_argument(
        "--cookie-file",
        default=None,
        help="Read Cookie header from file (default: codeforces/.cf_cookie if it exists)",
    )
    args = ap.parse_args()

    cookie = load_cookie()
    if not cookie and args.cookie_file:
        with open(args.cookie_file, "r", encoding="utf-8") as f:
            cookie = f.read().strip()
    if not cookie and os.path.isfile("codeforces/.cf_cookie"):
        with open("codeforces/.cf_cookie", "r", encoding="utf-8") as f:
            cookie = f.read().strip()
    if not cookie:
        print(
            "No cookie: set CODEFORCES_COOKIE or create codeforces/.cf_cookie "
            "(full Cookie header string from your browser).",
            file=sys.stderr,
        )
        return 1

    with open(args.manifest, "r", encoding="utf-8") as f:
        data = json.load(f)

    contest_id = data["contestId"]
    out_dir = args.out_dir or os.path.dirname(os.path.abspath(args.manifest))

    for prob in data["problems"]:
        sid = prob["submissionId"]
        fn = prob["filename"]
        url = f"https://codeforces.com/contest/{contest_id}/submission/{sid}"
        try:
            page = fetch_html(url, cookie)
        except urllib.error.HTTPError as e:
            print(f"{fn}: HTTP {e.code} for {url}", file=sys.stderr)
            continue
        if "Please wait" in page or "being checked" in page:
            print(
                f"{fn}: got Cloudflare challenge page — cookie missing/expired/wrong.",
                file=sys.stderr,
            )
            continue
        src = extract_source(page)
        if not src:
            print(f"{fn}: could not find program-source-text in {url}", file=sys.stderr)
            continue
        header = (
            f"// Codeforces {contest_id} ({data.get('contestName', '')}) — "
            f"{prob['index']}. {prob['name']}\n"
            f"// Submission: {url}\n\n"
        )
        path = os.path.join(out_dir, fn)
        with open(path, "w", encoding="utf-8") as out:
            out.write(header)
            out.write(src.rstrip() + "\n")
        print(f"Wrote {path}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
