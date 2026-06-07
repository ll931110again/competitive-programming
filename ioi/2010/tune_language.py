#!/usr/bin/env python3
"""Fast sweep of language-identification scoring variants."""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path


def load_data(path: Path) -> list[tuple[int, list[int]]]:
    data: list[tuple[int, list[int]]] = []
    code_id: dict[str, int] = {}
    next_id = 0
    with path.open() as f:
        for line in f:
            parts = line.split()
            code = parts[0]
            e = list(map(int, parts[1:101]))
            if code not in code_id:
                code_id[code] = next_id
                next_id += 1
            data.append((code_id[code], e))
    return data


def big_index(a: int, b: int) -> int:
    return (a * 65537 + b) & ((1 << 18) - 1)


def tri_index(a: int, b: int, c: int) -> int:
    return ((a * 65537 + b) * 65537 + c) & ((1 << 18) - 1)


@dataclass(frozen=True)
class Config:
    w_rocchio: float = 1.0
    w_bigram: float = 1.0
    w_trigram: float = 0.0
    w_unigram: float = 0.0
    skip_space: bool = False
    char_symbols: bool = False
    char_top_k: int = 64
    char_weight: float = 1.0


class Model:
    __slots__ = (
        "cfg",
        "seen",
        "have",
        "uni",
        "bigram",
        "tri",
        "char_set",
        "totcnt",
        "space",
        "calls",
    )

    def __init__(self, cfg: Config) -> None:
        self.cfg = cfg
        self.seen = [[False] * 65536 for _ in range(56)]
        self.have = [False] * 56
        self.uni = [[0] * 65536 for _ in range(56)]
        self.bigram = [[0] * (1 << 18) for _ in range(56)]
        self.tri = [[0] * (1 << 18) for _ in range(56)] if cfg.w_trigram else None
        self.char_set = [set() for _ in range(56)]
        self.totcnt = [0] * 65536
        self.space = 0
        self.calls = 0

    def predict(self, e: list[int]) -> int:
        cfg = self.cfg
        best = 0
        best_score = -1e18
        skip = cfg.skip_space and self.calls >= 20
        space = self.space

        for lang in range(56):
            if not self.have[lang]:
                continue
            score = 0.0
            local = [False] * 65536
            seen = self.seen[lang]
            uni = self.uni[lang]
            bigram = self.bigram[lang]
            tri = self.tri
            char_set = self.char_set[lang]

            for sym in e:
                if skip and sym == space:
                    continue
                if cfg.w_rocchio and not local[sym] and seen[sym]:
                    score += cfg.w_rocchio
                local[sym] = True
                if cfg.w_unigram:
                    score += cfg.w_unigram * uni[sym]

            if cfg.char_symbols and char_set:
                for sym in e:
                    if sym in char_set:
                        score += cfg.char_weight

            for i in range(1, 100):
                a, b = e[i - 1], e[i]
                if skip and (a == space or b == space):
                    continue
                score += cfg.w_bigram * bigram[big_index(a, b)]

            if tri is not None:
                tri_lang = tri[lang]
                w_tri = cfg.w_trigram
                for i in range(2, 100):
                    a, b, c = e[i - 2], e[i - 1], e[i]
                    if skip and (a == space or b == space or c == space):
                        continue
                    score += w_tri * tri_lang[tri_index(a, b, c)]

            if score > best_score:
                best_score = score
                best = lang
        return best

    def update(self, y: int, e: list[int]) -> None:
        cfg = self.cfg
        self.calls += 1
        self.have[y] = True
        for sym in e:
            self.totcnt[sym] += 1
            self.uni[y][sym] += 1
            self.seen[y][sym] = True
        if cfg.skip_space and self.calls == 20:
            self.space = max(range(65536), key=lambda i: self.totcnt[i])
        for i in range(1, 100):
            self.bigram[y][big_index(e[i - 1], e[i])] += 1
        if self.tri is not None:
            tri_y = self.tri[y]
            for i in range(2, 100):
                tri_y[tri_index(e[i - 2], e[i - 1], e[i])] += 1
        if cfg.char_symbols:
            ranked = sorted(
                ((sym, self.uni[y][sym]) for sym in range(65536) if self.uni[y][sym] > 0),
                key=lambda t: -t[1],
            )
            self.char_set[y] = {sym for sym, _ in ranked[: cfg.char_top_k]}


def run(data: list[tuple[int, list[int]]], cfg: Config) -> float:
    model = Model(cfg)
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
    if not args.data.is_file():
        raise SystemExit(f"missing {args.data}")

    data = load_data(args.data)
    configs = [
        ("baseline rocchio+bigram", Config()),
        ("space skip only", Config(skip_space=True)),
        ("trigram 0.25", Config(w_trigram=0.25)),
        ("trigram 0.5", Config(w_trigram=0.5)),
        ("trigram 0.75", Config(w_trigram=0.75)),
        ("trigram 1.0", Config(w_trigram=1.0)),
        ("trigram 1.5", Config(w_trigram=1.5)),
        ("trigram 2.0", Config(w_trigram=2.0)),
        ("tri only", Config(w_rocchio=0, w_bigram=0, w_trigram=1.0)),
        ("tri+skip w=0.25", Config(w_trigram=0.25, skip_space=True)),
        ("tri+skip w=0.5", Config(w_trigram=0.5, skip_space=True)),
        ("tri+skip w=0.75", Config(w_trigram=0.75, skip_space=True)),
        ("tri+skip w=1.0", Config(w_trigram=1.0, skip_space=True)),
        ("tri+skip w=1.5", Config(w_trigram=1.5, skip_space=True)),
        ("uni0.25+tri0.75+skip", Config(w_unigram=0.25, w_trigram=0.75, skip_space=True)),
        ("char top64", Config(char_symbols=True, char_top_k=64)),
        ("char64+big", Config(char_symbols=True, char_top_k=64, char_weight=0.5)),
        ("char64+tri0.5", Config(char_symbols=True, char_top_k=64, w_trigram=0.5)),
        ("char64+skip", Config(char_symbols=True, char_top_k=64, skip_space=True)),
        ("char64+tri0.5+skip", Config(char_symbols=True, char_top_k=64, w_trigram=0.5, skip_space=True, char_weight=0.25)),
    ]

    results: list[tuple[str, float]] = []
    for name, cfg in configs:
        acc = run(data, cfg)
        results.append((name, acc))
        print(f"{acc:.4f}  {name}", flush=True)

    print("\nTop 5:", flush=True)
    for name, acc in sorted(results, key=lambda t: -t[1])[:5]:
        print(f"{acc:.4f}  {name}", flush=True)


if __name__ == "__main__":
    main()
