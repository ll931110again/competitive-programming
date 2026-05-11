#!/usr/bin/env python3
"""Generate 01.out … 10.out from g_inputs/tests/* using solve_maze.solve_one."""

from __future__ import annotations

import argparse
import random
import time
from pathlib import Path

from grade_maze import grade_strings
from solve_maze import parse_grid, solve_one


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "--inputs-dir",
        type=Path,
        default=Path(__file__).resolve().parent / "g_inputs" / "tests",
    )
    ap.add_argument(
        "--out-dir",
        type=Path,
        default=Path(__file__).resolve().parent / "maze_outputs",
    )
    ap.add_argument(
        "--random-walk-trials",
        type=int,
        default=-1,
        help="solve_one trials (-1 = auto budget for score)",
    )
    args = ap.parse_args()

    args.out_dir.mkdir(parents=True, exist_ok=True)

    total_score = 0.0
    for name in [f"{i:02d}" for i in range(1, 11)]:
        in_path = args.inputs_dir / name
        out_path = args.out_dir / f"{name}.out"
        text = in_path.read_text(encoding="utf-8")
        g_in, n, m = parse_grid(text.splitlines())

        t0 = time.perf_counter()
        rng = random.Random(int(name))
        out = solve_one(g_in, rng, random_walk_trials=args.random_walk_trials)
        dt = time.perf_counter() - t0

        out_path.write_text("\n".join(out) + "\n", encoding="utf-8")
        gr = grade_strings(name, text.splitlines(), out)
        if gr.ok:
            total_score += gr.points
            print(
                f"{name}: {n}x{m}  wall={dt:.1f}s  P={gr.P}  "
                f"score=min(11,10^(P/BASE))={gr.points:.4f}  entrance={gr.entrance}  "
                f"-> {out_path}"
            )
        else:
            print(
                f"{name}: {n}x{m}  wall={dt:.1f}s  INVALID  "
                f"-> {out_path}"
            )
            for err in gr.errors:
                print(f"         {err}")

    print(f"Total grader score (sum): {total_score:.4f} / 110")


if __name__ == "__main__":
    main()
