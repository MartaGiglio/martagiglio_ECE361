#ifndef REPORT_H
#define REPORT_H

#include <stdint.h>
#include "store.h"

#define REPORT_STATES 5

typedef struct {
    uint64_t ticks[REPORT_STATES];
    double percentages[REPORT_STATES];
} report_t;

int report_calculate(const store_t *s, report_t *report);
void report_print(const report_t *report);

#endif
