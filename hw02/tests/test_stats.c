#include <stdio.h>
#include "../stats.h"

static int passed = 0;
static int failed = 0;

static void check(const char *name, int condition)
{
    if (condition) {
        printf("PASS  %s\n", name);
        passed++;
    } else {
        printf("FAIL  %s\n", name);
        failed++;
    }
}

int main(void)
{
    /* Test 1: array with one value */
    float one[] = {7.0f};
    check("one value: minimum", stats_min(one, 1) == 7.0f);
    check("one value: maximum", stats_max(one, 1) == 7.0f);
    check("one value: mean", stats_mean(one, 1) == 7.0f);

    /* Test 2: maximum at the beginning */
    float max_start[] = {9.0f, 2.0f, 4.0f};
    check("maximum at start", stats_max(max_start, 3) == 9.0f);

    /* Test 3: maximum at the end */
    float max_end[] = {2.0f, 4.0f, 9.0f};
    check("maximum at end", stats_max(max_end, 3) == 9.0f);

    /* Test 4: a value equal to threshold ends a run */
    float temps[] = {31.0f, 32.0f, 30.0f, 35.0f};
    int ticks[] = {10, 11, 12, 13};
    int start_tick = -1;
    int end_tick = -1;

    int run = stats_longest_run_above(
        temps, ticks, 4, 30.0f, &start_tick, &end_tick);

    check("threshold ends a run", run == 2);
    check("longest run start tick", start_tick == 10);
    check("longest run end tick", end_tick == 11);

    /* Test 5: recursive maximum on 10,000 values */
    float many[10000];

    for (int i = 0; i < 10000; i++)
        many[i] = (float)i;

    check("recursive maximum on 10000 values",
          stats_max(many, 10000) == 9999.0f);

    printf("\nSummary: %d passed, %d failed\n", passed, failed);

    return failed == 0 ? 0 : 1;
}
