#!/usr/bin/env python3
"""
Extract each <div class="problem-statement"> from a saved Codeforces contest
'Problems' page (e.g. Save As … on https://codeforces.com/contest/<id>/problems).

Usage:
  python3 parse_saved_cf_problems_html.py /path/to/Problems.html [output_dir]

Default output_dir: ./statements_from_browser (under this script's directory).

Filenames use the same stems as fetch_statements.py for A–E; F–H use f/g/h slugs.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

# Matches stems used in this folder for solutions / mirror fetch
STEM_BY_INDEX: dict[str, str] = {
    "A": "a_wonderful_contest",
    "B": "b_artistic_balance_tree",
    "C": "c_median_partition",
    "D": "d_permutation_construction",
    "E": "e_seek_the_truth",
    "F": "f_building_tree",
    "G": "g_statistics_on_tree",
    "H": "h_counting_sort",
}


def extract_problem_statement(html_fragment: str) -> str | None:
    m = re.search(r'<div class="problem-statement"[^>]*>', html_fragment)
    if not m:
        return None
    start = m.start()
    i = m.end()
    depth = 1
    while i < len(html_fragment) and depth > 0:
        next_open = html_fragment.find("<div", i)
        next_close = html_fragment.find("</div>", i)
        if next_close == -1:
            return None
        if next_open != -1 and next_open < next_close:
            depth += 1
            gt = html_fragment.find(">", next_open)
            if gt == -1:
                return None
            i = gt + 1
        else:
            depth -= 1
            end_close = next_close + len("</div>")
            if depth == 0:
                return html_fragment[start:end_close]
            i = end_close
    return None


def split_holders(full: str) -> list[tuple[str, str]]:
    """Return [(letter, html_chunk)] for each problemindexholder block."""
    pat = re.compile(
        r'<div\s+class="problemindexholder"\s+problemindex="([A-Z])"',
        re.IGNORECASE,
    )
    matches = list(pat.finditer(full))
    out: list[tuple[str, str]] = []
    for j, m in enumerate(matches):
        letter = m.group(1).upper()
        start = m.start()
        end = matches[j + 1].start() if j + 1 < len(matches) else len(full)
        out.append((letter, full[start:end]))
    return out


def title_from_statement(stmt_html: str) -> str | None:
    m = re.search(r'<div class="title">([^<]+)</div>', stmt_html)
    return m.group(1).strip() if m else None


def main() -> None:
    if len(sys.argv) < 2:
        print(__doc__, file=sys.stderr)
        sys.exit(2)

    path = Path(sys.argv[1]).expanduser().resolve()
    out_dir = Path(sys.argv[2]) if len(sys.argv) > 2 else Path(__file__).parent / "statements_from_browser"
    out_dir.mkdir(parents=True, exist_ok=True)

    full = path.read_text(encoding="utf-8", errors="replace")

    holders = split_holders(full)
    if not holders:
        print("No problemindexholder blocks found.", file=sys.stderr)
        sys.exit(1)

    index_md_lines = [
        "# Parsed from saved browser HTML",
        "",
        f"Source file: `{path}`",
        "",
        "| Letter | File | Title |",
        "|--------|------|-------|",
    ]

    for letter, chunk in holders:
        stmt = extract_problem_statement(chunk)
        stem = STEM_BY_INDEX.get(letter, letter.lower())
        title = title_from_statement(stmt) if stmt else letter

        md_path = out_dir / f"{stem}.md"
        lines = [
            f"# {title}" if title else f"# Problem {letter}",
            "",
            f"**Parsed from:** `{path.name}` (problem index **{letter}**)",
            "",
        ]
        if stmt:
            lines.append("## Statement (HTML)")
            lines.append("")
            lines.append(stmt)
            lines.append("")
        else:
            lines.append("*Could not find `problem-statement` in this holder.*")
            lines.append("")

        md_path.write_text("\n".join(lines), encoding="utf-8")
        title_esc = (title or "").replace("|", "\\|")
        index_md_lines.append(f"| {letter} | `{stem}.md` | {title_esc} |")
        print(f"wrote {md_path.name}")

    (out_dir / "INDEX.md").write_text("\n".join(index_md_lines) + "\n", encoding="utf-8")
    print(f"Wrote {out_dir / 'INDEX.md'}")


if __name__ == "__main__":
    main()
