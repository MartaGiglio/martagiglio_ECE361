# Project 24 — Paint Booth

## 1. Project overview

The Paint Booth program models an automotive paint booth as a process
sequencer. It uses switch inputs, sensor readings, and LED outputs to
represent the stages of a painting cycle.

The implementation will be developed incrementally throughout the term.
This document describes the initial architecture and will be expanded as
the project progresses.

## 2. Inputs and outputs

### Switch inputs

| Bit | Name | Meaning |
|---|---|---|
| 0 | START | Requests the start of a painting cycle |
| 1 | SPRAY_DONE | Signals that spraying is complete |

Both switches are specified as press events.

### LED outputs

| Bit | Name | Meaning |
|---|---|---|
| 0 | EXHAUST_FAN | Exhaust fan indicator |
| 1 | HEATER | Heater indicator |
| 2 | SPRAY_OK | Indicates that spraying conditions are acceptable |

### Sensor conditions

SPRAY_OK is on when both conditions are satisfied:

- Temperature is between 18 and 30 degrees Celsius, inclusive.
- Relative humidity is below 70%.

During CURE, the heater turns on below 58 degrees Celsius and turns
off at 60 degrees Celsius. The behavior between these thresholds will
be specified as part of the implementation.

## 3. States

The process has five states:

1. IDLE
2. PURGE
3. SPRAY
4. FLASH_OFF
5. CURE

The initial state is IDLE.

The specified timing and transition requirements are:

- IDLE waits for START.
- PURGE lasts at least 60 ticks and cannot finish until SPRAY_OK is on.
- SPRAY waits for SPRAY_DONE.
- FLASH_OFF lasts 120 ticks.
- CURE lasts 300 ticks and then returns to IDLE.

One tick represents one second.

The complete transition table and event definitions will be added when
the FSM is implemented.

## 4. Module architecture

### main.c

Coordinates initialization, the clock, input acquisition, state-machine
execution, output updates, record storage, and program modes.

### sensor.c and sensor.h

Define the sensor-reading interface. Sensor readings include a timestamp,
temperature, and relative humidity.

### fsm.c and fsm.h

Define the finite-state machine, its states, events, and transition logic.

### store.c and store.h

Define the interface for storing and retrieving timestamped records.
The internal data structure will evolve during the term.

### aux.c and aux.h

Define the interface for retaining the most recent sensor readings in a
fixed-size circular buffer of 32 entries.

### report.c and report.h

Define the reporting functionality, including total ticks spent in each
state and the percentage of total ticks represented by each state.

### iom361_r4.c and iom361_r4.h

Provide the supplied IOM361 library used to interface with the simulated
switches, sensors, and LEDs. These supplied files will be preserved.

## 5. Development plan

The project will be developed incrementally according to the course plan:

- Week 2: module stubs, Makefile, ASSIGNMENT.md, and initial SPEC.md.
- Week 3: sensor and store interfaces, and status-word layout.
- Week 4: data types and initial store implementation.
- Week 5: FSM transition table and scenarios 1, 3, and 4.
- Week 6: IOM361 registers, clock, main program, trace, and scenarios 5 and 7.
- Week 7: circular buffer and scenario 2.
- Week 8: binary search tree implementation of the store.
- Week 9: report implementation, scenario 6, and remaining unit tests.
- Week 10: final testing, hardening, and completion of this specification.

## 6. Open design decisions

The following decisions must be documented and tested as the relevant
modules are implemented:

- Exact FSM event definitions and transition behavior for unspecified cases.
- How switch press events are detected.
- The status-word bit layout and unused-bit behavior.
- The RGB LED color associated with each state.
- The timestamp ordering and handling of duplicate timestamps.
- The precise definition of report totals and percentages.
- The behavior of the circular buffer when it is full.
- The representation of time and the implications of a 32-bit time_t.
