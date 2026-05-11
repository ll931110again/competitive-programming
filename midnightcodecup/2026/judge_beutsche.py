#!/usr/bin/env python3
"""
Simulate Midnight Code Cup 2026 — A. Beutsche Dahn schedules and score them locally.

Scoring (inferred from statement + sample; weighted delay is what you should minimize):
  contribution_i = P_i * (T_arrival_i - T_appearance_i)
  S = sum_i contribution_i
  points_est = SCALE / sqrt(S)   (SCALE chosen so numbers are comparable to judge scale)

P_i by number of train rides (pick operations for that passenger):
  1 ride -> 1.0
  2 rides -> 1.05
  3 rides -> 1.2
  4 rides -> 1.5
  >4 rides -> invalid

Constraints checked: output format, capacity, edges, one train per undirected edge per tick,
move budget, tick budget, every tradesman delivered.

Usage:
  python3 judge_beutsche.py tests/01 solution.out
  python3 judge_beutsche.py --batch tests solution_dir   # expects 01..08 in each
"""

from __future__ import annotations

import argparse
import math
import os
import re
import sys
from dataclasses import dataclass, field
from typing import List, Optional, Tuple


def penalty_rides(num_picks: int) -> Optional[float]:
    m = {1: 1.0, 2: 1.05, 3: 1.2, 4: 1.5}
    return m.get(num_picks)


def read_input(text: str):
    it = iter(map(int, re.findall(r"-?\d+", text)))
    V = next(it)
    E = next(it)
    edges = []
    adj = [[] for _ in range(V + 1)]
    for _ in range(E):
        u, v = next(it), next(it)
        edges.append((u, v))
        adj[u].append(v)
        adj[v].append(u)
    Ttr = next(it)
    train_start = [0] + [next(it) for _ in range(Ttr)]
    C = next(it)
    N = next(it)
    passengers = [(0, 0, 0)]  # 1-based
    for _ in range(N):
        u, v, t = next(it), next(it), next(it)
        passengers.append((u, v, t))
    return {
        "V": V,
        "E": E,
        "adj": adj,
        "Ttr": Ttr,
        "train_start": train_start,
        "C": C,
        "N": N,
        "passengers": passengers,
    }


def parse_output(text: str) -> List[Tuple[int, List[str], List[Tuple[int, int]]]]:
    """Returns list of ticks: (K, pickdrop_lines, moves as (train, city))."""
    lines = text.splitlines()
    if not lines:
        raise ValueError("empty output")
    p = 0
    S = int(lines[p])
    p += 1
    ticks = []
    for _ in range(S):
        if p >= len(lines):
            raise ValueError("truncated at tick header")
        K = int(lines[p])
        p += 1
        cmds = []
        for _k in range(K):
            if p >= len(lines):
                raise ValueError("truncated in commands")
            cmds.append(lines[p].strip())
            p += 1
        if p >= len(lines):
            raise ValueError("truncated before M")
        M = int(lines[p])
        p += 1
        moves = []
        for _m in range(M):
            if p >= len(lines):
                raise ValueError("truncated in moves")
            a, b = map(int, lines[p].split())
            moves.append((a, b))
            p += 1
        ticks.append((K, cmds, moves))
    if p != len(lines):
        # allow trailing whitespace
        while p < len(lines) and not lines[p].strip():
            p += 1
        if p != len(lines):
            raise ValueError(f"junk after schedule line {p + 1}")
    return ticks


def simulate(inp: dict, ticks: List[Tuple[int, List[str], List[Tuple[int, int]]]]):
    V = inp["V"]
    Ttr = inp["Ttr"]
    C = inp["C"]
    N = inp["N"]
    adj = inp["adj"]
    passengers = inp["passengers"]

    train_pos = inp["train_start"][:]
    onboard = [[] for _ in range(Ttr + 1)]  # passenger ids
    waiting = [[] for _ in range(V + 1)]

    appeared = [False] * (N + 1)
    on_train = [0] * (N + 1)
    picks = [0] * (N + 1)
    done = [False] * (N + 1)
    arrival_tick = [0] * (N + 1)

    edge_set = lambda a, b: (min(a, b), max(a, b))

    def neighbor(u, v):
        return v in adj[u]

    total_moves = 0

    for tick_idx, (_K, cmds, moves) in enumerate(ticks, start=1):
        if tick_idx > 10**6:
            return False, "too many ticks in file", None

        # 1) appearances
        for i in range(1, N + 1):
            if done[i]:
                continue
            if not appeared[i] and passengers[i][2] == tick_idx:
                appeared[i] = True
                u = passengers[i][0]
                waiting[u].append(i)

        # 2) pick / drop
        for line in cmds:
            parts = line.split()
            if len(parts) != 3:
                return False, f"bad command line: {line!r}", None
            typ, ts, ps = parts
            t_id = int(ts)
            p_id = int(ps)
            if typ == "pick":
                if not (1 <= t_id <= Ttr and 1 <= p_id <= N):
                    return False, f"pick bad id tick={tick_idx}", None
                if done[p_id]:
                    return False, f"pick done passenger tick={tick_idx} p={p_id}", None
                if on_train[p_id]:
                    return False, f"pick already onboard tick={tick_idx}", None
                city = train_pos[t_id]
                if not appeared[p_id]:
                    return False, f"pick before appearance tick={tick_idx} p={p_id}", None
                if p_id not in waiting[city]:
                    return False, f"pick not at city tick={tick_idx} train@{city} p={p_id}", None
                if len(onboard[t_id]) >= C:
                    return False, f"capacity tick={tick_idx}", None
                waiting[city].remove(p_id)
                onboard[t_id].append(p_id)
                on_train[p_id] = t_id
                picks[p_id] += 1
            elif typ == "drop":
                if not (1 <= t_id <= Ttr and 1 <= p_id <= N):
                    return False, "drop bad id", None
                if on_train[p_id] != t_id:
                    return False, f"drop not on train tick={tick_idx}", None
                city = train_pos[t_id]
                dest = passengers[p_id][1]
                onboard[t_id].remove(p_id)
                on_train[p_id] = 0
                if city == dest:
                    if picks[p_id] not in (1, 2, 3, 4):
                        return False, f"too many rides p={p_id}", None
                    done[p_id] = True
                    arrival_tick[p_id] = tick_idx
                else:
                    waiting[city].append(p_id)
            else:
                return False, f"unknown cmd {line!r}", None

        # 3) moves
        seen_edges = set()
        move_trains = set()
        for t_id, nxt in moves:
            if not (1 <= t_id <= Ttr and 1 <= nxt <= V):
                return False, "move bad id", None
            if t_id in move_trains:
                return False, f"duplicate train move tick={tick_idx}", None
            move_trains.add(t_id)
            u = train_pos[t_id]
            if u == nxt:
                return False, "zero-length move", None
            if not neighbor(u, nxt):
                return False, f"not an edge tick={tick_idx} {u}->{nxt}", None
            e = edge_set(u, nxt)
            if e in seen_edges:
                return False, f"edge collision tick={tick_idx} edge={e}", None
            seen_edges.add(e)
            train_pos[t_id] = nxt
        total_moves += len(moves)

    for i in range(1, N + 1):
        if not done[i]:
            return False, f"passenger {i} never finished", None

    if total_moves > 2 * 10**6:
        return False, "move budget exceeded", None

    # score
    S = 0.0
    detail = []
    for i in range(1, N + 1):
        t0 = passengers[i][2]
        t1 = arrival_tick[i]
        P = penalty_rides(picks[i])
        if P is None:
            return False, f"bad ride count p={i} picks={picks[i]}", None
        c = P * (t1 - t0)
        S += c
        detail.append((i, t0, t1, t1 - t0, picks[i], P, c))

    return True, "OK", {"sum_weighted": S, "detail": detail, "total_moves": total_moves, "ticks": len(ticks)}


def points_estimate(sum_weighted: float, scale: float = 3000.0) -> float:
    """Monotonic in solution quality; same ordering as 1/sqrt(S) for fixed scale."""
    if sum_weighted <= 0:
        return float("inf")
    return scale / math.sqrt(sum_weighted)


def run_one(inp_path: str, out_path: str, scale: float) -> Tuple[bool, str, Optional[dict]]:
    with open(inp_path) as f:
        inp = read_input(f.read())
    with open(out_path) as f:
        ticks = parse_output(f.read())
    return simulate(inp, ticks)


def main():
    ap = argparse.ArgumentParser(description="Local judger + weighted-delay objective for Beutsche Dahn")
    ap.add_argument("input_path", nargs="?", help=".in / test file")
    ap.add_argument("output_path", nargs="?", help=".out schedule")
    ap.add_argument("--scale", type=float, default=3000.0, help="points_est = scale/sqrt(S)")
    ap.add_argument(
        "--batch",
        nargs=2,
        metavar=("INPUT_DIR", "OUT_DIR"),
        help="Score tests named 01..08 (reads INPUT_DIR/XX OUT_DIR/XX.out)",
    )
    args = ap.parse_args()

    if args.batch:
        zpad = ["01", "02", "03", "04", "05", "06", "07", "08"]
        rows = []
        tot_S = 0.0
        for z in zpad:
            tests_dir = args.batch[0]
            outs_dir = args.batch[1]
            cand = [
                os.path.join(tests_dir, z),
                os.path.join(tests_dir, "tests", z),
                os.path.join(tests_dir, z + ".in"),
            ]
            inp_path = next((p for p in cand if os.path.isfile(p)), None)
            outp = os.path.join(outs_dir, z + ".out")
            if inp_path is None or not os.path.isfile(outp):
                continue
            ok, msg, data = run_one(inp_path, outp, args.scale)
            if not ok:
                print(f"{z}\tFAIL\t{msg}")
                sys.exit(1)
            S = data["sum_weighted"]
            tot_S += S
            rows.append((z, S, data["ticks"], data["total_moves"]))
        print("test\tsum_weighted\tticks\tmoves")
        for z, S, tk, mv in rows:
            print(f"{z}\t{S:.6g}\t{tk}\t{mv}")
        print(f"TOTAL_sum_weighted\t{tot_S:.6g}")
        est = points_estimate(tot_S, args.scale)
        print(f"points_est(sum, scale={args.scale})\t{est:.6g}")
        return 0

    if not args.input_path or not args.output_path:
        ap.error("need input_path output_path or --batch")

    ok, msg, data = run_one(args.input_path, args.output_path, args.scale)
    print(f"status: {msg}")
    if not ok:
        sys.exit(1)
    S = data["sum_weighted"]
    print(f"sum_weighted = Σ P_i (T_arrival - T_appear) = {S:.6g}")
    print(f"points_est (scale={args.scale}) = {points_estimate(S, args.scale):.6g}")
    print(f"ticks = {data['ticks']}  total_moves = {data['total_moves']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
