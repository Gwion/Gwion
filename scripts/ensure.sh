#!/bin/sh

for file in $(find docs -type f -name "*.mdr")
do mdr $file
done

[ -d src ] || bash scripts/mdr2mdbook.sh

[ -f src/SUMMARY.md ] || bash scripts/summary.sh > src/SUMMARY.md

[ -d src/assets ] || cp -r benchmark/results src/assets
