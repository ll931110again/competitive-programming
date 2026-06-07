#!/usr/bin/env python3
"""
Improve maze outputs with per-test SA budgets and multi-seed restarts.

Keeps the best-scoring output per test. Large grids get more iterations and
several random seeds; small tests use shorter runs.
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from concurrent.futures import ProcessPoolExecutor, as_completed
from pathlib import Path

from grade_maze import grade_directory, grade_files
from pack_maze_submission import pack

ROOT = Path(__file__).resolve().parent
CPP_BIN = ROOT / "maze_sa"

# Iteration budget per test (tuned for ~10–20 min wall time on 100x100 with -O3).
ITERS: dict[str, int] = {
    "01": 300_000,
    "02": 6_000_000,
    "03": 8_000_000,
    "04": 6_000_000,
    "05": 6_000_000,
    "06": 500_000,
    "07": 600_000,
    "08": 600_000,
    "09": 500_000,
    "10": 8_000_000,
}

# Extra random restarts for weak / obstacle-dense cases.
RESTARTS: dict[str, int] = {
    "03": 4,
    "10": 3,
}


def compile_solver() -> None:
    subprocess.run(
        [
            "g++",
            "-O3",
            "-std=c++17",
            "-march=native",
            str(ROOT / "maze_sa.cc"),
            "-o",
            str(CPP_BIN),
        ],
        check=True,
    )


def parse_best(stderr: str) -> int:
    best = 0
    for line in stderr.splitlines():
        m = re.search(r"best_energy=(\d+)", line)
        if m:
            best = int(m.group(1))
    return best


def run_seed(
    test_id: str,
    inputs_dir: Path,
    work_dir: Path,
    iters: int,
    seed: int,
) -> tuple[int, int, Path]:
    inp = inputs_dir / test_id
    out = work_dir / f"{test_id}_seed{seed}.out"
    cmd = [
        str(CPP_BIN),
        str(inp),
        str(out),
        str(iters),
        "--seed",
        str(seed),
    ]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    if proc.returncode != 0:
        raise RuntimeError(f"{test_id} seed={seed} failed:\n{proc.stderr}")
    return seed, parse_best(proc.stderr), out


def improve_test(
    test_id: str,
    inputs_dir: Path,
    outputs_dir: Path,
    iters: int,
    restarts: int,
    jobs: int,
) -> tuple[str, int, float]:
    work_dir = outputs_dir / "_work"
    work_dir.mkdir(parents=True, exist_ok=True)

    seeds = [1000 + int(test_id) * 97 + k * 131 for k in range(restarts)]
    results: list[tuple[int, int, Path]] = []

    if jobs <= 1 or restarts == 1:
        for seed in seeds:
            results.append(run_seed(test_id, inputs_dir, work_dir, iters, seed))
    else:
        with ProcessPoolExecutor(max_workers=min(jobs, restarts)) as pool:
            futs = [
                pool.submit(run_seed, test_id, inputs_dir, work_dir, iters, seed)
                for seed in seeds
            ]
            for fut in as_completed(futs):
                results.append(fut.result())

    best_seed, best_p, best_path = max(results, key=lambda t: t[1])
    final = outputs_dir / f"{test_id}.out"
    final.write_text(best_path.read_text())
    graded = grade_files(test_id, inputs_dir / test_id, final)
    print(
        f"{test_id}: best P={graded.P} score={graded.points:.4f} "
        f"(seed={best_seed}, {restarts} restart(s), {iters} iters)"
    )
    return test_id, graded.P, graded.points


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "--tests",
        default="02,03,04,05,10",
        help="comma-separated test ids (default: large tests)",
    )
    ap.add_argument("--all", action="store_true", help="run all 10 tests")
    ap.add_argument("--inputs-dir", type=Path, default=ROOT / "g_inputs" / "tests")
    ap.add_argument("--outputs-dir", type=Path, default=ROOT / "maze_outputs")
    ap.add_argument("--jobs", type=int, default=2, help="parallel seeds per test")
    ap.add_argument("--pack", action="store_true")
    ap.add_argument("--compile", action="store_true")
    args = ap.parse_args()

    if args.compile or not CPP_BIN.is_file():
        compile_solver()

    if args.all:
        tests = [f"{i:02d}" for i in range(1, 11)]
    else:
        tests = [t.strip() for t in args.tests.split(",") if t.strip()]

    args.outputs_dir.mkdir(parents=True, exist_ok=True)

    for test_id in tests:
        iters = ITERS.get(test_id, 2_000_000)
        restarts = RESTARTS.get(test_id, 2)
        improve_test(
            test_id,
            args.inputs_dir,
            args.outputs_dir,
            iters,
            restarts,
            args.jobs,
        )

    rows = grade_directory(args.inputs_dir, args.outputs_dir)
    total = sum(r.points for r in rows)
    print(f"Total: {total:.4f} / {11 * len(rows)}")
    if args.pack:
        pack(args.outputs_dir, ROOT / "maze_outputs.zip")


if __name__ == "__main__":
    main()
