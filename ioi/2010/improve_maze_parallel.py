#!/usr/bin/env python3
"""Run maze_sa.py on multiple tests in parallel, then grade."""

from __future__ import annotations

import argparse
import subprocess
import sys
from concurrent.futures import ProcessPoolExecutor, as_completed
from pathlib import Path

from grade_maze import TEST_SPECS, grade_directory

# Extra SA budget for large grids (warm-start from existing .out).
DEFAULT_ITERS: dict[str, int] = {
    "01": 400_000,
    "02": 2_000_000,
    "03": 2_000_000,
    "04": 2_500_000,
    "05": 2_000_000,
    "06": 600_000,
    "07": 800_000,
    "08": 800_000,
    "09": 600_000,
    "10": 3_000_000,
}


def run_one(
    test_id: str,
    inputs_dir: Path,
    outputs_dir: Path,
    max_iters: int,
    alpha: float,
    seed: int,
    warm_start: bool,
) -> tuple[str, int, str]:
    inp = inputs_dir / test_id
    out = outputs_dir / f"{test_id}.out"
    warm = out if warm_start and out.is_file() else None
    cmd = [
        sys.executable,
        str(Path(__file__).resolve().parent / "maze_sa.py"),
        str(inp),
        "-o",
        str(out),
        "--seed",
        str(seed),
        "--alpha",
        str(alpha),
        "--max-iters",
        str(max_iters),
        "--save-every",
        "0",
        "--log-every",
        "0",
    ]
    if warm is not None:
        cmd.extend(["--warm", str(warm)])
    proc = subprocess.run(cmd, capture_output=True, text=True)
    best = 0
    for line in proc.stderr.splitlines():
        if line.startswith("best_energy="):
            try:
                best = int(line.split("=", 1)[1].split()[0])
            except ValueError:
                pass
    return test_id, best, proc.stderr.strip()[-200:]


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "--inputs-dir",
        type=Path,
        default=Path(__file__).resolve().parent / "g_inputs" / "tests",
    )
    ap.add_argument(
        "--outputs-dir",
        type=Path,
        default=Path(__file__).resolve().parent / "maze_outputs",
    )
    ap.add_argument("--tests", nargs="*", default=[f"{i:02d}" for i in range(1, 11)])
    ap.add_argument("--jobs", type=int, default=4)
    ap.add_argument(
        "--max-iters",
        type=int,
        default=0,
        help="uniform iters for all tests (0 = use per-test DEFAULT_ITERS)",
    )
    ap.add_argument("--alpha", type=float, default=0.999999999)
    ap.add_argument(
        "--warm",
        action="store_true",
        help="warm-start from existing .out (blog advises against)",
    )
    ap.add_argument("--seed", type=int, default=1)
    args = ap.parse_args()

    args.outputs_dir.mkdir(parents=True, exist_ok=True)

    def iters_for(tid: str) -> int:
        if args.max_iters > 0:
            return args.max_iters
        return DEFAULT_ITERS.get(tid, 1_000_000)

    with ProcessPoolExecutor(max_workers=args.jobs) as pool:
        futures = {
            pool.submit(
                run_one,
                tid,
                args.inputs_dir,
                args.outputs_dir,
                iters_for(tid),
                args.alpha,
                args.seed + int(tid),
                args.warm,
            ): tid
            for tid in args.tests
        }
        for fut in as_completed(futures):
            tid, best, tail = fut.result()
            spec = TEST_SPECS.get(tid)
            area = spec[0] * spec[1] if spec else 0
            print(
                f"{tid}: bestE={best}  iters={iters_for(tid)}  "
                f"area={area}  ({tail})"
            )

    print("--- grading ---")
    rows = grade_directory(args.inputs_dir, args.outputs_dir)
    total = sum(r.points for r in rows)
    for r in sorted(rows, key=lambda x: x.test_id):
        if r.ok:
            print(
                f"{r.test_id}: P={r.P}  score={r.points:.4f}  entrance={r.entrance}"
            )
        else:
            print(f"{r.test_id}: INVALID")
    print(f"Total: {total:.4f} / {11 * len(rows)}")


if __name__ == "__main__":
    main()
