#!/bin/sh

set -u

passed=0
failed=0
overall_status=0

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT HUP INT TERM

tests="
tests/test_status
tests/test_fsm
tests/test_store
tests/test_aux
tests/test_sensor
tests/test_report
"

for test in $tests; do
    echo "Running $test..."

    if "$test" > "$tmp/output" 2>&1; then
        rc=0
    else
        rc=$?
    fi

    cat "$tmp/output"

    summary=$(grep -E '[0-9]+ passed, [0-9]+ failed' "$tmp/output" | tail -n 1 || true)

    if [ -n "$summary" ]; then
        p=$(printf '%s\n' "$summary" | awk '{for(i=1;i<=NF;i++) if ($i=="passed,") print $(i-1)}')
        f=$(printf '%s\n' "$summary" | awk '{for(i=1;i<=NF;i++) if ($i=="failed") print $(i-1)}')

        passed=$((passed + p))
        failed=$((failed + f))
    elif [ "$rc" -ne 0 ]; then
        failed=$((failed + 1))
    fi

    if [ "$rc" -ne 0 ]; then
        overall_status=1
    fi
done

echo "Running scenario tests..."

if ./tests/test_scenarios.sh > "$tmp/scenarios" 2>&1; then
    rc=0
else
    rc=$?
fi

cat "$tmp/scenarios"

summary=$(grep -E '[0-9]+ passed, [0-9]+ failed' "$tmp/scenarios" | tail -n 1 || true)

if [ -n "$summary" ]; then
      p=$(printf '%s\n' "$summary" | awk '{for(i=1;i<=NF;i++) if ($i=="passed,") print $(i-1)}')
      f=$(printf '%s\n' "$summary" | awk '{for(i=1;i<=NF;i++) if ($i=="failed") print $(i-1)}')
    passed=$((passed + p))
    failed=$((failed + f))
elif [ "$rc" -ne 0 ]; then
    failed=$((failed + 1))
fi

if [ "$rc" -ne 0 ]; then
    overall_status=1
fi

echo "--------------------------------"
echo "TOTAL: $passed passed, $failed failed"

[ "$failed" -eq 0 ] && [ "$overall_status" -eq 0 ]
