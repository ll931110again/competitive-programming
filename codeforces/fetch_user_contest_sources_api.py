#!/usr/bin/env python3
"""Download your Codeforces submission sources via the signed API.

Uses ``user.status`` with ``includeSources=true`` (sources come back as
``sourceBase64``). Only works when ``CODEFORCES_API_KEY`` / ``CODEFORCES_API_SECRET``
in ``codeforces/.cf_api.env`` belong to the same account as ``--handle``.

Writes one file per problem index: latest accepted submission per index.

Example::

    python3 codeforces/fetch_user_contest_sources_api.py \\
        --handle ll931110 --contest-id 1810 --out-dir codeton/codeton4
"""

from __future__ import annotations

import argparse
import base64
import hashlib
import json
import os
import random
import re
import string
import sys
import time
import urllib.parse
import urllib.request


def load_api_env(path: str) -> tuple[str, str]:
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
        raise SystemExit(f"Missing CODEFORCES_API_KEY / CODEFORCES_API_SECRET in {path}")
    return key, secret


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
    """Pick a file extension from Codeforces ``programmingLanguage``."""
    if not lang or not lang.strip():
        return "txt"
    low = lang.strip().lower()
    if low.startswith("kotlin"):
        return "kt"
    if low == "go":
        return "go"
    if "python" in low:
        return "py"
    if "javascript" in low or "node" in low:
        return "js"
    if "rust" in low:
        return "rs"
    if "go " in low or low == "go":
        return "go"
    if "java" in low and "javascript" not in low:
        return "java"
    if "c++" in low or "gnu c++" in low or "clang" in low or "gcc" in low:
        return "cc"
    if "c#" in low or "mono c#" in low:
        return "cs"
    return "txt"


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--handle", required=True)
    ap.add_argument("--contest-id", type=int, required=True)
    ap.add_argument("--out-dir", required=True)
    ap.add_argument(
        "--api-env",
        default="codeforces/.cf_api.env",
        help="Path with CODEFORCES_API_KEY and CODEFORCES_API_SECRET",
    )
    ap.add_argument(
        "--ext",
        default=None,
        metavar="EXT",
        help="Force file extension (e.g. kt). Default: infer from submission language.",
    )
    args = ap.parse_args()

    key, secret = load_api_env(args.api_env)

    standings_url = (
        f"https://codeforces.com/api/contest.standings?contestId={args.contest_id}"
    )
    st = api_get(standings_url)
    if st.get("status") != "OK":
        print(st, file=sys.stderr)
        return 1
    contest_name = st["result"]["contest"]["name"]
    problems = st["result"]["problems"]

    h = urllib.parse.quote(args.handle)
    cs_url = f"https://codeforces.com/api/contest.status?contestId={args.contest_id}&handle={h}"
    cs = api_get(cs_url)
    time.sleep(2.1)
    if cs.get("status") != "OK":
        print(cs, file=sys.stderr)
        return 1

    best: dict[str, dict] = {}
    for s in cs["result"]:
        if s.get("verdict") != "OK":
            continue
        idx = s["problem"]["index"]
        sid = s["id"]
        prev = best.get(idx)
        if prev is None or sid > prev["id"]:
            best[idx] = s

    targets = {sub["id"] for sub in best.values()}
    collected: dict[int, dict] = {}
    start = 1
    while targets - set(collected.keys()) and start < 5_000_000:
        params = {
            "apiKey": key,
            "handle": args.handle,
            "includeSources": "true",
            "from": str(start),
            "count": "100",
        }
        url = cf_signed_url("user.status", secret, params)
        data = api_get(url)
        time.sleep(2.1)
        if data.get("status") != "OK":
            print(data, file=sys.stderr)
            return 1
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

    missing = targets - set(collected.keys())
    if missing:
        print(
            f"Warning: could not find sources for submission ids: {sorted(missing)}",
            file=sys.stderr,
        )

    for idx in list(best.keys()):
        bid = best[idx]["id"]
        if bid not in collected:
            del best[idx]
        else:
            best[idx] = collected[bid]

    out_dir = os.path.abspath(args.out_dir)
    os.makedirs(out_dir, exist_ok=True)

    index_to_title = {p["index"]: p["name"] for p in problems}

    for idx, sub in sorted(best.items(), key=lambda kv: kv[1]["id"]):
        title = index_to_title.get(idx, sub["problem"].get("name", idx))
        slug = name_slug(title)
        prefix = idx.lower().replace("+", "plus")
        ext = (
            args.ext.lstrip(".")
            if args.ext
            else extension_for_language(sub.get("programmingLanguage", ""))
        )
        if ext == "txt":
            print(
                f"Note: unknown language {sub.get('programmingLanguage')!r} "
                f"for {idx}, using .txt",
                file=sys.stderr,
            )
        fn = f"{prefix}_{slug}.{ext}"
        path = os.path.join(out_dir, fn)

        src_raw = base64.b64decode(sub["sourceBase64"]).decode("utf-8", errors="replace")
        src = src_raw.rstrip() + "\n"
        url_sub = (
            f"https://codeforces.com/contest/{args.contest_id}/submission/{sub['id']}"
        )
        header = (
            f"// Codeforces {args.contest_id} ({contest_name}) — "
            f"{idx}. {title}\n"
            f"// Submission: {url_sub}\n\n"
        )
        with open(path, "w", encoding="utf-8") as out:
            out.write(header)
            out.write(src)
        print(f"Wrote {path}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
