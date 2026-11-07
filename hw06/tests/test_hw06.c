#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include "iom361_r4.h"
#include "sensor.h"
#include "panel.h"
#include "scenario.h"

static int tests_passed = 0;
static int tests_failed = 0;

static void check(const char *name, int condition)
{
    if (condition) {
        printf("PASS: %s\n", name);
        tests_passed++;
    } else {
        printf("FAIL: %s\n", name);
        tests_failed++;
    }
}

/* Write a scenario file for testing */
static int write_scenario(const char *filename, const char *content)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        return 0;
    }

    fputs(content, file);
    fclose(file);

    return 1;
}

int main(void)
{
    int rc;
    uint32_t *base = iom361_initialize(4, 4, &rc);

    if (rc != 0 || base == NULL) {
        fprintf(stderr, "I/O initialization failed\n");
        return 1;
    }

    iom361_set_display(false);

    /* --------------------------------
       SENSOR TESTS
       -------------------------------- */

    reading_t reading = {0};

    _iom361_set_sensor1(91.0f, 40.0f);

    rc = sensor_read(base, 100, &reading);

    check("Sensor reads 91.0 C correctly",
          rc == 0 &&
          reading.temp_c == 91.0f &&
          reading.rh_pct == 40.0f &&
          reading.t == 100);

    reading_t previous = {123, 12.3f, 45.6f};

/* Initialize reading with known values */
reading = previous;

rc = sensor_read(NULL, 200, &reading);

check("Failed sensor read changes nothing",
      rc != 0 &&
      reading.t == previous.t &&
      reading.temp_c == previous.temp_c &&
      reading.rh_pct == previous.rh_pct);

    /* --------------------------------
       PANEL EDGE DETECTION TESTS
       -------------------------------- */

    check("Pressed detects a new press",
          panel_pressed(0x1, 0x0) == 0x1);

    check("Released detects a release",
          panel_released(0x0, 0x1) == 0x1);

    check("Held switch is not pressed again",
          panel_pressed(0x1, 0x1) == 0x0);

    /* --------------------------------
       PANEL LED TESTS
       -------------------------------- */

    panel_led(base, 0, 0);
    panel_led(base, 1, 0);
    panel_led(base, 2, 0);

    rc = panel_led(base, 1, 1);
    rc |= panel_led(base, 0, 1);

    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);

    check("panel_led preserves other LEDs",
          rc == 0 && leds == 0x3);

    rc = panel_led(base, 0, 0);
    leds = iom361_readReg(base, LEDS_REG, &rc);

    check("panel_led turns off only selected LED",
          rc == 0 && leds == 0x2);

    rc = panel_toggle(base, 2);
    leds = iom361_readReg(base, LEDS_REG, &rc);

    check("panel_toggle turns LED on",
          rc == 0 && leds == 0x6);

    rc = panel_toggle(base, 2);
    leds = iom361_readReg(base, LEDS_REG, &rc);

    check("panel_toggle turns LED off",
          rc == 0 && leds == 0x2);

    check("panel_led rejects invalid LED number",
          panel_led(base, 32, 1) == -1);

    /* --------------------------------
       SCENARIO TESTS
       -------------------------------- */

    scenario_t scenario;

    const char *valid_file = "tests/scenario_valid.tmp";

    int written = write_scenario(
        valid_file,
        "# Example scenario\n"
        "\n"
        "start 1000\n"
        "\n"
        "0 21.5 40.0 0\n"
        "# Another comment\n"
        "3 23.0 45.0 1\n"
    );

    rc = written ? scenario_load(valid_file, &scenario) : 1;

    check("Scenario loads comments and blank lines",
          rc == 0);

    if (rc == 0) {
        const scenario_line_t *first = scenario_at(&scenario, 0);
        const scenario_line_t *middle = scenario_at(&scenario, 2);
        const scenario_line_t *last = scenario_at(&scenario, 4);

        check("Scenario values hold between lines",
              first != NULL &&
              middle != NULL &&
              last != NULL &&
              middle->temp_c == 21.5f &&
              last->temp_c == 23.0f &&
              last->switches == 1);
    } else {
        check("Scenario values hold between lines", 0);
    }

    /* Scenario without start */
    written = write_scenario(
        "tests/scenario_no_start.tmp",
        "0 21.5 40.0 0\n"
    );

    rc = written
        ? scenario_load("tests/scenario_no_start.tmp", &scenario)
        : 0;

    check("Scenario without start is rejected",
          rc != 0);

    /* Scenario with a missing field */
    written = write_scenario(
        "tests/scenario_missing_field.tmp",
        "start 1000\n"
        "0 21.5 40.0\n"
    );

    rc = written
        ? scenario_load("tests/scenario_missing_field.tmp", &scenario)
        : 0;

    check("Scenario with missing field is rejected",
          rc != 0);

    /* Scenario with non-increasing ticks */
    written = write_scenario(
        "tests/scenario_bad_ticks.tmp",
        "start 1000\n"
        "0 21.5 40.0 0\n"
        "0 23.0 45.0 1\n"
    );

    rc = written
        ? scenario_load("tests/scenario_bad_ticks.tmp", &scenario)
        : 0;

    check("Scenario with non-increasing ticks is rejected",
          rc != 0);

    /* Remove temporary scenario files */
    remove("tests/scenario_valid.tmp");
    remove("tests/scenario_no_start.tmp");
    remove("tests/scenario_missing_field.tmp");
    remove("tests/scenario_bad_ticks.tmp");

    /* --------------------------------
       TEST SUMMARY
       -------------------------------- */

    printf("\nTests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
