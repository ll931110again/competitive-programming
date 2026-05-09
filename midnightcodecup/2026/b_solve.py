#!/usr/bin/env python3
"""
Lost Cursor (Midnight Code Cup 2026 B): build a move sequence that guarantees
every starting WHITE cell hits a BLACK cell at least once (clamped grid moves).

Strategy:
  For corner C, sync moves send every cell to C (clamped axis moves).
  Append Manhattan moves from C to a chosen black cell B.

Try all 4 corners and every black cell as B; pick shortest verifying sequence.

PNG loading uses the standard library (zlib + struct). Optional: Pillow if installed.
"""

from __future__ import annotations

import argparse
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import zipfile
import zlib
from pathlib import Path


def _paeth(a: int, b: int, c: int) -> int:
    p = a + b - c
    pa = abs(p - a)
    pb = abs(p - b)
    pc = abs(p - c)
    if pa <= pb and pa <= pc:
        return a
    if pb <= pc:
        return b
    return c


def load_png_binary_stdlib(path: Path) -> tuple[list[list[bool]], int]:
    """Decode 8-bit PNG (grayscale or RGB), non-interlaced. Black=True."""
    data = path.read_bytes()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError("Not a PNG file")

    pos = 8
    idat_parts: list[bytes] = []
    width = height = 0
    bit_depth = color_type = interlace = 0

    while pos < len(data):
        length = struct.unpack_from(">I", data, pos)[0]
        ctype = data[pos + 4 : pos + 8]
        chunk = data[pos + 8 : pos + 8 + length]
        pos += 8 + length + 4
        if ctype == b"IHDR":
            width, height, bit_depth, color_type, _, _, interlace = struct.unpack(
                ">IIBBBBB", chunk
            )
        elif ctype == b"IDAT":
            idat_parts.append(chunk)
        elif ctype == b"IEND":
            break

    if interlace != 0:
        raise NotImplementedError("Interlaced PNG not supported")
    if bit_depth != 8:
        raise NotImplementedError(f"Bit depth {bit_depth} not supported")

    if color_type == 0:
        bpp = 1
    elif color_type == 2:
        bpp = 3
    elif color_type == 6:
        bpp = 4
    else:
        raise NotImplementedError(f"PNG color type {color_type} not supported")

    raw = zlib.decompress(b"".join(idat_parts))
    stride = width * bpp
    grid: list[list[bool]] = []
    idx = 0
    prev = bytearray(stride)

    for _ in range(height):
        ftype = raw[idx]
        idx += 1
        scan = bytearray(raw[idx : idx + stride])
        idx += stride

        if ftype == 0:
            pass
        elif ftype == 1:
            for x in range(stride):
                left = scan[x - bpp] if x >= bpp else 0
                scan[x] = (scan[x] + left) & 255
        elif ftype == 2:
            for x in range(stride):
                scan[x] = (scan[x] + prev[x]) & 255
        elif ftype == 3:
            for x in range(stride):
                left = scan[x - bpp] if x >= bpp else 0
                up = prev[x]
                scan[x] = (scan[x] + ((left + up) // 2)) & 255
        elif ftype == 4:
            for x in range(stride):
                left = scan[x - bpp] if x >= bpp else 0
                up = prev[x]
                ul = prev[x - bpp] if x >= bpp else 0
                scan[x] = (scan[x] + _paeth(left, up, ul)) & 255
        else:
            raise ValueError(f"Unknown PNG filter {ftype}")

        prev = scan
        row: list[bool] = []
        for col in range(width):
            if color_type == 0:
                v = scan[col]
            elif color_type == 2:
                o = col * 3
                v = (scan[o] + scan[o + 1] + scan[o + 2]) // 3
            else:
                o = col * 4
                v = (scan[o] + scan[o + 1] + scan[o + 2]) // 3
            row.append(v < 128)
        grid.append(row)

    n = width
    if height != n:
        raise ValueError(f"Expected square image, got {width}x{height}")
    return grid, n


def load_png_binary(path: Path) -> tuple[list[list[bool]], int]:
    """Black=True, White=False."""
    try:
        from PIL import Image

        img = Image.open(path).convert("L")
        w, h = img.size
        if w != h:
            raise ValueError(f"Expected square image, got {w}x{h}")
        n = w
        pix = img.load()
        grid: list[list[bool]] = []
        for i in range(n):
            row = []
            for j in range(n):
                v = pix[j, i]
                row.append(v < 128)
            grid.append(row)
        return grid, n
    except ImportError:
        return load_png_binary_stdlib(path)


def step(r: int, c: int, ch: str, n: int) -> tuple[int, int]:
    if ch == "U":
        return max(0, r - 1), c
    if ch == "D":
        return min(n - 1, r + 1), c
    if ch == "L":
        return r, max(0, c - 1)
    if ch == "R":
        return r, min(n - 1, c + 1)
    raise ValueError(ch)


def sync_to_corner(corner: str, n: int) -> str:
    k = n - 1
    if corner == "TL":
        return "U" * k + "L" * k
    if corner == "TR":
        return "U" * k + "R" * k
    if corner == "BL":
        return "D" * k + "L" * k
    if corner == "BR":
        return "D" * k + "R" * k
    raise ValueError(corner)


def manhattan_path(r: int, c: int, tr: int, tc: int, n: int) -> str:
    out = []
    while r != tr or c != tc:
        if r < tr:
            out.append("D")
            r, c = step(r, c, "D", n)
        elif r > tr:
            out.append("U")
            r, c = step(r, c, "U", n)
        elif c < tc:
            out.append("R")
            r, c = step(r, c, "R", n)
        else:
            out.append("L")
            r, c = step(r, c, "L", n)
    return "".join(out)


def corner_coords(corner: str, n: int) -> tuple[int, int]:
    if corner == "TL":
        return 0, 0
    if corner == "TR":
        return 0, n - 1
    if corner == "BL":
        return n - 1, 0
    if corner == "BR":
        return n - 1, n - 1
    raise ValueError(corner)


def _pack_rows(grid: list[list[bool]]) -> tuple[bytes, ...]:
    return tuple(bytes(1 if cell else 0 for cell in row) for row in grid)


def _verify_chunk(
    args: tuple[tuple[bytes, ...], bytes, int, list[tuple[int, int]]],
) -> bool:
    rows, moves_b, n, starts = args
    for r0, c0 in starts:
        r, c = r0, c0
        for ch in moves_b:
            if rows[r][c]:
                break
            if ch == 85:
                r = r - 1 if r > 0 else 0
            elif ch == 68:
                r = r + 1 if r < n - 1 else n - 1
            elif ch == 76:
                c = c - 1 if c > 0 else 0
            elif ch == 82:
                c = c + 1 if c < n - 1 else n - 1
            else:
                raise ValueError(ch)
        if not rows[r][c]:
            return False
    return True


def verify(grid: list[list[bool]], moves: str, n: int) -> bool:
    whites = [(i, j) for i in range(n) for j in range(n) if not grid[i][j]]
    rows = _pack_rows(grid)
    moves_b = moves.encode("ascii")
    return _verify_chunk((rows, moves_b, n, whites))


def build_candidate(grid: list[list[bool]], n: int, corner: str, br: int, bc: int) -> str:
    corner_r, corner_c = corner_coords(corner, n)
    sync = sync_to_corner(corner, n)
    suf = manhattan_path(corner_r, corner_c, br, bc, n)
    return sync + suf


def solve_grid(grid: list[list[bool]], n: int, max_len: int = 5000) -> str | None:
    blacks = [(i, j) for i in range(n) for j in range(n) if grid[i][j]]
    if not blacks:
        return None

    best: str | None = None
    for corner in ("TL", "TR", "BL", "BR"):
        cr, cc = corner_coords(corner, n)
        ranked = sorted(
            blacks, key=lambda t: abs(t[0] - cr) + abs(t[1] - cc)
        )
        for br, bc in ranked:
            cand = build_candidate(grid, n, corner, br, bc)
            if len(cand) > max_len:
                continue
            if not verify(grid, cand, n):
                continue
            if best is None or len(cand) < len(best) or (
                len(cand) == len(best) and cand < best
            ):
                best = cand
            break
    return best


def collect_png_paths(inputs: list[str]) -> list[tuple[str, Path]]:
    """Return list of (stem_for_out, path) e.g. ('01', Path(.../01.png))."""
    pairs: list[tuple[str, Path]] = []
    for raw in inputs:
        path = Path(raw)
        if path.is_dir():
            for png in sorted(path.glob("*.png")):
                m = re.match(r"^(\d+)\.png$", png.name, re.I)
                stem = m.group(1) if m else png.stem
                pairs.append((stem, png))
        elif path.suffix.lower() == ".zip":
            tmp = Path(tempfile.mkdtemp(prefix="lost_cursor_"))
            try:
                with zipfile.ZipFile(path, "r") as z:
                    z.extractall(tmp)
                for png in sorted(tmp.rglob("*.png")):
                    m = re.match(r"^(\d+)\.png$", png.name, re.I)
                    stem = m.group(1) if m else png.stem
                    pairs.append((stem, png))
            except Exception:
                shutil.rmtree(tmp, ignore_errors=True)
                raise
            # Keep tmp until process exit (files needed for PIL); leak is OK for CLI tool.
        else:
            m = re.match(r"^(\d+)\.png$", path.name, re.I)
            stem = m.group(1) if m else path.stem
            pairs.append((stem, path))
    def sort_key(p: tuple[str, Path]) -> tuple[int, str]:
        s, _ = p
        if s.isdigit():
            return (0, int(s))
        return (1, s)

    pairs.sort(key=sort_key)
    return pairs


def repo_root() -> Path:
    # midnightcodecup/2026/b_solve.py -> competitive-programming/
    return Path(__file__).resolve().parent.parent.parent


def ensure_cpp_runner() -> Path:
    """Compile b.cc to b_runner next to this script (uses repo bits/stdc++.h)."""
    here = Path(__file__).resolve().parent
    runner = here / "b_runner"
    src = here / "b.cc"
    if not src.is_file():
        raise FileNotFoundError(src)
    need = not runner.is_file()
    if not need:
        need = runner.stat().st_mtime < src.stat().st_mtime
    if need:
        subprocess.run(
            [
                "g++",
                "-std=c++17",
                "-O3",
                "-I",
                str(repo_root()),
                str(src),
                "-o",
                str(runner),
            ],
            check=True,
        )
    return runner


def grid_to_cpp_lines(grid: list[list[bool]]) -> str:
    return "\n".join(
        "".join("1" if cell else "0" for cell in row) for row in grid
    )


def solve_via_cpp(grid: list[list[bool]], runner: Path) -> str | None:
    proc = subprocess.run(
        [str(runner)],
        input=grid_to_cpp_lines(grid) + "\n",
        text=True,
        capture_output=True,
        check=False,
    )
    if proc.returncode != 0:
        sys.stderr.write(proc.stderr or "")
        return None
    s = proc.stdout.strip()
    return s if s else None


def main() -> None:
    ap = argparse.ArgumentParser(description="Solve Lost Cursor from PNG(s) or zip.")
    ap.add_argument("inputs", nargs="+", help="PNG file(s), directory of PNGs, or inputs.zip")
    ap.add_argument(
        "-o",
        "--out-dir",
        type=Path,
        default=Path("b_outputs"),
        help="Output directory for NN.out files",
    )
    args = ap.parse_args()

    out_dir: Path = args.out_dir
    out_dir.mkdir(parents=True, exist_ok=True)

    pairs = collect_png_paths(list(args.inputs))
    if not pairs:
        ap.print_help()
        sys.exit("No PNG inputs found.")

    runner = ensure_cpp_runner()

    def out_name(stem: str) -> str:
        if stem.isdigit():
            return f"{int(stem):02d}.out"
        return f"{stem}.out"

    for stem, png in pairs:
        grid, n = load_png_binary(png)
        ans = solve_via_cpp(grid, runner)
        if ans is None:
            print(f"{png}: NO SOLUTION FOUND", file=sys.stderr)
            continue
        out_file = out_dir / out_name(stem)
        out_file.write_text(ans.rstrip() + "\n")
        print(f"{png.name} -> {out_file.name} len={len(ans)} n={n}")


if __name__ == "__main__":
    main()
