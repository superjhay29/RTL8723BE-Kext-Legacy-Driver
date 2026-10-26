#!/bin/bash
# Package the built kext into build/RTL8723BE.dmg
set -euo pipefail
KEXT="$1"
STAGE=build/dmg
rm -rf "$STAGE"
mkdir -p "$STAGE"
cp -R "$KEXT" "$STAGE/"
if [ -f README.md ]; then cp README.md "$STAGE/README.txt"; fi
hdiutil create -volname RTL8723BE -srcfolder "$STAGE" -ov -format UDZO build/RTL8723BE.dmg
