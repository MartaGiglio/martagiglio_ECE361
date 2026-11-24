#include <stdio.h>
#include <math.h>
#include "../sensor.h"
#include "../iom361_r4.h"

static int passed = 0;
static int failed = 0;

#define CHECK(cond, name) do { \
    if (cond) passed++; \
    else { failed++; printf("FAIL: %s\n", name); } \
} while (0)

static void test_sensor_read(void)
{
    int rc = 0;
    uint32_t *base = iom361_initialize(2, 3, &rc);

    CHECK(base != NULL, "initialize IOM361");
    CHECK(rc == 0, "initialization return code");

    if (base == NULL) return;

    _iom361_set_sensor1(23.4f, 55.6f);

    reading_t r = {0};
    int result = sensor_read(base, 12345, &r);

    CHECK(result == 0, "sensor read succeeds");
    CHECK(r.timestamp == 12345, "timestamp is stored");
    CHECK(fabsf(r.temp_c - 23.4f) < 0.1f,
          "temperature conversion");
    CHECK(fabsf(r.rh_pct - 55.6f) < 0.1f,
          "humidity conversion");
}

static void test_sensor_edges(void)
{
    int rc = 0;
    uint32_t *base = iom361_initialize(2, 3, &rc);

    if (base == NULL) {
        CHECK(0, "initialize IOM361 for edge tests");
        return;
    }

    _iom361_set_sensor1(-50.0f, 0.0f);

    reading_t r = {0};
    CHECK(sensor_read(base, 1, &r) == 0, "read minimum sensor values");
    CHECK(fabsf(r.temp_c - (-50.0f)) < 0.1f,
          "minimum temperature");
    CHECK(fabsf(r.rh_pct) < 0.1f, "minimum humidity");

    _iom361_set_sensor1(149.9f, 99.9f);

    CHECK(sensor_read(base, 2, &r) == 0, "read high sensor values");
    CHECK(r.temp_c > 149.0f, "high temperature conversion");
    CHECK(r.rh_pct > 99.0f, "high humidity conversion");
}

static void test_invalid_arguments(void)
{
    reading_t r = {0};

    CHECK(sensor_read(NULL, 0, &r) == -1, "reject NULL base");
    CHECK(sensor_read((uint32_t *)1, 0, NULL) == -1,
          "reject NULL reading");
}

int main(void)
{
    iom361_set_display(false);

    test_sensor_read();
    test_sensor_edges();
    test_invalid_arguments();

    printf("SENSOR: %d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
