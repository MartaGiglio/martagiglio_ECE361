# AI Usage — ECE 361 HW2

## Tools used

ChatGPT was used to help interpret the assignment requirements, plan the
module interfaces, write and organize C code, and troubleshoot compilation
and testing.

## Verification

I compiled the program using the provided Makefile with
`-std=c11 -Wall -Wextra`.

I ran `make test`. All 9 unit-test checks passed, and all 4 regression
tests passed.

## Issue encountered and resolution

The initial link step failed because the statistics functions were not
available in the compiled object file. I inspected the source and rewrote
`stats.c`, then rebuilt the program. The build succeeded, and the unit and
regression tests passed afterward.

## My responsibility

I ran the commands, reviewed the terminal output, and checked the results.
I am responsible for understanding, verifying, and submitting the final code.
