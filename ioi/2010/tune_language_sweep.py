#!/usr/bin/env python3
"""Parallel sweep of language scoring configs."""

from __future__ import annotations

import json
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

DATA = Path(__file__).resolve().parent / "language/appeal/Subtask1-data/grader.in.1"
ONE = Path(__file__).resolve().parent / "tune_language_one.py"

CONFIGS = [
    ("baseline", {}),
    ("tri0.1", {"w_trigram": 0.1}),
    ("tri0.15", {"w_trigram": 0.15}),
    ("tri0.2", {"w_trigram": 0.2}),
    ("tri0.25", {"w_trigram": 0.25}),
    ("tri0.3", {"w_trigram": 0.3}),
    ("tri0.35", {"w_trigram": 0.35}),
    ("tri0.4", {"w_trigram": 0.4}),
    ("tri0.5", {"w_trigram": 0.5}),
    ("tri0.75", {"w_trigram": 0.75}),
    ("tri1.0", {"w_trigram": 1.0}),
    ("tri1.5", {"w_trigram": 1.5}),
    ("tri2.0", {"w_trigram": 2.0}),
    ("tri only", {"w_rocchio": 0, "w_bigram": 0, "w_trigram": 1.0}),
    ("big+tri0.25 no roc", {"w_rocchio": 0, "w_trigram": 0.25}),
    ("roc+tri0.25 no big", {"w_bigram": 0, "w_trigram": 0.25}),
    ("skip only", {"skip_space": True}),
    ("tri0.25+skip", {"w_trigram": 0.25, "skip_space": True}),
    ("tri0.5+skip", {"w_trigram": 0.5, "skip_space": True}),
    ("char64 w=1", {"char_symbols": True, "char_top_k": 64, "char_weight": 1.0}),
    ("char64 w=0.5", {"char_symbols": True, "char_top_k": 64, "char_weight": 0.5}),
    ("char64+tri0.25", {"char_symbols": True, "char_top_k": 64, "w_trigram": 0.25, "char_weight": 0.25}),
    ("char128+tri0.25", {"char_symbols": True, "char_top_k": 128, "w_trigram": 0.25, "char_weight": 0.25}),
    ("char32+tri0.25", {"char_symbols": True, "char_top_k": 32, "w_trigram": 0.25, "char_weight": 0.25}),
    ("uni0.25+tri0.75+skip", {"w_unigram": 0.25, "w_trigram": 0.75, "skip_space": True}),
]


def run_one(name: str, cfg: dict) -> tuple[str, float]:
    proc = subprocess.run(
        [
            sys.executable,
            str(ONE),
            "--data",
            str(DATA),
            "--name",
            name,
            "--config",
            json.dumps(cfg),
        ],
        capture_output=True,
        text=True,
        check=True,
    )
    out = json.loads(proc.stdout.strip())
    return out["name"], out["accuracy"]


def main() -> None:
    results: list[tuple[str, float]] = []
    with ThreadPoolExecutor(max_workers=4) as pool:
        futs = [pool.submit(run_one, name, cfg) for name, cfg in CONFIGS]
        for fut in as_completed(futs):
            name, acc = fut.result()
            results.append((name, acc))
            print(f"{acc:.4f}  {name}", flush=True)

    print("\nTop 8:")
    for name, acc in sorted(results, key=lambda t: -t[1])[:8]:
        print(f"{acc:.4f}  {name}")


if __name__ == "__main__":
    main()
