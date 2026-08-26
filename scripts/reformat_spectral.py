#!/usr/bin/env python3
"""Reformat Spectral Cup C++ solutions toward the repo C++ style guide."""

from __future__ import annotations

import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SPECTRAL = ROOT / "codeforces" / "spectral"

HEADERS = {
    "R1/a_wonderful_contest.cc": """\
// Codeforces Spectral::Cup 2026 Round 1 — Wonderful Contest
// https://codeforces.com/contest/2222/problem/A
//
// Sketch
// ------
// YES iff some rating equals 100. O(n) per test.
""",
    "R1/b_artistic_balance_tree.cc": """\
// Codeforces Spectral::Cup 2026 Round 1 — Artistic Balance Tree
// https://codeforces.com/contest/2222/problem/B
//
// Sketch
// ------
// Split the array into even/odd positions. Operations on an index flip a
// "marked" flag for that parity. After sorting each parity, the sequence is
// non-decreasing iff each unmarked parity is sorted ascending and each marked
// parity is sorted descending. O(n log n).
""",
    "R1/c_median_partition.cc": """\
// Codeforces Spectral::Cup 2026 Round 1 — Median Partition
// https://codeforces.com/contest/2222/problem/C
//
// Sketch
// ------
// Let m be the median of the whole array. DP: max number of odd-length pieces
// whose median is m. A segment is valid iff it contains m and m sits in the
// median slot. O(n^2) per test.
""",
    "R1/d_permutation_construction.cc": """\
// Codeforces Spectral::Cup 2026 Round 1 — Permutation Construction
// https://codeforces.com/contest/2222/problem/D
//
// Sketch
// ------
// Rank prefix sums in decreasing order and emit that rank as the permutation
// value. O(n log n).
""",
    "R1/e_seek_the_truth.cc": """\
// Codeforces Spectral::Cup 2026 Round 1 — Seek the Truth
// https://codeforces.com/contest/2222/problem/E
//
// Sketch
// ------
// Interactive. Distinguish AND / OR / XOR of a hidden mask by inserting 0 and
// the all-ones value, then recover bits by inserting single-bit probes.
""",
    "R2/a_slimes_on_a_line.cc": """\
// Codeforces Spectral::Cup 2026 Round 2 — Slimes on a Line
// https://codeforces.com/contest/2229/problem/A
//
// Sketch
// ------
// After sorting, the minimum possible max gap after equalizing toward the
// middle is ceil((max - min) / 2). O(n log n).
""",
    "R2/b_absolute_cinema.cc": """\
// Codeforces Spectral::Cup 2026 Round 2 — Absolute Cinema
// https://codeforces.com/contest/2229/problem/B
//
// Sketch
// ------
// Swap each pair so a_i <= b_i, then the answer is determined by comparing
// the two sequences. Linear scan after the swaps.
""",
    "R2/c1_we_be_flipping_easy_version.cc": """\
// Codeforces Spectral::Cup 2026 Round 2 — We Be Flipping (easy)
// https://codeforces.com/contest/2229/problem/C1
//
// Sketch
// ------
// Greedily flip suffixes from the right so every position becomes zero.
// Record the flip operations. O(n).
""",
    "R2/c2_we_be_flipping_hard_version.cc": """\
// Codeforces Spectral::Cup 2026 Round 2 — We Be Flipping (hard)
// https://codeforces.com/contest/2229/problem/C2
//
// Sketch
// ------
// Same suffix-flip idea as C1, with prefix sums of values and absolute values
// to compute the minimum cost. O(n).
""",
    "R2/d_me_when_median_problem.cc": """\
// Codeforces Spectral::Cup 2026 Round 2 — Me When Median Problem
// https://codeforces.com/contest/2229/problem/D
//
// Sketch
// ------
// Binary search the median x. A check scans the two arrays as events and
// tracks a running delta. O(n log A).
""",
    "R2/e_deconstruction_tree.cc": """\
// Codeforces Spectral::Cup 2026 Round 2 — Deconstruction Tree
// https://codeforces.com/contest/2229/problem/E
//
// Sketch
// ------
// DP on the path from the rightmost leaf toward n. Prefix sums of reachable
// ways, modulo 998244353. O(n).
""",
    "R2/f_load_unbalancing.cc": """\
// Codeforces Spectral::Cup 2026 Round 2 — Load Unbalancing
// https://codeforces.com/contest/2229/problem/F
//
// Sketch
// ------
// Binary search the load threshold and SOS/subset DP over n <= 18 items
// to test whether extra capacity can form enough groups. O(2^n n log ANS).
""",
    "R3/a_who_watches_the_watchpig.cc": """\
// Codeforces Spectral::Cup 2026 Round 3 — Who Watches the Watchpig
// https://codeforces.com/contest/2245/problem/A
//
// Sketch
// ------
// Need 2k <= n. Count L among the first k and R among the last k. O(n).
""",
    "R3/b_delete_and_concatenate.cc": """\
// Codeforces Spectral::Cup 2026 Round 3 — Delete and Concatenate
// https://codeforces.com/contest/2245/problem/B
//
// Sketch
// ------
// Shift values by c, sort, and take the largest half (and any remaining
// positives). O(n log n).
""",
    "R3/c_mexor.cc": """\
// Codeforces Spectral::Cup 2026 Round 3 — Mexor
// https://codeforces.com/contest/2245/problem/C
//
// Sketch
// ------
// Construct an array whose XOR-mex related parameter equals k, or report
// impossible. Bit-length of n/k drives the construction. O(n).
""",
    "R3/d2_construct_an_array_hard_version.cc": """\
// Codeforces Spectral::Cup 2026 Round 3 — Construct an Array (hard)
// https://codeforces.com/contest/2245/problem/D2
//
// Sketch
// ------
// Model constraints as a 2-colored graph. Peel vertices whose degree in
// either color is 0; the leftover orientation is a valid construction. O(n+m).
""",
    "R3/e_tom_and_jerry.cc": """\
// Codeforces Spectral::Cup 2026 Round 3 — Tom and Jerry
// https://codeforces.com/contest/2245/problem/E
//
// Sketch
// ------
// On the tree, DFS through even-degree components and charge odd-degree
// neighbors. The total is the number of winning first paths. O(n).
""",
    "R3/f_familiar.cc": """\
// Codeforces Spectral::Cup 2026 Round 3 — Familiar
// https://codeforces.com/contest/2245/problem/F
//
// Sketch
// ------
// Interval DP on permutations with stack-height constraints. f[i][j][c] counts
// relative permutations of [i, j] that end with c items on the stack; g is the
// sum over c. Recurrence over the position of the minimum yields O(n^3).
""",
}

I64_ALIAS = re.compile(r"^\s*using\s+i64\s*=\s*long\s+long\s*;\s*$", re.MULTILINE)
MAXN = re.compile(r"^#define\s+maxn\s+(\d+)\s*$", re.MULTILINE)
MOD_DEFINE = re.compile(r"^#define\s+mod\s+998'244'353\s*$", re.MULTILINE)


def rewrite_body(text: str) -> str:
    text = I64_ALIAS.sub("", text)
    text = MAXN.sub(r"constexpr int kMaxN = \1;", text)
    text = MOD_DEFINE.sub("constexpr int kMod = 998'244'353;", text)
    text = text.replace("unsigned long long", "__ULL__")
    text = text.replace("long long", "int64_t")
    text = text.replace("__ULL__", "unsigned long long")
    text = re.sub(r"\bi64\b", "int64_t", text)
    text = re.sub(r"\bmaxn\b", "kMaxN", text)
    # Keep interactive flushes; only rewrite batch cin.tie.
    text = text.replace("cin.tie(0);", "cin.tie(nullptr);")
    text = text.replace("cin.tie(0)", "cin.tie(nullptr)")
    # Avoid turning the identifier `mod` inside comments; only a #define was
    # converted. Remaining `mod` uses in e_deconstruction_tree become kMod
    # when they were the macro.
    return text


def ensure_header(rel: str, text: str) -> str:
    header = HEADERS.get(rel)
    if header is None:
        return text
    if "Sketch" in text.split("#include", 1)[0]:
        return text
    # Drop a leading blank line and any existing one-line comments before includes.
    rest = text.lstrip("\n")
    if rest.startswith("/*"):
        end = rest.find("*/")
        if end != -1:
            rest = rest[end + 2 :].lstrip("\n")
    while rest.startswith("//"):
        rest = rest.split("\n", 1)[1] if "\n" in rest else ""
        rest = rest.lstrip("\n")
    return header + "\n" + rest


def main() -> None:
    files = sorted(SPECTRAL.rglob("*.cc"))
    for path in files:
        rel = path.relative_to(SPECTRAL).as_posix()
        original = path.read_text()
        text = rewrite_body(original)
        text = ensure_header(rel, text)
        if rel.endswith("e_deconstruction_tree.cc"):
            text = re.sub(r"(?<!\w)mod(?!\w)", "kMod", text)
            text = text.replace("void DFS(", "void dfs(")
            text = text.replace("DFS(", "dfs(")
        path.write_text(text)

    subprocess.check_call(
        ["clang-format", "-i", *[str(p) for p in files]],
        cwd=ROOT,
    )
    print(f"reformatted {len(files)} files")


if __name__ == "__main__":
    main()
