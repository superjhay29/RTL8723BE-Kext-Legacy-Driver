#!/bin/bash
set -euo pipefail
KEXT="$1"; STAGE=build/dmg
rm -rf "$STAGE"; mkdir -p "$STAGE"
cp -R "$KEXT" "$STAGE/"
cp README.md "$STAGE/README.txt"
hdiutil create -volname RTL8723BE -srcfolder "$STAGE" -ov -format UDZO build/RTL8723BE.dmg
