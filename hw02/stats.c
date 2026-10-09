#include "stats.h"

float stats_min(const float a[], int n)
{
    float min = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] < min)
            min = a[i];
    }

    return min;
}

float stats_max(const float a[], int n)
{
    if (n == 1)
        return a[0];

    float rest = stats_max(a + 1, n - 1);
    return a[0] > rest ? a[0] : rest;
}

float stats_mean(const float a[], int n)
{
    float sum = 0.0f;

    for (int i = 0; i < n; i++)
        sum += a[i];

    return sum / n;
}

int stats_longest_run_above(const float temps[], const int ticks[],
                            int n, float threshold,
                            int *start_tick, int *end_tick)
{
    int run = 0;
    int best = 0;
    int best_start = -1;
    int start = 0;

    for (int i = 0; i < n; i++) {
        if (temps[i] > threshold) {
            if (run == 0)
                start = i;

            run++;

            if (run > best) {
                best = run;
                best_start = start;
            }
        } else {
            run = 0;
        }
    }

    if (best > 0) {
        *start_tick = ticks[best_start];
        *end_tick = ticks[best_start + best - 1];
    }

    return best;
}
