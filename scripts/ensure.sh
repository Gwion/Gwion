#!/bin/sh
[ -d src ] || sh scripts/mdr2mdbook.sh

[ -f src/SUMMARY.md ] || sh scripts/summary.sh > src/SUMMARY.md

[ -d src/assets ] || cp -r benchmark/results src/assets
