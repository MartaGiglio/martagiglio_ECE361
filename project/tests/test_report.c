#include <stdio.h>
#include <math.h>
#include "../report.h"
#include "../fsm.h"

static int passed = 0;
static int failed = 0;

#define CHECK(cond, name) do { \
    if (cond) passed++; \
    else { failed++; printf("FAIL: %s\n", name); } \
} while (0)

static record_t make_record(time_t timestamp, int state)
{
    record_t r = {0};
    r.reading.timestamp = timestamp;
    r.state = state;
    return r;
}

static void test_durations(void)
{
    store_t *s = store_create();
    if (s == NULL) {
        CHECK(0, "create store");
        return;
    }

    record_t records[] = {
        make_record(100, IDLE),
        make_record(110, PURGE),
        make_record(130, SPRAY)
    };

    for (size_t i = 0; i < 3; i++)
        CHECK(store_insert(s, &records[i]) == 0, "insert report record");

    report_t report;
    CHECK(report_calculate(s, &report) == 0, "calculate report");

    CHECK(report.ticks[IDLE] == 10, "IDLE duration");
    CHECK(report.ticks[PURGE] == 20, "PURGE duration");
    CHECK(report.ticks[SPRAY] == 0, "last state has no following interval");

    CHECK(fabs(report.percentages[IDLE] - 33.3333) < 0.01,
          "IDLE percentage");

    CHECK(fabs(report.percentages[PURGE] - 66.6667) < 0.01,
          "PURGE percentage");

    CHECK(report.ticks[FLASH_OFF] == 0, "empty state duration");
    CHECK(report.ticks[CURE] == 0, "empty state duration");

    store_destroy(s);
}

static void test_single_record(void)
{
    store_t *s = store_create();
    if (s == NULL) {
        CHECK(0, "create store for single record");
        return;
    }

    record_t r = make_record(100, IDLE);
    store_insert(s, &r);

    report_t report;
    CHECK(report_calculate(s, &report) == 0, "single record report");

    uint64_t total = 0;
    for (int i = 0; i < REPORT_STATES; i++) {
        total += report.ticks[i];
        CHECK(report.percentages[i] == 0.0,
              "single record percentage is zero");
    }

    CHECK(total == 0, "single record has zero duration");

    store_destroy(s);
}

static void test_null_arguments(void)
{
    report_t report;

    CHECK(report_calculate(NULL, &report) != 0, "reject NULL store");
    CHECK(report_calculate(NULL, NULL) != 0, "reject NULL arguments");
    report_print(NULL);
}

int main(void)
{
    test_durations();
    test_single_record();
    test_null_arguments();

    printf("REPORT: %d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
