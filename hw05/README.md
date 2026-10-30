# ECE361 - Homework 5

## Combination Lock

This project implements a combination lock using a finite state machine (FSM).

The combination is 3-1-4.

## States

- LOCKED: The lock is waiting for the first digit.
- GOT1: The first digit (3) was entered correctly.
- GOT2: The first two digits (3 and 1) were entered correctly.
- OPEN: The correct combination was entered.

## Events

- E_RIGHT: Correct digit.
- E_WRONG: Incorrect digit.
- E_CLOSE: Door closes.
- E_TIMEOUT: Timer expires.

## Actions

- A_NONE: No action.
- A_UNLOCK: Unlock the door.
- A_LOCK: Lock the door.
- A_BEEP: Produce a beep.

## Build

Run:

    make

## Tests

Run:

    make test

## Run

Run:

    ./lock

The program reads characters from standard input.

Example:

    echo -n "314c" | ./lock

## Files

- lock.h: State, event, and action definitions.
- lock.c: Transition table and FSM functions.
- main.c: Main program.
- tests/test_lock.c: Automated tests.
- data/codes.txt: Example input sequences.

## Transition Table

| Current State | E_RIGHT | E_WRONG | E_CLOSE | E_TIMEOUT |
|---|---|---|---|---|
| LOCKED | GOT1 / NONE | LOCKED / BEEP | LOCKED / NONE | LOCKED / NONE |
| GOT1 | GOT2 / NONE | LOCKED / BEEP | GOT1 / NONE | LOCKED / NONE |
| GOT2 | OPEN / UNLOCK | LOCKED / BEEP | GOT2 / NONE | LOCKED / NONE |
| OPEN | OPEN / NONE | OPEN / NONE | LOCKED / LOCK | LOCKED / LOCK |
