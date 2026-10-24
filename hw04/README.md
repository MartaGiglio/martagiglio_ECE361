# ECE361 Homework 4 - Tally ADT

## Description

This project implements a Tally Abstract Data Type (ADT) in C.

The tally stores names and counts how many times each name is added. Names are stored in alphabetical order. The implementation supports up to 16 distinct names per tally and up to 4 tally instances at the same time.

The project includes:
- `tally.h`: The public interface for the Tally ADT.
- `tally.c`: The implementation of the Tally ADT.
- `events.c`: Reads input lines, counts names, and prints the results.
- `tests/test_tally.c`: Tests the Tally ADT functions.
- `Makefile`: Builds the program and runs the tests.

## Compilation

To compile the program, run:

    make

This builds the `events` executable.

To remove generated files, run:

    make clean

## Running the Program

The program reads input from standard input. Each valid line is treated as a name. Blank lines and lines beginning with `#` are skipped.

Example command:

    printf "banana\napple\nbanana\n# comment\n\napple\n" | ./events

Expected output:

    apple 2
    banana 2
    names: 2, lines skipped: 2

Names are printed in alphabetical order, followed by the number of distinct names and the number of skipped lines.

## Running the Tests

To compile and run the tests, execute:

    make test

The tests check tally creation and destruction, adding names, counting occurrences, handling invalid arguments, capacity limits, independent tally instances, and alphabetical output.

The test program prints a PASS or FAIL result for each test and returns a nonzero exit status if any test fails.

## Opaque Type

The type `tally_t` is declared in `tally.h` as:

    typedef struct tally tally_t;

The structure's fields are defined only in `tally.c`. Therefore, code that includes only `tally.h` cannot access the internal fields of `tally_t`.

Code using the public interface can create, modify, query, print, and destroy a tally without knowing how its internal data is stored.

This hides implementation details and allows the internal representation to change without requiring changes to code that uses the interface.


### Compiler Error Demonstration

To test what the opaque type prevents, I created a temporary file named opaque_test.c that included only tally.h and attempted to access the field t->n.

The compiler produced this error:

       opaque_test.c: In function ‘main’:
       opaque_test.c:6:14: error: invalid use of incomplete typedef ‘tally_t’
            6 |     int n = t->n;
              |              ^~

This error occurs because tally.h declares tally_t but does not define its internal fields. The complete structure is defined only in tally.c. Therefore, code that includes only tally.h can use the public functions but cannot directly access the internal data. This is useful because it hides implementation details and allows the implementation to change without affecting code that uses the public interface.
