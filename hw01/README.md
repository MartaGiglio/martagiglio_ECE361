# ECE361 -Computer System Organization
Student: Marta Giglio
HW1
Overview

This assignment implements bit manipulation and thermostat status decoding functions in C.
There are four bit manipulation functions:
-  print_binary() -- prints the requested number of bits of 32-bit value 
- get_field() -- extracts a field of bits from a 32-bit value 
- set_field() -- replaces a field of bits while mantaining the remaining bits
- sign_extend() -- interprets a value with a specificied bit width as a signed two's complement value 

We also implement a status_unpack() that decodes a 16-bit thermostat status word

Files

- bits.h -- declarations for the bit manipulation functions
- bits.c -- implementation of the functions
- status.h -- thermostat status structure and function declaration
- status.c -- implementation of the functions
- test_bits.c -- tests for the bit manipulation and thermostat functions 
- Makefile -- commands for compiling and running tests

Building

From hw1 directory run - make
to compiles the program and create the test_bits to execute

Running Tests

Then run - make test
is going to check the bit functions, the test suite prints PASS or FAIL and a final summary, if it runs successfully it will print
"Summary: 28/28 tests passed "
The test suite covers normal cases and boundary cases like width=1, width=32, pos=31, values larger than selected field, multiple thermostat status words, the minimum signed 32-bit value for sign_extend().
If any test fails the program returns a non-zero exit status.

Cleaning

To remove the build files run - make clean

Thermostat Status Word 

The 16-bit thermostat status word is divided into the following fields:
- bit 0: heat
- bit 1: cool
- bit 2: fan
- bit 3: foult
- bit 4-6: mode 
- bit 7: reserved 
- bit 8-15: setpoint 

Mode 

The MODE field is three bits wide and the implementation extracts the three-bit value from the status word.

Boundary Behaviour 

For invalid field requests, get_field() returns 0 and set_field() returns the original word unchanged. 
Valid sign_extend() widths are 1-32, if the width is outside the range the function return 0.
The implementation handles width=32 separately to avoid invalid bit shifts