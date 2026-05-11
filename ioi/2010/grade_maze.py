#!/usr/bin/env python3
"""
Local grader for IOI 2010 Day 2 Problem G — Maze.

For each test, score = min(11, 10^(P / BASE)), where P is the maximum number of
crushed squares on a shortest entrance→cell path (entrance counts as step 1),
and BASE is the constant for that test.

Also validates: dimensions, obstacles unchanged, only '#' may become '.',
exactly one '.' on the field perimeter (entrance), all other '.' strictly
interior (statement constraint).
"""

from __future__ import annotations

import argparse
import math
from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional, Sequence, Tuple

from solve_maze import eccentricity_energy, is_perimeter, parse_grid

# Test id -> (rows, cols, BASE)
TEST_SPECS: dict[str, tuple[int, int, float]] = {
    "01": (6, 10, 20.0),
    "02": (100, 100, 4000.0),
    "03": (100, 100, 4000.0),
    "04": (100, 100, 4000.0),
    "05": (100, 100, 5000.0),
    "06": (11, 11, 54.0),
    "07": (20, 20, 33.0),
    "08": (20, 20, 95.0),
    "09": (11, 21, 104.0),
    "10": (200, 200, 7800.0),
}


def score_points(P: int, base: float) -> float:
    """min(11, 10^(P/BASE)); numerically stable for large P."""
    if base <= 0 or P < 1:
        return 0.0
    x = P / base
    # min(11, 10^x): once 10^x >= 11, saturated at 11
    if x >= math.log10(11):
        return 11.0
    return float(10.0**x)


def score_points_for_test(P: int, test_id: str) -> float:
    """Grader score for eccentricity P on a known test id."""
    spec = TEST_SPECS.get(test_id)
    if spec is None or P < 1:
        return 0.0
    return score_points(P, spec[2])


def marginal_gain_points(P: int, base: float) -> float:
    """How much min(11,10^(P/BASE)) increases when P grows by 1 (tie-breaking aid)."""
    return score_points(P + 1, base) - score_points(P, base)


def validate_output(inp: Sequence[str], out: Sequence[str]) -> List[str]:
    """Return list of human-readable errors (empty iff valid)."""
    errs: List[str] = []
    if not inp:
        errs.append("input is empty")
        return errs
    n, m = len(inp), len(inp[0])
    if len(out) != n:
        errs.append(f"row count {len(out)} != input {n}")
        return errs
    for i, row in enumerate(out):
        if len(row) != m:
            errs.append(f"row {i} length {len(row)} != width {m}")
            return errs
        for j, ch in enumerate(row):
            if ch not in "#.X":
                errs.append(f"invalid character {ch!r} at ({i},{j})")
                return errs

    perimeter_dots = 0
    for i in range(n):
        for j in range(m):
            ich, och = inp[i][j], out[i][j]
            if ich == "X":
                if och != "X":
                    errs.append(f"obstacle changed at ({i},{j})")
                    return errs
                continue
            if ich != "#":
                errs.append(f"unexpected input char {ich!r} at ({i},{j})")
                return errs
            # ich == '#'
            if och not in "#.":
                errs.append(f"invalid output for corn cell at ({i},{j})")
                return errs
            if och == ".":
                if is_perimeter(i, j, n, m):
                    perimeter_dots += 1

    if perimeter_dots != 1:
        errs.append(
            f"need exactly one '.' on field perimeter (entrance), got {perimeter_dots}"
        )
    return errs


@dataclass(frozen=True)
class GradeResult:
    test_id: str
    ok: bool
    P: int
    points: float
    base: float
    errors: Tuple[str, ...]
    entrance: Optional[Tuple[int, int]]


def grade_strings(
    test_id: str,
    inp_lines: Sequence[str],
    out_lines: Sequence[str],
) -> GradeResult:
    """Grade one test given raw text lines."""
    spec = TEST_SPECS.get(test_id)
    if spec is None:
        return GradeResult(
            test_id=test_id,
            ok=False,
            P=0,
            points=0.0,
            base=0.0,
            errors=(f"unknown test id {test_id!r}",),
            entrance=None,
        )

    exp_n, exp_m, base = spec
    inp, n, m = parse_grid(list(inp_lines))
    out, on, om = parse_grid(list(out_lines))

    errs = list(validate_output(inp, out))
    if (n, m) != (exp_n, exp_m):
        errs.insert(0, f"grid size {n}x{m} != expected {exp_n}x{exp_m}")

    if errs:
        return GradeResult(
            test_id=test_id,
            ok=False,
            P=0,
            points=0.0,
            base=base,
            errors=tuple(errs),
            entrance=None,
        )

    P, ent = eccentricity_energy(out, n, m)
    if P < 1:
        return GradeResult(
            test_id=test_id,
            ok=False,
            P=0,
            points=0.0,
            base=base,
            errors=("could not determine entrance / BFS eccentricity",),
            entrance=None,
        )

    pts = score_points(P, base)
    return GradeResult(
        test_id=test_id,
        ok=True,
        P=P,
        points=pts,
        base=base,
        errors=(),
        entrance=ent,
    )


def grade_files(
    test_id: str,
    input_path: Path,
    output_path: Path,
) -> GradeResult:
    inp_text = input_path.read_text(encoding="utf-8")
    out_text = output_path.read_text(encoding="utf-8")
    return grade_strings(test_id, inp_text.splitlines(), out_text.splitlines())


def grade_directory(
    inputs_dir: Path,
    outputs_dir: Path,
) -> List[GradeResult]:
    results: List[GradeResult] = []
    for tid in sorted(TEST_SPECS.keys()):
        inp = inputs_dir / tid
        outp = outputs_dir / f"{tid}.out"
        if not inp.is_file():
            results.append(
                GradeResult(
                    test_id=tid,
                    ok=False,
                    P=0,
                    points=0.0,
                    base=TEST_SPECS[tid][2],
                    errors=(f"missing input {inp}",),
                    entrance=None,
                )
            )
            continue
        if not outp.is_file():
            results.append(
                GradeResult(
                    test_id=tid,
                    ok=False,
                    P=0,
                    points=0.0,
                    base=TEST_SPECS[tid][2],
                    errors=(f"missing output {outp}",),
                    entrance=None,
                )
            )
            continue
        results.append(grade_files(tid, inp, outp))
    return results


def main() -> None:
    root = Path(__file__).resolve().parent
    ap = argparse.ArgumentParser(description="Grade IOI 2010 Maze outputs")
    ap.add_argument(
        "--inputs-dir",
        type=Path,
        default=root / "g_inputs" / "tests",
    )
    ap.add_argument(
        "--outputs-dir",
        type=Path,
        default=root / "maze_outputs",
    )
    ap.add_argument(
        "--json",
        action="store_true",
        help="print machine-readable JSON lines",
    )
    args = ap.parse_args()

    rows = grade_directory(args.inputs_dir, args.outputs_dir)
    total = sum(r.points for r in rows)

    if args.json:
        import json

        for r in rows:
            print(
                json.dumps(
                    {
                        "test": r.test_id,
                        "ok": r.ok,
                        "P": r.P,
                        "BASE": r.base,
                        "points": round(r.points, 6),
                        "errors": list(r.errors),
                    }
                )
            )
        print(json.dumps({"total_points": round(total, 6)}))
        return

    for r in rows:
        if r.ok:
            print(
                f"{r.test_id}: P={r.P}  BASE={r.base:g}  "
                f"score=min(11,10^(P/BASE))={r.points:.4f}  entrance={r.entrance}"
            )
        else:
            print(f"{r.test_id}: INVALID  score=0.00")
            for e in r.errors:
                print(f"         - {e}")
    print(f"Total (sum of tests): {total:.4f} / {11 * len(TEST_SPECS)}")


if __name__ == "__main__":
    main()
