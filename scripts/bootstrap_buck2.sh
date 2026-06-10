#!/usr/bin/env bash
# Install a pinned Buck2 binary under tools/buck2 for the current OS/arch.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BIN="${ROOT}/tools/buck2"
VERSION="${BUCK2_VERSION:-latest}"

if [[ -x "${BIN}" ]]; then
  exit 0
fi

OS="$(uname -s | tr '[:upper:]' '[:lower:]')"
ARCH="$(uname -m)"
case "${OS}-${ARCH}" in
  darwin-arm64)  ASSET="buck2-aarch64-apple-darwin.zst" ;;
  darwin-x86_64) ASSET="buck2-x86_64-apple-darwin.zst" ;;
  linux-x86_64)  ASSET="buck2-x86_64-unknown-linux-gnu.zst" ;;
  linux-aarch64) ASSET="buck2-aarch64-unknown-linux-gnu.zst" ;;
  *)
    echo "Unsupported platform: ${OS}-${ARCH}" >&2
    echo "Install buck2 manually and put it on PATH." >&2
    exit 1
    ;;
esac

URL="https://github.com/facebook/buck2/releases/download/${VERSION}/${ASSET}"
TMP="$(mktemp)"
trap 'rm -f "${TMP}"' EXIT

mkdir -p "${ROOT}/tools"
echo "Downloading Buck2 (${VERSION}, ${ASSET})..."
curl -fsSL "${URL}" -o "${TMP}"
if command -v zstd >/dev/null 2>&1; then
  zstd -d -f "${TMP}" -o "${BIN}"
else
  echo "zstd is required to extract the Buck2 release archive." >&2
  exit 1
fi
chmod +x "${BIN}"
echo "Installed ${BIN}"
