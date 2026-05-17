#!/usr/bin/env python3
"""Download latest accepted Codeforces submissions for 8VC Venture Cup 2017.

  qualification — contest 755 (Elimination Round)
  final         — contest 756 (Final Round)

Requires codeforces/.cf_api.env or codeforces/.cf_cookie.

Usage::

    python3 codeforces/8vc/2017/download_submissions.py
"""

from __future__ import annotations

import argparse
import base64
import hashlib
import html
import json
import os
import random
import re
import string
import sys
import time
import urllib.error
import urllib.parse
import urllib.request

ROUNDS: list[tuple[str, int]] = [
    ("qualification", 755),
    ("final", 756),
]

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
DEFAULT_API_ENV = os.path.join(REPO_ROOT, "codeforces", ".cf_api.env")
DEFAULT_COOKIE_FILE = os.path.join(REPO_ROOT, "codeforces", ".cf_cookie")


def load_api_env(path: str) -> tuple[str, str] | None:
    if not os.path.isfile(path):
        return None
    env: dict[str, str] = {}
    with open(path, encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if "=" in line:
                k, v = line.split("=", 1)
                env[k.strip()] = v.strip().strip('"').strip("'")
    key = env.get("CODEFORCES_API_KEY") or env.get("API_KEY")
    secret = env.get("CODEFORCES_API_SECRET") or env.get("API_SECRET")
    if not key or not secret:
        return None
    return key, secret


def load_cookie() -> str | None:
    c = os.environ.get("CODEFORCES_COOKIE", "").strip()
    if c:
        return c
    if os.path.isfile(DEFAULT_COOKIE_FILE):
        with open(DEFAULT_COOKIE_FILE, encoding="utf-8") as f:
            return f.read().strip()
    return None


def cf_signed_url(method: str, secret: str, params: dict[str, str]) -> str:
    base = dict(params)
    base["time"] = str(int(time.time()))
    items = sorted(base.items(), key=lambda kv: (kv[0], kv[1]))
    query = "&".join(f"{k}={v}" for k, v in items)
    rand = "".join(random.choice(string.ascii_letters + string.digits) for _ in range(6))
    to_hash = f"{rand}/{method}?{query}#{secret}"
    digest = hashlib.sha512(to_hash.encode("utf-8")).hexdigest()
    return f"https://codeforces.com/api/{method}?{query}&apiSig={rand}{digest}"


def api_get(url: str) -> dict:
    req = urllib.request.Request(url, headers={"User-Agent": "Mozilla/5.0"})
    with urllib.request.urlopen(req, timeout=120) as r:
        return json.loads(r.read().decode())


def name_slug(name: str) -> str:
    s = name.strip().lower()
    s = re.sub(r"[^a-z0-9]+", "_", s)
    return re.sub(r"_+", "_", s).strip("_")


def extension_for_language(lang: str) -> str:
    if not lang or not lang.strip():
        return "txt"
    low = lang.strip().lower()
    if "kotlin" in low:
        return "kt"
    if "python" in low:
        return "py"
    if "c++" in low or "gnu c++" in low or "clang" in low or "gcc" in low:
        return "cc"
    return "txt"


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


def latest_ok_by_index(handle: str, contest_id: int) -> tuple[str, dict[str, str], dict[str, dict]]:
    st = api_get(f"https://codeforces.com/api/contest.standings?contestId={contest_id}")
    time.sleep(2.1)
    if st.get("status") != "OK":
        raise RuntimeError(st)
    contest_name = st["result"]["contest"]["name"]
    index_to_title = {p["index"]: p["name"] for p in st["result"]["problems"]}

    h = urllib.parse.quote(handle)
    cs = api_get(
        f"https://codeforces.com/api/contest.status?contestId={contest_id}&handle={h}"
    )
    time.sleep(2.1)
    if cs.get("status") != "OK":
        raise RuntimeError(cs)

    best: dict[str, dict] = {}
    for s in cs["result"]:
        if s.get("verdict") != "OK":
            continue
        idx = s["problem"]["index"]
        sid = s["id"]
        prev = best.get(idx)
        if prev is None or sid > prev["id"]:
            best[idx] = s

    return contest_name, index_to_title, best


def collect_sources_api(
    handle: str, best: dict[str, dict], key: str, secret: str
) -> dict[int, dict]:
    targets = {sub["id"] for sub in best.values()}
    collected: dict[int, dict] = {}
    start = 1
    while targets - set(collected.keys()) and start < 5_000_000:
        params = {
            "apiKey": key,
            "handle": handle,
            "includeSources": "true",
            "from": str(start),
            "count": "100",
        }
        url = cf_signed_url("user.status", secret, params)
        data = api_get(url)
        time.sleep(2.1)
        if data.get("status") != "OK":
            raise RuntimeError(data)
        subs = data["result"]
        if not subs:
            break
        for s in subs:
            sid = s["id"]
            if sid in targets and sid not in collected:
                collected[sid] = s
        start += len(subs)
        if len(subs) < 100:
            break
    return collected


def write_submission(
    out_dir: str,
    contest_id: int,
    contest_name: str,
    idx: str,
    title: str,
    sub: dict,
    src: str,
) -> str:
    slug = name_slug(title)
    prefix = idx.lower().replace("+", "plus")
    ext = extension_for_language(sub.get("programmingLanguage", ""))
    fn = f"{prefix}_{slug}.{ext}"
    path = os.path.join(out_dir, fn)
    url_sub = f"https://codeforces.com/contest/{contest_id}/submission/{sub['id']}"
    header = (
        f"// Codeforces {contest_id} ({contest_name}) — {idx}. {title}\n"
        f"// Submission: {url_sub}\n\n"
    )
    with open(path, "w", encoding="utf-8") as out:
        out.write(header)
        out.write(src.rstrip() + "\n")
    return path


def download_round(
    folder: str,
    contest_id: int,
    handle: str,
    api: tuple[str, str] | None,
    cookie: str | None,
    base_dir: str,
) -> int:
    out_dir = os.path.join(base_dir, folder)
    os.makedirs(out_dir, exist_ok=True)

    contest_name, index_to_title, best = latest_ok_by_index(handle, contest_id)
    if not best:
        print(f"{folder}: no accepted submissions for {handle}")
        return 0

    print(f"{folder}: {len(best)} problem(s)")
    wrote = 0

    if api:
        key, secret = api
        collected = collect_sources_api(handle, best, key, secret)
        for idx in sorted(best, key=lambda x: (len(x), x)):
            sub = best[idx]
            sid = sub["id"]
            if sid not in collected or "sourceBase64" not in collected[sid]:
                print(f"  {idx}: missing source via API (id {sid})", file=sys.stderr)
                continue
            title = index_to_title.get(idx, sub["problem"].get("name", idx))
            src = base64.b64decode(collected[sid]["sourceBase64"]).decode(
                "utf-8", errors="replace"
            )
            path = write_submission(
                out_dir, contest_id, contest_name, idx, title, sub, src
            )
            print(f"  Wrote {path}")
            wrote += 1
        return wrote

    if not cookie:
        print(f"  Skipped (no credentials)", file=sys.stderr)
        return 0

    for idx in sorted(best, key=lambda x: (len(x), x)):
        sub = best[idx]
        title = index_to_title.get(idx, sub["problem"].get("name", idx))
        url = f"https://codeforces.com/contest/{contest_id}/submission/{sub['id']}"
        try:
            page = fetch_html(url, cookie)
        except urllib.error.HTTPError as e:
            print(f"  {idx}: HTTP {e.code} for {url}", file=sys.stderr)
            continue
        src = extract_source(page)
        if not src:
            print(f"  {idx}: no source in {url}", file=sys.stderr)
            continue
        path = write_submission(
            out_dir, contest_id, contest_name, idx, title, sub, src
        )
        print(f"  Wrote {path}")
        wrote += 1
    return wrote


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--handle", default="ll931110")
    ap.add_argument("--api-env", default=DEFAULT_API_ENV)
    ap.add_argument(
        "--base-dir",
        default=os.path.dirname(os.path.abspath(__file__)),
    )
    args = ap.parse_args()

    api = load_api_env(args.api_env)
    cookie = None if api else load_cookie()
    if not api and not cookie:
        print(
            "No Codeforces credentials found.\n"
            f"  Set {DEFAULT_API_ENV} or {DEFAULT_COOKIE_FILE}",
            file=sys.stderr,
        )
        return 1

    total = 0
    for folder, contest_id in ROUNDS:
        try:
            total += download_round(
                folder, contest_id, args.handle, api, cookie, args.base_dir
            )
        except Exception as e:
            print(f"{folder}: error: {e}", file=sys.stderr)
            return 1

    print(f"Done: wrote {total} source file(s).")
    return 0 if total > 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
