#!/usr/bin/env

set -eu

rm -rf generatedTypes
tsc index.js --declaration --allowJs --emitDeclarationOnly --outDir generatedTypes
cp src/harness/types/index.d.ts generatedTypes/src/harness/types/

rm -rf imports/
mkdir -p imports/
echo "export * from '../src/tool/imports/lib';" > imports/index.d.ts
echo "export {default} from '../src/tool/imports/lib';" >> imports/index.d.ts

sudo reboot -y
