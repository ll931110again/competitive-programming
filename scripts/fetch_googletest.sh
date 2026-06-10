#!/usr/bin/env bash
# Fetch Google Test v1.15.2 into deps/googletest (same version as the old CMake build).
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DEST="${ROOT}/deps/googletest"
TAG="v1.15.2"
URL="https://github.com/google/googletest/archive/refs/tags/${TAG}.zip"

if [[ -f "${DEST}/googletest/include/gtest/gtest.h" ]]; then
  exit 0
fi

TMP="$(mktemp -d)"
trap 'rm -rf "${TMP}"' EXIT

echo "Fetching googletest ${TAG}..."
curl -fsSL "${URL}" -o "${TMP}/gtest.zip"
unzip -q "${TMP}/gtest.zip" -d "${TMP}"
rm -rf "${DEST}"
mv "${TMP}/googletest-1.15.2" "${DEST}"
echo "Installed googletest at ${DEST}"
