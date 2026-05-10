#!/usr/bin/env python3
"""
Download Codeforces contest 2222 problem statements from mirror.codeforces.com
(main codeforces.com is often behind Cloudflare for bots).

Run from repo root or this directory:
  python3 fetch_statements.py
"""

from __future__ import annotations

import re
import ssl
import sys
import urllib.request
from pathlib import Path

CONTEST_ID = 2222
CONTEST_URL = f"https://codeforces.com/contest/{CONTEST_ID}"
MIRROR_BASE = f"https://mirror.codeforces.com/contest/{CONTEST_ID}/problem"

# Letter, filename stem (matches renamed .cc sources), display title from standings API
PROBLEMS: list[tuple[str, str, str]] = [
    ("A", "a_wonderful_contest", "A Wonderful Contest"),
    ("B", "b_artistic_balance_tree", "Artistic Balance Tree"),
    ("C", "c_median_partition", "Median Partition"),
    ("D", "d_permutation_construction", "Permutation Construction"),
    ("E", "e_seek_the_truth", "Seek the Truth"),
]

UA = "Mozilla/5.0 (compatible; spectral-local-mirror/1.0)"
CTX = ssl.create_default_context()


def fetch(url: str) -> str:
    req = urllib.request.Request(url, headers={"User-Agent": UA})
    with urllib.request.urlopen(req, context=CTX, timeout=120) as resp:
        return resp.read().decode("utf-8", errors="replace")


def extract_problem_statement(html: str) -> str | None:
    m = re.search(r'<div class="problem-statement"[^>]*>', html)
    if not m:
        return None
    start = m.start()
    i = m.end()
    depth = 1
    while i < len(html) and depth > 0:
        next_open = html.find("<div", i)
        next_close = html.find("</div>", i)
        if next_close == -1:
            return None
        if next_open != -1 and next_open < next_close:
            depth += 1
            gt = html.find(">", next_open)
            if gt == -1:
                return None
            i = gt + 1
        else:
            depth -= 1
            end_close = next_close + len("</div>")
            if depth == 0:
                return html[start:end_close]
            i = end_close
    return None


def absolutize_cf_assets(fragment: str) -> str:
    """Use main codeforces.com host for assets (mirror paths may differ)."""
    origin = "https://codeforces.com"
    out = re.sub(
        r'(["\'])\s*/(problemsets|contests)/',
        rf"\1{origin}/\2/",
        fragment,
    )
    out = re.sub(
        r'(\n\s*)"/(problemsets|contests)/',
        rf'\1"{origin}/\2/',
        out,
    )
    return out


def main() -> None:
    root = Path(__file__).resolve().parent
    out_dir = root / "statements"
    out_dir.mkdir(parents=True, exist_ok=True)

    index_lines = [
        f"# Spectral Cup 2026 R1 (CF {CONTEST_ID}) — problem statements",
        "",
        f"Contest: <{CONTEST_URL}>",
        "",
        "HTML statements were scraped from `mirror.codeforces.com` (Cloudflare blocks many automated requests to the main site). Re-run `fetch_statements.py` to refresh.",
        "",
        "| File | Problem |",
        "|------|---------|",
    ]

    for letter, stem, title in PROBLEMS:
        url = f"{MIRROR_BASE}/{letter}"
        try:
            html = fetch(url)
        except Exception as e:
            print(f"FAIL {letter}: {e}", file=sys.stderr)
            continue

        body = extract_problem_statement(html)
        if body:
            body = absolutize_cf_assets(body)

        md_lines = [
            f"# {letter}. {title}",
            "",
            f"**Official:** <{CONTEST_URL}/problem/{letter}>",
            f"**Mirror used:** <{url}>",
            "",
        ]
        if body:
            md_lines.append("## Statement (HTML)")
            md_lines.append("")
            md_lines.append(body)
            md_lines.append("")
        else:
            md_lines.append(
                "*Could not extract `problem-statement`; open the official URL in a browser.*"
            )
            md_lines.append("")

        (out_dir / f"{stem}.md").write_text("\n".join(md_lines), encoding="utf-8")
        index_lines.append(f"| [`{stem}.md`]({stem}.md) | {letter} — {title} |")
        print(f"ok {letter} -> {stem}.md")

    (out_dir / "INDEX.md").write_text("\n".join(index_lines) + "\n", encoding="utf-8")
    print(f"Wrote {out_dir}/INDEX.md")


if __name__ == "__main__":
    main()
