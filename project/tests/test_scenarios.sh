#!/bin/sh

set -u

passed=0
failed=0

tmp_dir=$(mktemp -d)
trap 'rm -rf "$tmp_dir"' EXIT HUP INT TERM

check_scenario() {
    name=$1
    scenario=$2
    expected=$3

    if ./booth "$scenario" > "$tmp_dir/actual"; then
        if diff -u "$expected" "$tmp_dir/actual"; then
            echo "PASS: $name"
            passed=$((passed + 1))
        else
            echo "FAIL: $name"
            failed=$((failed + 1))
        fi
    else
        echo "FAIL: $name (program execution failed)"
        failed=$((failed + 1))
    fi
}

check_scenario "normal operation" \
    scenarios/normal.txt expected/normal.txt

check_scenario "wrap-around" \
    scenarios/wraparound.txt expected/wraparound.txt

check_scenario "thresholds" \
    scenarios/thresholds.txt expected/thresholds.txt

check_scenario "Rule 3" \
    scenarios/spray_conditions_change.txt expected/spray_conditions_change.txt

check_scenario "unexpected inputs" \
    scenarios/unexpected_inputs.txt expected/unexpected_inputs.txt

check_scenario "midnight" \
    scenarios/midnight.txt expected/midnight.txt

if ./booth scenarios/report.txt --report > "$tmp_dir/report"; then
    if diff -u expected/report.txt "$tmp_dir/report"; then
        echo "PASS: report scenario"
        passed=$((passed + 1))
    else
        echo "FAIL: report scenario"
        failed=$((failed + 1))
    fi
else
    echo "FAIL: report scenario (program execution failed)"
    failed=$((failed + 1))
fi

echo "$passed passed, $failed failed"

[ "$failed" -eq 0 ]
