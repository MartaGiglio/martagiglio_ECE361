#include "report.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

static const char *state_names[REPORT_STATES] = {
    "IDLE",
    "PURGE",
    "SPRAY",
    "FLASH_OFF",
    "CURE"
};

struct report_context {
    report_t *report;
    time_t previous_timestamp;
    int previous_state;
    int has_previous;
};

/* Processes two consecutive records. */
static void process_record(const record_t *record, void *ctx) {
    struct report_context *context = ctx;

    if (context->has_previous) {
        double elapsed = difftime(record->reading.timestamp,
                                   context->previous_timestamp);

        if (elapsed > 0 &&
            context->previous_state >= 0 &&
            context->previous_state < REPORT_STATES) {
            context->report->ticks[context->previous_state]
                += (uint64_t)elapsed;
        }
    }

    context->previous_timestamp = record->reading.timestamp;
    context->previous_state = record->state;
    context->has_previous = 1;
}

/* Calculates total ticks and percentages for each state. */
int report_calculate(const store_t *s, report_t *report) {
    if (s == NULL || report == NULL) {
        return -1;
    }

    memset(report, 0, sizeof(*report));

    struct report_context context = {
        .report = report,
        .previous_timestamp = 0,
        .previous_state = 0,
        .has_previous = 0
    };

    store_walk(s, process_record, &context);

    uint64_t total = 0;

    for (int i = 0; i < REPORT_STATES; i++) {
        total += report->ticks[i];
    }

    if (total > 0) {
        for (int i = 0; i < REPORT_STATES; i++) {
            report->percentages[i] =
                100.0 * (double)report->ticks[i] / (double)total;
        }
    }

    return 0;
}

/* Prints the report. */
void report_print(const report_t *report) {
    if (report == NULL) {
        return;
    }

    printf("Paint Booth Report\n");
    printf("------------------\n");

    for (int i = 0; i < REPORT_STATES; i++) {
        printf("%-10s  %" PRIu64 " ticks  %.2f%%\n",
               state_names[i],
               report->ticks[i],
               report->percentages[i]);
    }
}
