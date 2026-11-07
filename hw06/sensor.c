#include "sensor.h"
#include "iom361_r4.h"

#include <math.h>

#define SENSOR_MASK 0xFFFFF

int sensor_read(uint32_t *base, time_t now, reading_t *r)
{

if (base == NULL || r == NULL) {
    return 1;
}

    int rc;

    uint32_t temp_raw = iom361_readReg(base, TEMP_REG, &rc);

    if (rc != 0) {
        return rc;
    }

    uint32_t humid_raw = iom361_readReg(base, HUMID_REG, &rc);

    if (rc != 0) {
        return rc;
    }

    temp_raw &= SENSOR_MASK;
    humid_raw &= SENSOR_MASK;

    float temp = ((float)temp_raw / 1048576.0f) * 200.0f - 50.0f;
    float rh = ((float)humid_raw / 1048576.0f) * 100.0f;

    r->t = now;
    r->temp_c = roundf(temp * 10.0f) / 10.0f;
    r->rh_pct = roundf(rh * 10.0f) / 10.0f;

    return 0;
}
