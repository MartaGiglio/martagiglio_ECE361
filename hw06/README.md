# ECE361 HW6 - Sensor and Panel Scenario Replay

## Overview

This project implements a sensor interface, a panel interface, and a scenario replay program using the IOM361 I/O simulator.

The program reads a scenario file, simulates sensor and switch inputs, detects newly pressed switches, toggles the corresponding LEDs, and prints a timestamped trace.

## Files

- `sensor.c`, `sensor.h`: Sensor reading and conversion.
- `panel.c`, `panel.h`: Switch edge detection and LED control.
- `scenario.c`, `scenario.h`: Scenario file parsing and tick lookup.
- `main.c`: Scenario replay program.
- `iom361_r4.c`, `iom361_r4.h`: IOM361 simulator interface.
- `scenarios/`: Input scenario files.
- `expected/`: Expected output traces.
- `tests/test_hw06.c`: Unit tests.
- `regress.sh`: Regression test script.

## Build

Compile the program with:

```bash
make
