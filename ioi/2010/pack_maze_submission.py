#!/usr/bin/env python3
"""Zip maze_outputs/01.out … 10.out into maze_outputs.zip."""

from __future__ import annotations

import argparse
import zipfile
from pathlib import Path

from grade_maze import grade_directory

TEST_IDS = [f"{i:02d}" for i in range(1, 11)]


def pack(outputs_dir: Path, out_zip: Path) -> None:
    outputs_dir = outputs_dir.resolve()
    out_zip.parent.mkdir(parents=True, exist_ok=True)

    with zipfile.ZipFile(out_zip, "w", compression=zipfile.ZIP_DEFLATED) as zf:
        for tid in TEST_IDS:
            src = outputs_dir / f"{tid}.out"
            if not src.is_file():
                raise SystemExit(f"missing {src}")
            zf.write(src, arcname=f"{tid}.out")

    print(f"wrote {out_zip} ({out_zip.stat().st_size} bytes)")


def main() -> None:
    root = Path(__file__).resolve().parent
    ap = argparse.ArgumentParser(description="Zip IOI 2010 maze outputs")
    ap.add_argument("--outputs-dir", type=Path, default=root / "maze_outputs")
    ap.add_argument("--out", type=Path, default=root / "maze_outputs.zip")
    ap.add_argument("--inputs-dir", type=Path, default=root / "g_inputs" / "tests")
    args = ap.parse_args()

    pack(args.outputs_dir, args.out)

    rows = grade_directory(args.inputs_dir, args.outputs_dir)
    total = sum(r.points for r in rows)
    print(f"graded total: {total:.4f} / {11 * len(rows)}")


if __name__ == "__main__":
    main()
