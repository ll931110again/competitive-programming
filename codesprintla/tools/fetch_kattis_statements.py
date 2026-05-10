#!/usr/bin/env python3
"""
Download Kattis problem pages and save under codesprintla/<year>/statements/:

  - statements/<slug>.html.full   — full HTML page (offline archive)
  - statements/<slug>.md          — title + problembody HTML fragment

Run from repo root:
  python3 codesprintla/tools/fetch_kattis_statements.py

2025 problems live on open.kattis.com. 2026 contest problems are fetched from the
CodeSprint LA 2026 contest host (they may 404 on open.kattis until published).
"""

from __future__ import annotations

import html as html_module
import re
import ssl
import sys
import time
import urllib.request
from pathlib import Path
from urllib.parse import urlparse

UA = "Mozilla/5.0 (compatible; codesprintla-local-mirror/1.0)"
CTX = ssl.create_default_context()

PROBLEMS_2025 = [
    "amulet",
    "arcanesecret",
    "balancingthecouncil",
    "chemtechcontagion",
    "evasion",
    "getjinxed",
    "hextechordnance",
    "mappingtheundercity",
    "olympicceremony",
    "popcornbutton",
    "teamup2",
    "topsidevszaun",
]

PROBLEMS_2026 = [
    "cutthecake2",
    "guardianlaser",
    "goroncity",
    "leftshift",
    "lynelmelee",
    "mappingthegreatsea",
    "multiversemadness",
    "ocarinaandchime",
    "robbiesresearchquest",
    "seizethemeans",
    "sorcerersrift",
    "spirittracks",
]


def fetch(url: str) -> str:
    req = urllib.request.Request(url, headers={"User-Agent": UA})
    with urllib.request.urlopen(req, context=CTX, timeout=60) as resp:
        return resp.read().decode("utf-8", errors="replace")


def extract_title(html: str) -> str | None:
    m = re.search(
        r'<h1 class="book-page-heading">\s*(.+?)\s*</h1>',
        html,
        re.DOTALL | re.IGNORECASE,
    )
    if not m:
        return None
    raw = m.group(1)
    raw = re.sub(r"<br\s*/?>", " — ", raw, flags=re.IGNORECASE)
    raw = re.sub(r"<[^>]+>", "", raw)
    s = re.sub(r"\s+", " ", raw).strip()
    return html_module.unescape(s)


def extract_problembody_inner(html: str) -> str | None:
    marker = '<div class="problembody">'
    start = html.find(marker)
    if start == -1:
        return None
    i = start + len(marker)
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


def absolutize_statement_assets(fragment: str, page_url: str) -> str:
    """Rewrite root-relative `/problems/` and `/assets/` URLs for offline Markdown preview."""
    p = urlparse(page_url)
    origin = f"{p.scheme}://{p.netloc}"
    out = re.sub(
        r'(["\'])\s*/(problems|assets)/',
        rf"\1{origin}/\2/",
        fragment,
    )
    out = re.sub(
        r'(\n\s*)"/(problems|assets)/',
        rf'\1"{origin}/\2/',
        out,
    )
    return out


def write_index(
    out_dir: Path,
    year: str,
    items: list[tuple[str, str, str]],
    source_blurb: str,
) -> None:
    lines = [
        f"# CodeSprint LA {year} — problem statements (local mirror)",
        "",
        source_blurb,
        "",
        "| Slug | Title | Files |",
        "|------|-------|-------|",
    ]
    for slug, title, url in sorted(items, key=lambda x: x[0]):
        title_esc = title.replace("|", "\\|")
        lines.append(
            f"| [{slug}]({url}) | {title_esc} | `{slug}.md`, `{slug}.html.full` |"
        )
    lines.append("")
    (out_dir / "INDEX.md").write_text("\n".join(lines), encoding="utf-8")


def mirror_year(
    root: Path,
    year: str,
    slugs: list[str],
    url_for_slug: dict[str, str] | None,
    index_blurb: str,
    delay_sec: float = 0.35,
) -> None:
    """If url_for_slug is None, use default open.kattis.com/problems/{slug}."""
    out_dir = root / "codesprintla" / year / "statements"
    out_dir.mkdir(parents=True, exist_ok=True)
    index_items: list[tuple[str, str, str]] = []

    for slug in slugs:
        if url_for_slug and slug in url_for_slug:
            url = url_for_slug[slug]
        else:
            url = f"https://open.kattis.com/problems/{slug}"
        try:
            html = fetch(url)
        except Exception as e:
            print(f"FAIL {year} {slug}: {e}", file=sys.stderr)
            continue

        (out_dir / f"{slug}.html.full").write_text(html, encoding="utf-8")

        title = extract_title(html) or slug
        body = extract_problembody_inner(html)
        if body:
            body = absolutize_statement_assets(body, url)

        md_parts = [
            f"# {title}",
            "",
            f"**Source:** <{url}>",
            "",
        ]
        if body:
            md_parts.append("## Statement (HTML fragment from `problembody`)")
            md_parts.append("")
            md_parts.append(body)
            md_parts.append("")
        else:
            md_parts.append(
                "*Could not extract `problembody`; see the `.html.full` file.*"
            )
            md_parts.append("")

        (out_dir / f"{slug}.md").write_text(
            "\n".join(md_parts), encoding="utf-8"
        )
        index_items.append((slug, title, url))
        print(f"ok {year} {slug} ({title})")
        time.sleep(delay_sec)

    write_index(out_dir, year, index_items, index_blurb)


def open_kattis_url(slug: str) -> str:
    return f"https://open.kattis.com/problems/{slug}"


def contest_2026_url(slug: str) -> str:
    return (
        "https://codesprintla26.kattis.com/contests/codesprintla26open/problems/"
        + slug
    )


def main() -> None:
    root = Path(__file__).resolve().parents[2]

    mirror_year(
        root,
        "2025",
        PROBLEMS_2025,
        None,
        "Fetched from [open.kattis.com](https://open.kattis.com/).",
    )

    url_map_2026 = {s: contest_2026_url(s) for s in PROBLEMS_2026}
    mirror_year(
        root,
        "2026",
        PROBLEMS_2026,
        url_map_2026,
        "Fetched from the [CodeSprint LA 2026 open contest]"
        "(https://codesprintla26.kattis.com/contests/codesprintla26open) "
        "(problem pages may not exist on open.kattis until published).",
    )
    print("Done.")


if __name__ == "__main__":
    main()
