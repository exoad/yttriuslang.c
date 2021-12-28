#!/usr/bin/env
# Licensed
set -eu

rm -rf generatedTypes
tsc index.js --declaration --allowJs --emitDeclarationOnly --outDir generatedTypes
cp src/harness/types/index.d.ts generatedTypes/src/harness/types/

rm -rf imports/
mkdir -p imports/
