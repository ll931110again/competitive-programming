#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$root"

files=()
while IFS= read -r -d '' file; do
  files+=("$file")
done < <(
  find . \
    \( -path './third_party/*' -o -path './vendor/*' -o -path './.git/*' \) -prune \
    -o \( -name '*.cc' -o -name '*.cpp' -o -name '*.hh' -o -name '*.h' \) -print0
)

clang-format -i "${files[@]}"
echo "clang-format: ${#files[@]} files" >&2
