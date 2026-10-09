# ECE 361 — HW2

## Overview

This project refactors the original sensor-reading program into separate
C modules while preserving its output.

## Build

Compile the program with:

    make

This creates the `readings` executable.

## Run

Read input from a file:

    ./readings < data/normal.txt

Use a custom temperature threshold:

    ./readings 25 < data/edge.txt

The default threshold is 30.0 degrees Celsius.

## Tests

Run the unit tests and regression tests:

    make test

Run only the regression tests:

    sh regress.sh

Remove generated files:

    make clean

## Module diagram

    main.c
      |-- reading.c / reading.h
      |     Reads and parses input readings.
      |
      |-- stats.c / stats.h
      |     Calculates minimum, maximum, mean, and longest run.
      |
      |-- histogram.c / histogram.h
            Prints the temperature histogram.

## Implementation notes

The reading arrays are allocated in `main.c` and passed to the
functions that need them. The maximum function is recursive and is
used for both temperature and humidity arrays. Each module exposes
its public functions through a header file.
