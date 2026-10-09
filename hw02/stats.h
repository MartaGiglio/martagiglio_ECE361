#ifndef STATS_H
#define STATS_H

/*
 * Return the minimum value in a.
 * Assumes n >= 1.
 */
float stats_min(const float a[], int n);

/*
 * Return the maximum value in a using recursion.
 * Assumes n >= 1.
 */
float stats_max(const float a[], int n);

/*
 * Return the arithmetic mean of a.
 * Assumes n >= 1.
 */
float stats_mean(const float a[], int n);

/*
 * Return the longest consecutive run with values strictly above threshold.
 * If a run exists, store its first and last tick in start_tick and end_tick.
 * Assumes n >= 1 and that ticks has at least n elements.
 */
int stats_longest_run_above(const float temps[], const int ticks[],
                            int n, float threshold,
                            int *start_tick, int *end_tick);

#endif
