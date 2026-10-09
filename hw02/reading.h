#ifndef READING_H
#define READING_H

#define MAX_READINGS 1000

/*
 * Read sensor readings from standard input.
 * ticks, temps, and hums must each have room for capacity values.
 * skipped must point to a valid integer.
 * Returns the number of readings successfully parsed.
 */
int reading_read(int ticks[], float temps[], float hums[],
                 int capacity, int *skipped);

#endif
