#!/bin/bash

passed=0
failed=0

for name in balanced deep wrong_type nothing_open still_open empty
do
    ./balance < "inputs/$name.txt" > "actual.txt"
    status=$?

    expected_status=0

    case "$name" in
        wrong_type|nothing_open|still_open)
            expected_status=1
            ;;
    esac

    if diff -q "expected/$name.txt" actual.txt > /dev/null &&
       [ "$status" -eq "$expected_status" ]; then
        echo "PASS: $name"
        passed=$((passed + 1))
    else
        echo "FAIL: $name"
        failed=$((failed + 1))
    fi
done

rm -f actual.txt

echo
echo "Tests passed: $passed"
echo "Tests failed: $failed"

if [ "$failed" -ne 0 ]; then
    exit 1
fi

exit 0
