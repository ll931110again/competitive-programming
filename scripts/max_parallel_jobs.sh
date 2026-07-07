#!/usr/bin/env sh
# Print max parallel jobs: one per logical CPU core (fallback 4).
if command -v sysctl >/dev/null 2>&1; then
  sysctl -n hw.logicalcpu 2>/dev/null || echo 4
elif command -v nproc >/dev/null 2>&1; then
  nproc
else
  echo 4
fi
