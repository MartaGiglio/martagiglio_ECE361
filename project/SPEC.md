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

- Temperature is between 18.0 and 30.0 °C. **Decision:** both limits
  are included (18.0 and 30.0 give SPRAY_OK on). 
- Relative humidity is below 70%. **Decision:** strictly below
  (70.0 %RH gives SPRAY_OK off).

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

## 7. Interfaces (week 3)

### reading_t and sensor_read

`reading_t` holds the timestamp (`time_t`), the temperature in °C
(`temp_c`) and the humidity in %RH (`rh_pct`), both as `float`.

`int sensor_read(uint32_t *base, time_t timestamp, reading_t *reading)`
reads TEMP_REG and HUMID_REG, converts them with the AHT20 formulas and
rounds to 0.1 °C and 0.1 %RH. Return value: 0 = success, -1 = NULL
argument, otherwise the `rc` code reported by iom361 (1, 2, 3 or 5).
The module does not print.

### record_t

`record_t` holds three things: the `reading_t`, the FSM state at that
moment (`int`, numbered as in section 3 below), and the status word
(`uint32_t`).

`record_compare` orders records by `reading.timestamp` only (key K1).
It returns a negative value, zero, or a positive value.

### State numbering

IDLE = 0, PURGE = 1, SPRAY = 2, FLASH_OFF = 3, CURE = 4.

### Status word bit layout

| Bits | Field |
|---|---|
| 0-2 | state (0..4) |
| 3 | unused (0) |
| 4 | EXHAUST_FAN |
| 5 | HEATER |
| 6 | SPRAY_OK |
| 7 | spray conditions met |
| 8-31 | unused (0) |

Example: PURGE with the fan on = 0x00000011. Packing and unpacking use
`get_field` and `set_field` from HW1 (copied into `bits.c`).
