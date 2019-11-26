#!/bin/sh
[ -d md ] || sh scripts/mdr2mdbook.sh

[ -f md/SUMMARY.md ] || sh scripts/summary.sh > md/SUMMARY.md

[ -d md/assets ] || cp -r benchmark/results md/assets
