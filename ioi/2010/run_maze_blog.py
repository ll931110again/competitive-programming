#!/usr/bin/env python3
"""
Run IOI 2010 Maze simulated annealing per CF blog entry 118853.

Defaults: t0=2.5, alpha=0.999999999, save every 2^23 iterations.
Uses maze_sa (C++) when compiled, else maze_sa.py.

Blog recommendation: long runs without warm-start; random initial state with
~70%% interior '.' density and a random perimeter entrance.

Example (single test, run until killed):
  python3 run_maze_blog.py 04

Example (all tests, 5M iterations each, parallel):
  python3 run_maze_blog.py --all --jobs 4 --max-iters 5000000
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from concurrent.futures import ProcessPoolExecutor, as_completed
from pathlib import Path

from grade_maze import grade_directory
from pack_maze_submission import pack

ROOT = Path(__file__).resolve().parent
CPP_BIN = ROOT / "maze_sa"
PY_SCRIPT = ROOT / "maze_sa.py"


def solver_cmd() -> list[str]:
    if CPP_BIN.is_file():
        return [str(CPP_BIN)]
    return [sys.executable, str(PY_SCRIPT)]


def run_one(
    test_id: str,
    inputs_dir: Path,
    outputs_dir: Path,
    max_iters: int,
    alpha: float,
    warm: bool,
    seed: int,
) -> tuple[str, str]:
    inp = inputs_dir / test_id
    out = outputs_dir / f"{test_id}.out"
    base = solver_cmd()

    if CPP_BIN.is_file():
        cmd = base + [str(inp), str(out)]
        if max_iters > 0:
            cmd.append(str(max_iters))
        if warm and out.is_file():
            cmd.extend(["--warm", str(out)])
    else:
        cmd = base + [str(inp), "-o", str(out), "--seed", str(seed), "--alpha", str(alpha)]
        if max_iters > 0:
            cmd.extend(["--max-iters", str(max_iters)])
        if warm and out.is_file():
            cmd.extend(["--warm", str(out)])

    proc = subprocess.run(cmd, capture_output=True, text=True)
    tail = proc.stderr.strip().splitlines()[-1] if proc.stderr else ""
    return test_id, tail


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("test", nargs="?", help="test id 01-10 (omit with --all)")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--inputs-dir", type=Path, default=ROOT / "g_inputs" / "tests")
    ap.add_argument("--outputs-dir", type=Path, default=ROOT / "maze_outputs")
    ap.add_argument("--jobs", type=int, default=1)
    ap.add_argument(
        "--max-iters",
        type=int,
        default=0,
        help="0 = run until killed (C++ only); Python needs a positive limit",
    )
    ap.add_argument("--alpha", type=float, default=0.999999999)
    ap.add_argument("--warm", action="store_true", help="warm-start from existing .out")
    ap.add_argument("--pack", action="store_true", help="rebuild maze_outputs.zip after")
    ap.add_argument("--compile", action="store_true", help="build maze_sa from maze_sa.cc")
    args = ap.parse_args()

    if args.compile or not CPP_BIN.is_file():
        subprocess.run(
            ["g++", "-O2", "-std=c++17", str(ROOT / "maze_sa.cc"), "-o", str(CPP_BIN)],
            check=True,
        )

    tests = [f"{i:02d}" for i in range(1, 11)] if args.all else [args.test]
    if not tests or tests == [None]:
        ap.error("provide test id or --all")

    args.outputs_dir.mkdir(parents=True, exist_ok=True)
    max_iters = args.max_iters
    if not CPP_BIN.is_file() and max_iters <= 0:
        max_iters = 2_000_000
        print("Python solver: default --max-iters 2000000", file=sys.stderr)

    if args.jobs <= 1:
        for tid in tests:
            tid, tail = run_one(
                tid, args.inputs_dir, args.outputs_dir, max_iters, args.alpha, args.warm, int(tid)
            )
            print(f"{tid}: {tail}")
    else:
        with ProcessPoolExecutor(max_workers=args.jobs) as pool:
            futs = [
                pool.submit(
                    run_one,
                    tid,
                    args.inputs_dir,
                    args.outputs_dir,
                    max_iters,
                    args.alpha,
                    args.warm,
                    int(tid),
                )
                for tid in tests
            ]
            for fut in as_completed(futs):
                tid, tail = fut.result()
                print(f"{tid}: {tail}")

    rows = grade_directory(args.inputs_dir, args.outputs_dir)
    total = sum(r.points for r in rows)
    print(f"Total: {total:.4f} / {11 * len(rows)}")
    if args.pack:
        pack(args.outputs_dir, ROOT / "maze_outputs.zip")


if __name__ == "__main__":
    main()
