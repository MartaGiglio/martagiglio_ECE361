#include "scenario.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int scenario_load(const char *filename, scenario_t *s)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL || s == NULL) {
        if (file != NULL) {
            fclose(file);
        }
        return 1;
    }

    char line[256];
    int line_number = 0;
    int has_start = 0;

    s->count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;

        /* Remove comments */
        char *comment = strchr(line, '#');
        if (comment != NULL) {
            *comment = '\0';
        }

        /* Skip blank lines */
        char *p = line;
        while (isspace((unsigned char)*p)) {
            p++;
        }

        if (*p == '\0') {
            continue;
        }

        /* Read start timestamp */
        if (!has_start) {
            long long timestamp;
            int pos = 0;

            if (sscanf(p, "start %lld %n", &timestamp, &pos) != 1) {
                fclose(file);
                return line_number;
            }

            if (p[pos] != '\0') {
                fclose(file);
                return line_number;
            }

            s->start = (time_t)timestamp;
            has_start = 1;
            continue;
        }

        /* Check maximum number of lines */
        if (s->count >= SCENARIO_MAX_LINES) {
            fclose(file);
            return line_number;
        }

        /* Read tick, temperature, humidity and switches */
        unsigned long long tick;
        float temp;
        float rh;
        unsigned int switches;
        int pos = 0;

        if (sscanf(p, "%llu %f %f %x %n",
                   &tick, &temp, &rh, &switches, &pos) != 4) {
            fclose(file);
            return line_number;
        }

        if (p[pos] != '\0' || tick > UINT32_MAX) {
            fclose(file);
            return line_number;
        }

        /* Check increasing ticks */
        if (s->count > 0 &&
            tick <= s->lines[s->count - 1].tick) {
            fclose(file);
            return line_number;
        }

        scenario_line_t *entry = &s->lines[s->count];

        entry->tick = (uint32_t)tick;
        entry->temp_c = temp;
        entry->rh_pct = rh;
        entry->switches = (uint32_t)switches;

        s->count++;
    }

    fclose(file);

    if (!has_start || s->count == 0) {
        return line_number + 1;
    }

    return 0;
}

const scenario_line_t *scenario_at(const scenario_t *s, uint32_t tick)
{
    if (s == NULL || s->count == 0) {
        return NULL;
    }

    int index = -1;

    for (int i = 0; i < s->count; i++) {
        if (s->lines[i].tick > tick) {
            break;
        }

        index = i;
    }

    if (index == -1) {
        return NULL;
    }

    return &s->lines[index];
}
