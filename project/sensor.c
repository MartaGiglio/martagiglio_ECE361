#include <stddef.h>
#include <math.h>
#include "sensor.h"
#include "iom361_r4.h"

int sensor_read(uint32_t *base, time_t timestamp, reading_t *reading)
{
int rc = 0;


if (base == NULL || reading == NULL) {
    return -1;
}

uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);
if (rc != 0) {
    return rc;
}

uint32_t raw_humid = iom361_readReg(base, HUMID_REG, &rc);
if (rc != 0) {
    return rc;
}

float temperature = ((float)raw_temp / 1048576.0f) * 200.0f - 50.0f;
float humidity = ((float)raw_humid / 1048576.0f) * 100.0f;

reading->timestamp = timestamp;
reading->temp_c = roundf(temperature * 10.0f) / 10.0f;
reading->rh_pct = roundf(humidity * 10.0f) / 10.0f;

return 0;


}


