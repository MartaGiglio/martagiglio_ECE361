#include <stdio.h>
#include "iom361_r4.h"
#include "sensor.h"

int main(void)
{
    int rc = 0;

    uint32_t *base = iom361_initialize(2, 3, &rc);

    if (rc != 0 || base == NULL) {
        printf("Error initializing I/O module: %d\n", rc);
        return 1;
    }

    _iom361_set_sensor1(23.4f, 55.6f);

    reading_t reading;
    time_t timestamp = 0; /* Temporary timestamp for this test */

    rc = sensor_read(base, timestamp, &reading);

    if (rc != 0) {
        printf("Error reading sensor: %d\n", rc);
        return 1;
    }

    printf("Paint Booth Sensor Readings\n");
    printf("Temperature: %.1f C\n", reading.temp_c);
    printf("Humidity: %.1f %%RH\n", reading.rh_pct);

    return 0;
}



