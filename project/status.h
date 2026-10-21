#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>

typedef struct {
    uint8_t state;          /* 0..4 (3 bit) */
    uint8_t exhaust_fan;    /* 0/1 */
    uint8_t heater;         /* 0/1 */
    uint8_t spray_ok;       /* 0/1 */
    uint8_t conditions_met; /* 0/1 */
} booth_status_t;

uint32_t       status_pack(const booth_status_t *s);
booth_status_t status_unpack(uint32_t word);

#endif


