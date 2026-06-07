#!/usr/bin/env python3
"""KL-divergence n-gram language models for IOI 2010 Language."""

from __future__ import annotations

import argparse
import math
from dataclasses import dataclass
from pathlib import Path

from tune_language import load_data

MASK = (1 << 18) - 1
V = 1 << 18


def roll_hash(prev: int, sym: int) -> int:
    return (prev * 65537 + sym) & MASK


def excerpt_profile(e: list[int], order: int) -> tuple[dict[int, int], int]:
    counts: dict[int, int] = {}
    total = 0
    for i in range(100 - order + 1):
        h = 0
        for j in range(order):
            h = roll_hash(h, e[i + j])
        counts[h] = counts.get(h, 0) + 1
        total += 1
    return counts, total


def kl_divergence(
    profile: dict[int, int],
    total_e: int,
    model: list[int],
    model_total: int,
    alpha: float,
) -> float:
    denom = model_total + alpha * V
    kl = 0.0
    for h, c in profile.items():
        p = c / total_e
        q = (model[h] + alpha) / denom
        kl += p * math.log(p / q)
    return kl


@dataclass(frozen=True)
class KLConfig:
    orders: tuple[int, ...] = (4,)
    alpha: float = 0.01
    weights: tuple[float, ...] | None = None  # per order, default equal


class KLModel:
    def __init__(self, cfg: KLConfig) -> None:
        self.cfg = cfg
        self.counts = {
            o: [[0] * V for _ in range(56)] for o in cfg.orders
        }
        self.totals = {o: [0] * 56 for o in cfg.orders}
        self.have = [False] * 56

    def predict(self, e: list[int]) -> int:
        weights = self.cfg.weights
        if weights is None:
            weights = tuple(1.0 / len(self.cfg.orders) for _ in self.cfg.orders)

        best = 0
        best_score = 1e100
        profiles = {o: excerpt_profile(e, o) for o in self.cfg.orders}

        for lang in range(56):
            if not self.have[lang]:
                continue
            score = 0.0
            for o, w in zip(self.cfg.orders, weights):
                prof, tot = profiles[o]
                score += w * kl_divergence(
                    prof, tot, self.counts[o][lang], self.totals[o][lang], self.cfg.alpha
                )
            if score < best_score:
                best_score = score
                best = lang
        return best

    def update(self, y: int, e: list[int]) -> None:
        self.have[y] = True
        for o in self.cfg.orders:
            prof, tot = excerpt_profile(e, o)
            row = self.counts[o][y]
            for h, c in prof.items():
                row[h] += c
            self.totals[o][y] += tot


def run(data: list[tuple[int, list[int]]], cfg: KLConfig) -> float:
    model = KLModel(cfg)
    correct = 0
    for y, e in data:
        if model.predict(e) == y:
            correct += 1
        model.update(y, e)
    return correct / len(data)


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "--data",
        type=Path,
        default=Path(__file__).resolve().parent
        / "language/appeal/Subtask1-data/grader.in.1",
    )
    args = ap.parse_args()
    data = load_data(args.data)

    configs = [
        ("4-gram KL a=0.01", KLConfig(orders=(4,), alpha=0.01)),
        ("4-gram KL a=0.1", KLConfig(orders=(4,), alpha=0.1)),
        ("4-gram KL a=1.0", KLConfig(orders=(4,), alpha=1.0)),
        ("3-gram KL", KLConfig(orders=(3,), alpha=0.01)),
        ("2-gram KL", KLConfig(orders=(2,), alpha=0.01)),
        ("2+3+4 KL", KLConfig(orders=(2, 3, 4), alpha=0.01)),
        ("3+4 KL", KLConfig(orders=(3, 4), alpha=0.01)),
        ("4-gram KL a=0.05", KLConfig(orders=(4,), alpha=0.05)),
        ("4-gram KL a=0.5", KLConfig(orders=(4,), alpha=0.5)),
    ]

    results: list[tuple[str, float]] = []
    for name, cfg in configs:
        acc = run(data, cfg)
        results.append((name, acc))
        print(f"{acc:.4f}  {name}")

    print("\nTop:")
    for name, acc in sorted(results, key=lambda t: -t[1])[:5]:
        print(f"{acc:.4f}  {name}")


if __name__ == "__main__":
    main()
