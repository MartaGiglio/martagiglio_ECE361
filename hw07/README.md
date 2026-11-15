# ECE361 Homework 7

## Overview

This project implements a dynamically growing integer stack (istack ADT) and a bracket checker using the stack.

## Build

Compile the project:

```bash
make
```

## Tests

Run all tests:

```bash
make test
```

Run AddressSanitizer and UndefinedBehaviorSanitizer:

```bash
make memcheck
```

Remove generated executables and temporary files:

```bash
make clean
```

## Regression Tests

The regression tests are located in `inputs/` and their expected outputs are in `expected/`.

Run them with:

```bash
./regress.sh
```

The tests cover balanced brackets, nesting deeper than eight levels, mismatched brackets, closing brackets without an opening bracket, unclosed brackets, and empty input.

## Part 4: Memory Bug Experiments

Three memory errors were intentionally introduced and tested with sanitizers.

1. **Double free:** The same stack was destroyed twice. AddressSanitizer detected an attempt to free already-freed memory.
2. **Use after free:** A push operation was attempted after destroying the stack. AddressSanitizer detected access to freed memory.
3. **Memory leak:** The stack destruction was omitted from `balance.c`. LeakSanitizer detected memory that was not released.

All three bugs were subsequently corrected. The final implementation passes `make memcheck`.
