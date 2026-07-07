#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
export AHC_CXX_STD=c++20
exec "$ROOT/scripts/ahc_calibrate.sh" ahc002 "$@"
