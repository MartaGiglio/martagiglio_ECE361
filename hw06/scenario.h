#ifndef SCENARIO_H
#define SCENARIO_H

#include <stdint.h>
#include <time.h>

#define SCENARIO_MAX_LINES 1000

typedef struct {
    uint32_t tick;
    float temp_c;
    float rh_pct;
    uint32_t switches;
} scenario_line_t;

typedef struct {
    time_t start;
    int count;
    scenario_line_t lines[SCENARIO_MAX_LINES];
} scenario_t;

int scenario_load(const char *filename, scenario_t *s);

const scenario_line_t *scenario_at(const scenario_t *s, uint32_t tick);

#endif
