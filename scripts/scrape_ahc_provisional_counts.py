#!/usr/bin/env python3
"""Scrape official provisional test counts from AtCoder AHC task statements."""

from __future__ import annotations

import json
import re
import time
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ATCODER = ROOT / "atcoder"
OUT_JSON = ATCODER / "AHC_PROVISIONAL_TESTS.json"
OUT_MD = ATCODER / "AHC_PROVISIONAL_TESTS.md"

TASK_SLUG: dict[str, str] = {
    "ahc023": "asprocon10_a",
}

PATTERNS = [
    r"Provisional test:\s*(\d+)",
    r"暫定テスト\s*(\d+)個",
    r"Provisional tests consist of\s*(\d+)",
    r"テストケースは全部で\s*(\d+)個",
    r"合計で\s*(\d+)\s*個のテストケース",
    r"(\d+)\s*test cases",
]


def task_slug(contest: str) -> str:
    if contest in TASK_SLUG:
        return TASK_SLUG[contest]
    for cc in sorted((ATCODER / contest).glob("a_*.cc")):
        text = cc.read_text(encoding="utf-8", errors="replace")
        m = re.search(rf"/contests/{contest}/tasks/([a-z0-9_]+)", text)
        if m:
            return m.group(1)
    return f"{contest}_a"


def scrape_provisional(contest: str) -> int | None:
    slug = task_slug(contest)
    url = f"https://atcoder.jp/contests/{contest}/tasks/{slug}"
    req = urllib.request.Request(url, headers={"User-Agent": "competitive-programming/1.0"})
    html = urllib.request.urlopen(req, timeout=30).read().decode("utf-8", errors="replace")
    for pat in PATTERNS:
        m = re.search(pat, html, re.I)
        if m:
            return int(m.group(1))
    return None


def local_inputs(contest: str) -> int:
    in_dir = ATCODER / contest / "tools" / "in"
    if not in_dir.is_dir():
        return 0
    return len(list(in_dir.glob("*.txt")))


def main() -> None:
    results: dict[str, dict] = {}
    for num in range(1, 68):
        contest = f"ahc{num:03d}"
        entry: dict = {"task": task_slug(contest)}
        try:
            entry["provisional"] = scrape_provisional(contest)
        except Exception as exc:  # noqa: BLE001
            entry["error"] = str(exc)
        entry["local_in"] = local_inputs(contest)
        if entry.get("provisional") is not None:
            prov = entry["provisional"]
            local = entry["local_in"]
            if local < prov:
                entry["status"] = f"missing {prov - local} inputs (have {local}/{prov})"
            elif local > prov:
                entry["status"] = f"extra {local - prov} local inputs (calibrate on first {prov})"
            else:
                entry["status"] = "ok"
        results[contest] = entry
        print(f"{contest}: provisional={entry.get('provisional')} local={entry['local_in']}")
        time.sleep(0.12)

    OUT_JSON.write_text(json.dumps(results, indent=2) + "\n", encoding="utf-8")

    lines = [
        "# AHC provisional test counts",
        "",
        "Official **provisional** case counts from each task statement.",
        "Calibrate solutions on seeds `0 .. N-1` (first `N` files in `tools/in/`).",
        "",
        "Regenerate: `python3 scripts/scrape_ahc_provisional_counts.py`",
        "",
        "| Contest | Provisional | Local `in/` | Status |",
        "|---------|------------:|------------:|--------|",
    ]
    for num in range(1, 68):
        cid = f"ahc{num:03d}"
        e = results[cid]
        prov = e.get("provisional", "—")
        local = e.get("local_in", 0)
        status = e.get("status", e.get("error", "—"))
        lines.append(f"| {cid} | {prov} | {local} | {status} |")
    lines.append("")
    OUT_MD.write_text("\n".join(lines), encoding="utf-8")
    print(f"wrote {OUT_JSON} and {OUT_MD}")


if __name__ == "__main__":
    main()
