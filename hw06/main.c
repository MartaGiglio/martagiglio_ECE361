#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "iom361_r4.h"
#include "sensor.h"
#include "panel.h"
#include "scenario.h"

int main(int argc, char *argv[])
{
    /* Check command-line arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s scenario.txt\n", argv[0]);
        return 1;
    }

    /* Load scenario */
    scenario_t scenario;

    int rc = scenario_load(argv[1], &scenario);

    if (rc != 0) {
        fprintf(stderr, "Error in scenario file at line %d\n", rc);
        return 1;
    }

    /* Initialize I/O */
    uint32_t *base = iom361_initialize(4, 4, &rc);

    if (rc != 0 || base == NULL) {
        fprintf(stderr, "I/O initialization failed\n");
        return 1;
    }

    iom361_set_display(false);

    /* Initialize previous switch state */
    uint32_t previous_switches = 0;

    /* Get the last tick */
    uint32_t last_tick = scenario.lines[scenario.count - 1].tick;

    /* Replay scenario */
    for (uint32_t tick = 0; ; tick++) {

        /* Get the scenario line in force */
        const scenario_line_t *entry = scenario_at(&scenario, tick);

        if (entry == NULL) {
            fprintf(stderr, "No scenario data for tick %u\n", tick);
            return 1;
        }

        /* Set the simulated world */
        _iom361_set_sensor1(entry->temp_c, entry->rh_pct);
        _iom361_set_switches(entry->switches);

        /* Set current time */
        time_t now = scenario.start + (time_t)tick;

        /* Read sensor */
        reading_t reading;

        rc = sensor_read(base, now, &reading);

        if (rc != 0) {
            fprintf(stderr, "Sensor read failed: %d\n", rc);
            return 1;
        }

        /* Read switches */
        uint32_t switches = iom361_readReg(base, SWITCHES_REG, &rc);

        if (rc != 0) {
            fprintf(stderr, "Switch read failed: %d\n", rc);
            return 1;
        }

        /* Detect newly pressed switches */
        uint32_t pressed = panel_pressed(switches, previous_switches);

        /* Toggle corresponding LEDs */
        for (int k = 0; k < 4; k++) {
            if (pressed & (1u << k)) {
                rc = panel_toggle(base, k);

                if (rc != 0) {
                    fprintf(stderr, "LED toggle failed: %d\n", rc);
                    return 1;
                }
            }
        }

        /* Read LED state */
        uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);

        if (rc != 0) {
            fprintf(stderr, "LED read failed: %d\n", rc);
            return 1;
        }

        /* Convert time to UTC */
        struct tm *utc = gmtime(&reading.t);
        char timestamp[20];

        if (utc == NULL ||
            strftime(timestamp, sizeof(timestamp),
                     "%Y-%m-%d %H:%M:%S", utc) == 0) {
            fprintf(stderr, "Time conversion failed\n");
            return 1;
        }

        /* Print trace */
        printf("%s temp=%.1f rh=%.1f switches=0x%X "
               "pressed=0x%X leds=0x%X\n",
               timestamp,
               reading.temp_c,
               reading.rh_pct,
               (unsigned int)switches,
               (unsigned int)pressed,
               (unsigned int)leds);

        /* Save current switch state for next tick */
        previous_switches = switches;

        /* Stop after processing the last tick */
        if (tick == last_tick) {
            break;
        }
    }

    return 0;
}
