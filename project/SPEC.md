# ECE 361 Fall 2026 — Project 24: Paint Booth

## 1. Project Overview

The Paint Booth is an embedded process sequencer that models an automotive paint booth. It reads temperature and humidity sensors, monitors two switches, controls three LEDs and an RGB LED, and records the system state over time.

The system follows five sequential states:

1. IDLE
2. PURGE
3. SPRAY
4. FLASH_OFF
5. CURE

The program is implemented in C using the supplied IOM361 hardware simulation library.

One tick represents one second.

## 2. Hardware Inputs and Outputs

### 2.1 Switches

The system uses two switches:

| Bit | Name | Type | Meaning |
|---|---|---|---|
| 0 | START | Press event | Starts a new paint cycle |
| 1 | SPRAY_DONE | Press event | Indicates that spraying is complete |

A press event is detected when a switch changes from 0 to 1.

The program uses the current and previous switch values to detect rising edges:

- Pressed: current = 1, previous = 0.
- Released: current = 0, previous = 1.

Keeping a switch on does not generate repeated events.

### 2.2 LEDs

| Bit | Name | Meaning |
|---|---|---|
| 0 | EXHAUST_FAN | Exhaust fan indicator |
| 1 | HEATER | Heater indicator |
| 2 | SPRAY_OK | Spraying conditions are acceptable |

The exhaust fan is active in PURGE and FLASH_OFF.

The heater is controlled during CURE.

SPRAY_OK is calculated from the current sensor readings.

### 2.3 RGB LED

The RGB LED indicates the current state.

| State | Color | RGB |
|---|---|---|
| IDLE | Blue | (0, 0, 255) |
| PURGE | Green | (0, 255, 0) |
| SPRAY | Red | (255, 0, 0) |
| FLASH_OFF | Orange | (255, 165, 0) |
| CURE | Yellow | (255, 255, 0) |

Each state has a distinct color.

## 3. Sensor Conditions

The sensor module reads the temperature and humidity registers.

The conversion formulas are:

Temperature:

T = (ST / 2^20) × 200 − 50

Relative humidity:

RH = (SRH / 2^20) × 100

The resulting values are rounded to one decimal place.

### 3.1 SPRAY_OK

SPRAY_OK is true when both conditions are satisfied:

- Temperature is between 18.0 °C and 30.0 °C, inclusive.
- Relative humidity is strictly below 70.0%.

Therefore:

- 18.0 °C: acceptable.
- 30.0 °C: acceptable.
- 70.0% RH: not acceptable.

### 3.2 Heater Hysteresis

During CURE, the heater uses two temperature thresholds:

- Below 58.0 °C: heater turns ON.
- At or above 60.0 °C: heater turns OFF.
- Between 58.0 °C and 60.0 °C: heater keeps its previous condition.

This is hysteresis. It prevents the heater from rapidly switching on and off near a single temperature threshold.

The heater is OFF outside CURE.

## 4. Finite State Machine

### 4.1 States

| State | Description |
|---|---|
| IDLE | Booth is inactive and waits for START |
| PURGE | Exhaust fan clears the booth air |
| SPRAY | Painter performs the spraying operation |
| FLASH_OFF | Solvent evaporates while the fan runs |
| CURE | Paint is heated for the curing phase |

The initial state is IDLE.

### 4.2 Events

| Event | Meaning |
|---|---|
| EVENT_NONE | No event |
| EVENT_START | Rising edge on START |
| EVENT_PURGE_READY | PURGE has lasted at least 60 ticks and SPRAY_OK is true |
| EVENT_SPRAY_DONE | Rising edge on SPRAY_DONE |
| EVENT_FLASH_DONE | FLASH_OFF has lasted 120 ticks |
| EVENT_CURE_DONE | CURE has lasted 300 ticks |

### 4.3 Transition Table

The FSM uses a two-dimensional transition table indexed by the current state and event.

Each entry contains the next state and an action.

| Current State | Event | Next State | Action |
|---|---|---|---|
| IDLE | EVENT_START | PURGE | ACTION_START_PURGE |
| PURGE | EVENT_PURGE_READY | SPRAY | ACTION_START_SPRAY |
| SPRAY | EVENT_SPRAY_DONE | FLASH_OFF | ACTION_START_FLASH_OFF |
| FLASH_OFF | EVENT_FLASH_DONE | CURE | ACTION_START_CURE |
| CURE | EVENT_CURE_DONE | IDLE | ACTION_END_CYCLE |

All other state/event combinations leave the current state unchanged and produce ACTION_NONE.

### 4.4 State Diagram

```text
                 START
        +----------------------+
        |                      v
     +------+              +-------+
     | IDLE |              | PURGE |
     +------+              +-------+
        ^                       |
        |                       | At least 60 ticks
        |                       | and SPRAY_OK
        |                       v
     +------+              +-------+
     | CURE |              | SPRAY |
     +------+              +-------+
        ^                       |
        |                       | SPRAY_DONE
        |                       v
        |                  +-----------+
        +------------------| FLASH_OFF |
           300 ticks       +-----------+
                                |
                                | 120 ticks
                                |
                                +-------> CURE
```

The normal sequence is:

IDLE → PURGE → SPRAY → FLASH_OFF → CURE → IDLE.

### 4.5 Decisions for Unspecified Situations

The assignment leaves some situations open to implementation decisions.

The following decisions are used:

1. SPRAY_DONE during PURGE is ignored. The painter cannot finish a spraying operation that has not started.
2. START pressed outside IDLE is ignored.
3. If SPRAY_OK becomes false during SPRAY, the system remains in SPRAY. The SPRAY_OK LED indicates the current sensor condition, but the FSM waits for SPRAY_DONE.
4. PURGE cannot end before 60 ticks, even if SPRAY_OK becomes true earlier.
5. If SPRAY_OK is false after 60 ticks, PURGE continues until the condition becomes true.
6. If the heater never reaches 60.0 °C, CURE still ends after 300 ticks. The duration is fixed by the operating rule.
7. Switches held ON do not generate repeated press events. They must be released and pressed again to generate another event.

These decisions keep the sequence deterministic and prevent unexpected switch activity from skipping phases.

## 5. Status Word

The system stores its status in one 32-bit unsigned integer (`uint32_t`).

The bit layout is:

| Bits | Field | Meaning |
|---|---|---|
| 0–2 | State | Current FSM state |
| 3 | Unused | Always 0 |
| 4 | EXHAUST_FAN | Fan indicator |
| 5 | HEATER | Heater indicator |
| 6 | SPRAY_OK | Spraying conditions acceptable |
| 7 | Conditions met | Sensor conditions satisfied |
| 8–31 | Unused | Always 0 |

The state values are:

- IDLE = 0
- PURGE = 1
- SPRAY = 2
- FLASH_OFF = 3
- CURE = 4

The status module uses the `get_field` and `set_field` functions from the earlier coursework to pack and unpack the fields.

Example:

PURGE with the exhaust fan ON:

State = 1
Fan = 1

Status word = 0x00000011

Unused bits must remain zero.

## 6. Data Structures

### 6.1 Record Type

Each stored record contains:

- A sensor reading.
- The FSM state at that timestamp.
- The packed status word.

The structure is:

```c
typedef struct {
    reading_t reading;
    int state;
    uint32_t status;
} record_t;
```

The timestamp is the key used to order records.

The `record_compare` function compares only the timestamps and returns a negative value, zero, or a positive value.

Duplicate timestamps are rejected.

### 6.2 Store: AVL Binary Search Tree

The store is an opaque abstract data type.

The header declares:

```c
typedef struct store store_t;
```

Only `store.c` knows the internal structure.

The implementation uses an AVL tree, a self-balancing binary search tree.

The tree is ordered by timestamp.

#### Why AVL?

Timestamps arrive in increasing order.

In a normal, unbalanced binary search tree, inserting increasing timestamps would create a structure similar to a linked list.

For N records:

- Search could require O(N) operations.
- Insertion could require O(N) operations.
- Tree height could become N.

The AVL tree automatically maintains balance using rotations.

Its operations have these expected worst-case complexities:

| Operation | Complexity |
|---|---|
| Find | O(log N) |
| Insert | O(log N) |
| In-order walk | O(N) |
| Count | O(1) |
| Height | O(1) |
| Destroy | O(N) |

The implementation supports:

- Creation.
- Insertion.
- Search by timestamp.
- In-order traversal with a callback.
- Record count.
- Tree height.
- Destruction.

### 6.3 Auxiliary Circular Buffer

The auxiliary module stores the most recent 32 sensor readings.

It uses a fixed-size array:

```c
#define AUX_CAPACITY 32
```

The structure maintains:

- `readings[32]`: the fixed array.
- `head`: index of the newest reading.
- `count`: number of valid entries.

When the buffer is empty, the first reading is stored at index 0.

For each new reading, the head advances:

```c
head = (head + 1) % AUX_CAPACITY;
```

When the buffer is full, the next insertion overwrites the oldest reading.

The `aux_get` function returns the i-th newest reading:

- Index 0: newest reading.
- Index 1: second newest.
- Index 31: oldest reading when full.

The main operations have the following complexity:

| Operation | Complexity |
|---|---|
| Create | O(1) |
| Add | O(1) |
| Get i-th newest | O(1) |
| Count | O(1) |
| Is full | O(1) |
| Destroy | O(1) |

#### Alternative: Linked List

A linked list could also store the most recent readings.

However, maintaining a fixed limit of 32 entries would require allocating and freeing nodes as readings arrive.

Finding an arbitrary reading would require O(N) traversal.

The circular buffer is more appropriate because:

- Its capacity is fixed.
- It does not require dynamic allocation for each reading.
- Insertion is O(1).
- Access by relative age is O(1).
- Memory usage is predictable.

## 7. Report Definition

The report calculates the total time spent in each of the five states.

It also calculates the percentage of total recorded time represented by each state.

Records are traversed in timestamp order.

For every pair of consecutive records:

1. Calculate the difference between their timestamps.
2. Add that duration to the state of the earlier record.

The total duration is the difference between the last and first timestamps.

The percentage is:

Percentage = (State duration / Total duration) × 100

Every state appears in the report, including states never entered. A state that was never entered has zero ticks and zero percent.

The last record does not add extra time because there is no later reading to establish another interval.

If there are fewer than two records, the total duration is zero and all state durations and percentages are zero.

The report module accesses records through the public `store.h` interface only.

## 8. Time Representation

The program uses `time_t` to represent Unix time: seconds since 1 January 1970, 00:00:00 UTC.

The scenario file provides the starting timestamp.

Each tick advances time by one second.

The timestamp for a reading is:

```c
timestamp = start_time + tick;
```

The program uses `gmtime()` and `strftime()` to format timestamps in UTC.

UTC ensures that traces are independent of the computer's local time zone.

### The Year 2038 Problem

If `time_t` is a signed 32-bit integer, its maximum value is 2,147,483,647.

This corresponds to 19 January 2038 at 03:14:07 UTC.

One second later, the value overflows.

On systems using signed 32-bit `time_t`, this can cause timestamps to wrap to a negative value and represent an incorrect date.

A 64-bit `time_t` avoids this problem for practical purposes.

The project uses the system's `time_t` type rather than assuming a fixed integer width.

## 9. Program Architecture

### main.c

The main program coordinates the system.

Its responsibilities include:

1. Parse the command-line arguments.
2. Read the scenario file.
3. Initialize IOM361.
4. Initialize the store and auxiliary buffer.
5. Advance the simulated clock.
6. Set the simulated sensor and switch values.
7. Read the sensors.
8. Detect switch press events.
9. Generate FSM events.
10. Execute the FSM transition.
11. Update LEDs and RGB LED.
12. Pack the status word.
13. Store the current record.
14. Update the auxiliary buffer.
15. Print the trace or the final report.

The program supports two modes:

```bash
./booth scenario.txt
```

Prints one trace line per tick.

```bash
./booth scenario.txt --report
```

Replays the scenario and prints only the report.

### Module Responsibilities

| Module | Responsibility |
|---|---|
| main.c | Coordinates the complete simulation |
| sensor.c | Reads and converts sensor values |
| fsm.c | Defines states, events, and transitions |
| store.c | Stores timestamped records in an AVL tree |
| aux.c | Maintains the circular buffer |
| report.c | Calculates state durations and percentages |
| status.c | Packs and unpacks the status word |
| bits.c | Provides bit-field operations |
| iom361_r4.c | Supplied hardware simulation |

The scenario replay in `main.c` controls the simulated environment using `_iom361_set_sensor1()` and `_iom361_set_switches()`.

The sensor and FSM modules do not directly modify the simulated environment.

## 10. Scenario Format and Trace

A scenario file contains:

1. A starting Unix timestamp.
2. A sequence of readings and switch values associated with ticks.

Lines beginning with `#` are comments.

The values remain unchanged between scenario entries.

Example:

```text
start 1791273600

# tick temperature humidity switches
0 22.0 50.0 0x0
5 22.0 50.0 0x1
6 22.0 50.0 0x0
60 23.0 50.0 0x0
```

The trace contains one line per simulated tick.

Example:

```text
2026-10-06 08:00:46 state=PURGE temp=22.0 rh=50.0 leds=0x1 status=0x00000011
```

The trace includes:

- UTC timestamp.
- Current state.
- Temperature.
- Relative humidity.
- LED values.
- Packed status word.

## 11. Design Evaluation: State Structure

The five states represent the physical phases of the paint booth in a clear sequence.

Keeping PURGE, SPRAY, FLASH_OFF, and CURE separate makes the timing and output behavior easier to understand and test.

The state machine also prevents the process from skipping required phases.

One possible alternative would be to combine some phases into a more general state with additional flags. However, this would make the transition logic more complicated and less readable.

The five-state design is therefore appropriate for this process sequencer because each phase has different timing requirements and hardware outputs.

## 12. Testing Strategy

The project uses unit tests and scenario tests.

Unit tests verify individual modules:

- Status packing and unpacking.
- FSM transitions.
- AVL tree insertion, searching, ordering, and balancing.
- Circular buffer behavior.
- Sensor conversion.
- Report calculations.

Scenario tests verify the complete system under realistic sequences of sensor readings and switch inputs.

The build uses:

```bash
gcc -std=c11 -Wall -Wextra
```

Unit tests are also compiled with AddressSanitizer and UndefinedBehaviorSanitizer.

The complete test suite is executed using:

```bash
make test
```

The project must compile without warnings and pass all tests.
