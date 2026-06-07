#!/usr/bin/env python3
"""Local grader for IOI 2010 Hotter Colder (HC / Guess)."""

from __future__ import annotations

import argparse
import importlib.util
import sys
from pathlib import Path
from typing import Callable, List, Tuple

COLD, SAME, HOT = -1, 0, 1


def hotter_colder(secret: int, prev: int, guess: int) -> int:
    if abs(guess - secret) < abs(prev - secret):
        return HOT
    if abs(guess - secret) > abs(prev - secret):
        return COLD
    return SAME


def load_hc(path: Path) -> Callable[[int], int]:
    """Load HC from a Python module defining HC(n) using global Guess()."""
    spec = importlib.util.spec_from_file_location("hotter_mod", path)
    assert spec and spec.loader
    mod = importlib.util.module_from_spec(spec)
    guesses: List[int] = []

    def guess(g: int) -> int:
        guesses.append(g)
        if len(guesses) == 1:
            return SAME
        return hotter_colder(mod._secret, guesses[-2], g)

    mod.Guess = guess
    mod._guesses = guesses
    spec.loader.exec_module(mod)
    if not hasattr(mod, "HC"):
        raise SystemExit(f"{path} has no HC(n)")
    return mod.HC


def run_case(n: int, secret: int, hc: Callable[[int], int]) -> Tuple[int, int, bool]:
    import hotter_mod  # type: ignore

    hotter_mod._secret = secret
    hotter_mod._guesses.clear()
    got = hc(n)
    ok = got == secret
    return got, len(hotter_mod._guesses), ok


def max_guesses_subtask(n: int, st: int) -> int:
    if st == 1:
        return 500
    if st == 2:
        return 18
    if st == 3:
        return 16
    # subtask 4: W with 2^W <= 3N
    w = 0
    while (1 << w) <= 3 * n:
        w += 1
    return w - 1


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("solution", type=Path, help="Python file with HC(n)")
    ap.add_argument(
        "--input",
        type=Path,
        default=Path(__file__).parent / "hottercolder/appeal/Subtask1-data/grader.in.1",
    )
    ap.add_argument("--max-n", type=int, default=500)
    ap.add_argument("--limit", type=int, default=0, help="max test lines (0=all)")
    args = ap.parse_args()

    # Inject module name for run_case
    sys.modules["hotter_mod"] = type(sys)("hotter_mod")
    sys.modules["hotter_mod"]._secret = 0
    sys.modules["hotter_mod"]._guesses = []

    spec = importlib.util.spec_from_file_location("hotter_mod", args.solution)
    assert spec and spec.loader
    mod = importlib.util.module_from_spec(spec)

    mod._guesses = []
    spec.loader.exec_module(mod)

    def guess(g: int) -> int:
        mod._guesses.append(g)
        if len(mod._guesses) == 1:
            return SAME
        return hotter_colder(mod._secret, mod._guesses[-2], g)

    mod.Guess = guess
    hc = mod.HC

    lines = args.input.read_text().splitlines()
    if args.limit:
        lines = lines[: args.limit]

    bad = 0
    st_fail = {1: 0, 2: 0, 3: 0, 4: 0}
    worst = 0
    for line in lines:
        n_s, j_s = line.split()
        n, secret = int(n_s), int(j_s)
        if n > args.max_n:
            continue
        mod._secret = secret
        mod._guesses = []
        got = hc(n)
        g = len(mod._guesses)
        worst = max(worst, g)
        if got != secret:
            bad += 1
            if bad <= 5:
                print(f"WRONG n={n} secret={secret} got={got} guesses={g}")
            continue
        for st in (1, 2, 3, 4):
            if g > max_guesses_subtask(n, st):
                st_fail[st] += 1

    total = sum(1 for ln in lines if int(ln.split()[0]) <= args.max_n)
    print(f"tested {total}, wrong {bad}, worst_guesses={worst} (n<={args.max_n})")
    for st in (1, 2, 3, 4):
        ok = total - st_fail[st] - bad
        print(f"  subtask {st}: {ok}/{total} within guess limit")


if __name__ == "__main__":
    main()
