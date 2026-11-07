#!/bin/bash

set -e

echo "Running regression tests..."

for scenario in scenarios/*.txt
do
    name=$(basename "$scenario" .txt)

    echo "Testing $name..."

    ./replay "$scenario" > "actual.tmp"

    if diff -u "expected/$name.txt" "actual.tmp"; then
        echo "PASS: $name"
    else
        echo "FAIL: $name"
        rm -f actual.tmp
        exit 1
    fi
done

rm -f actual.tmp

echo "All regression tests passed."
