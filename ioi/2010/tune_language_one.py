#!/usr/bin/env python3
"""Run one language config (for parallel sweeps)."""

from __future__ import annotations

import argparse
import json
from pathlib import Path

from tune_language import Config, load_data, run


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--data", type=Path, required=True)
    ap.add_argument("--name", required=True)
    ap.add_argument("--config", required=True, help="JSON object of Config fields")
    args = ap.parse_args()

    cfg = Config(**json.loads(args.config))
    data = load_data(args.data)
    acc = run(data, cfg)
    print(json.dumps({"name": args.name, "accuracy": acc}))


if __name__ == "__main__":
    main()
