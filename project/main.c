#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>

#include "iom361_r4.h"
#include "sensor.h"
#include "fsm.h"
#include "store.h"
#include "aux.h"
#include "report.h"
#include "status.h"

#define START_BIT       0x1u
#define SPRAY_DONE_BIT  0x2u

#define FAN_BIT         0x1u
#define HEATER_BIT      0x2u
#define SPRAY_OK_BIT    0x4u

#define PURGE_TIME      60
#define FLASH_TIME      120
#define CURE_TIME       300

typedef struct {
    unsigned int tick;
    float temp;
    float humidity;
    uint32_t switches;
} scenario_entry_t;

typedef struct {
    scenario_entry_t *entries;
    size_t count;
    size_t capacity;
    time_t start_time;
} scenario_t;

static const char *state_names[] = {
    "IDLE", "PURGE", "SPRAY", "FLASH_OFF", "CURE"
};

/* Add one scenario entry, growing the array when necessary. */
static int scenario_add(scenario_t *scenario,
                        scenario_entry_t entry)
{
    if (scenario->count == scenario->capacity) {
        size_t new_capacity =
            scenario->capacity == 0 ? 16 : scenario->capacity * 2;

        scenario_entry_t *new_entries =
            realloc(scenario->entries,
                    new_capacity * sizeof(*new_entries));

        if (new_entries == NULL) {
            return -1;
        }

        scenario->entries = new_entries;
        scenario->capacity = new_capacity;
    }

    scenario->entries[scenario->count++] = entry;
    return 0;
}

/* Read the start timestamp and all scenario entries. */
static int scenario_load(const char *filename,
                         scenario_t *scenario)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Could not open scenario");
        return -1;
    }

    char line[256];
    bool have_start = false;

    while (fgets(line, sizeof(line), file) != NULL) {
        char *comment = strchr(line, '#');

        if (comment != NULL) {
            *comment = '\0';
        }

        char *p = line;

        while (*p == ' ' || *p == '\t' ||
               *p == '\n' || *p == '\r') {
            p++;
        }

        if (*p == '\0') {
            continue;
        }

        if (!have_start) {
            long long timestamp;

            if (sscanf(p, "start %lld", &timestamp) != 1) {
                fprintf(stderr, "Invalid scenario start time\n");
                fclose(file);
                return -1;
            }

            scenario->start_time = (time_t)timestamp;
            have_start = true;
            continue;
        }

        scenario_entry_t entry;

        if (sscanf(p, "%u %f %f %" SCNx32,
                   &entry.tick,
                   &entry.temp,
                   &entry.humidity,
                   &entry.switches) != 4) {
            fprintf(stderr, "Invalid scenario line: %s", p);
            fclose(file);
            return -1;
        }

        if (scenario->count > 0 &&
            entry.tick <= scenario->entries[scenario->count - 1].tick) {
            fprintf(stderr, "Scenario ticks must increase\n");
            fclose(file);
            return -1;
        }

        if (scenario_add(scenario, entry) != 0) {
            fprintf(stderr, "Out of memory reading scenario\n");
            fclose(file);
            return -1;
        }
    }

    fclose(file);

    if (!have_start || scenario->count == 0 ||
        scenario->entries[0].tick != 0) {
        fprintf(stderr, "Scenario needs a start time and tick 0\n");
        return -1;
    }

    return 0;
}

/* Determine whether a switch was newly pressed. */
static bool switch_pressed(uint32_t current,
                           uint32_t previous,
                           uint32_t mask)
{
    return (current & mask) != 0 &&
           (previous & mask) == 0;
}

/* Convert the current state to an RGB LED color. */
static uint32_t state_color(booth_state_t state)
{
    switch (state) {
        case IDLE:
            return build_rgb_reg(1, 0, 0, 255);

        case PURGE:
            return build_rgb_reg(1, 0, 255, 0);

        case SPRAY:
            return build_rgb_reg(1, 255, 0, 0);

        case FLASH_OFF:
            return build_rgb_reg(1, 255, 165, 0);

        case CURE:
            return build_rgb_reg(1, 255, 255, 0);

        default:
            return build_rgb_reg(0, 0, 0, 0);
    }
}

/* Print one trace line for a tick. */
static void print_trace(const reading_t *reading,
                        booth_state_t state,
                        uint32_t leds,
                        uint32_t status_word)
{
    struct tm *utc = gmtime(&reading->timestamp);

    if (utc == NULL) {
        fprintf(stderr, "Invalid timestamp\n");
        return;
    }

    char timestamp[32];

    strftime(timestamp, sizeof(timestamp),
             "%Y-%m-%d %H:%M:%S", utc);

    printf("%s state=%s temp=%.1f rh=%.1f "
           "leds=0x%X status=0x%08" PRIX32 "\n",
           timestamp,
           state_names[state],
           reading->temp_c,
           reading->rh_pct,
           (unsigned int)leds,
           status_word);
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3) {
        fprintf(stderr,
                "Usage: %s scenario.txt [--report]\n",
                argv[0]);
        return 1;
    }

    bool report_only = false;

    if (argc == 3) {
        if (strcmp(argv[2], "--report") != 0) {
            fprintf(stderr, "Unknown option: %s\n", argv[2]);
            return 1;
        }
        report_only = true;
    }

    scenario_t scenario = {0};

    if (scenario_load(argv[1], &scenario) != 0) {
        free(scenario.entries);
        return 1;
    }

    int rc = 0;

    uint32_t *base = iom361_initialize(2, 3, &rc);

    if (base == NULL || rc != 0) {
        fprintf(stderr, "IOM361 initialization failed: %d\n", rc);
        free(scenario.entries);
        return 1;
    }

    iom361_set_display(false);

    store_t *store = store_create();
    aux_t *aux = aux_create();

    if (store == NULL || aux == NULL) {
        fprintf(stderr, "Could not allocate data structures\n");
        store_destroy(store);
        aux_destroy(aux);
        free(scenario.entries);
        return 1;
    }

    booth_state_t state = IDLE;

    unsigned int state_start_tick = 0;

    uint32_t previous_switches = 0;

    bool heater_on = false;

    size_t scenario_index = 0;
    unsigned int last_tick =
        scenario.entries[scenario.count - 1].tick;

    float current_temp = 23.5f;
    float current_humidity = 75.0f;
    uint32_t current_switches = 0;

    for (unsigned int tick = 0; tick <= last_tick; tick++) {

        /* Apply scenario values when a new entry is reached. */
        if (scenario_index < scenario.count &&
            scenario.entries[scenario_index].tick == tick) {

            current_temp = scenario.entries[scenario_index].temp;
            current_humidity =
                scenario.entries[scenario_index].humidity;
            current_switches =
                scenario.entries[scenario_index].switches;

            scenario_index++;
        }

        _iom361_set_sensor1(current_temp, current_humidity);
        _iom361_set_switches(current_switches);

        time_t timestamp = scenario.start_time + tick;

        reading_t reading;

        rc = sensor_read(base, timestamp, &reading);

        if (rc != 0) {
            fprintf(stderr, "Sensor read failed: %d\n", rc);
            break;
        }

        bool spray_ok =
            reading.temp_c >= 18.0f &&
            reading.temp_c <= 30.0f &&
            reading.rh_pct < 70.0f;

        bool start_pressed =
            switch_pressed(current_switches,
                           previous_switches,
                           START_BIT);

        bool spray_done_pressed =
            switch_pressed(current_switches,
                           previous_switches,
                           SPRAY_DONE_BIT);

        booth_event_t event = EVENT_NONE;

        switch (state) {
            case IDLE:
                if (start_pressed) {
                    event = EVENT_START;
                }
                break;

            case PURGE:
                if (tick - state_start_tick >= PURGE_TIME &&
                    spray_ok) {
                    event = EVENT_PURGE_READY;
                }
                break;

            case SPRAY:
                if (spray_done_pressed) {
                    event = EVENT_SPRAY_DONE;
                }
                break;

            case FLASH_OFF:
                if (tick - state_start_tick >= FLASH_TIME) {
                    event = EVENT_FLASH_DONE;
                }
                break;

            case CURE:
                if (tick - state_start_tick >= CURE_TIME) {
                    event = EVENT_CURE_DONE;
                }
                break;

            default:
                break;
        }

        fsm_transition_t transition =
            fsm_transition(state, event);

        if (transition.next_state != state) {
            state = transition.next_state;
            state_start_tick = tick;

            if (state == CURE) {
                heater_on = false;
            }
        }

        /* Heater hysteresis applies during CURE. */
        if (state == CURE) {
            if (reading.temp_c < 58.0f) {
                heater_on = true;
            } else if (reading.temp_c >= 60.0f) {
                heater_on = false;
            }
        } else {
            heater_on = false;
        }

        bool fan_on =
            state == PURGE || state == FLASH_OFF;

        uint32_t leds = 0;

        if (fan_on) {
            leds |= FAN_BIT;
        }

        if (heater_on) {
            leds |= HEATER_BIT;
        }

        if (spray_ok) {
            leds |= SPRAY_OK_BIT;
        }

        booth_status_t status = {
            .state = (uint8_t)state,
            .exhaust_fan = fan_on,
            .heater = heater_on,
            .spray_ok = spray_ok,
            .conditions_met = spray_ok
        };

        uint32_t status_word = status_pack(&status);

        iom361_writeReg(base, LEDS_REG, leds, &rc);
        if (rc != 0) {
            fprintf(stderr, "LED write failed: %d\n", rc);
            break;
        }

        iom361_writeReg(base, RGB_LED_REG,
                        state_color(state), &rc);
        if (rc != 0) {
            fprintf(stderr, "RGB write failed: %d\n", rc);
            break;
        }

        record_t record = {
            .reading = reading,
            .state = (int)state,
            .status = status_word
        };

        if (store_insert(store, &record) != 0 ||
            aux_add(aux, &reading) != 0) {
            fprintf(stderr, "Could not store tick data\n");
            break;
        }

        if (!report_only) {
            print_trace(&reading, state, leds, status_word);
        }

        previous_switches = current_switches;
    }

    if (report_only) {
        report_t report;

        if (report_calculate(store, &report) == 0) {
            report_print(&report);
        } else {
            fprintf(stderr, "Could not calculate report\n");
        }
    }

    store_destroy(store);
    aux_destroy(aux);
    free(scenario.entries);

    return 0;
}
