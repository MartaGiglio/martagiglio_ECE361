#include "panel.h"
#include "iom361_r4.h"

uint32_t panel_pressed(uint32_t now, uint32_t before)
{
    return now & ~before;
}

uint32_t panel_released(uint32_t now, uint32_t before)
{
    return before & ~now;
}

int panel_led(uint32_t *base, int n, int on)
{
    if (n < 0 || n > 31) {
        return -1;
    }

    int rc;
    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);

    if (rc != 0) {
        return rc;
    }

    uint32_t mask = 1u << n;

    if (on) {
        leds |= mask;
    } else {
        leds &= ~mask;
    }

    iom361_writeReg(base, LEDS_REG, leds, &rc);

    return rc;
}

int panel_toggle(uint32_t *base, int n)
{
    if (n < 0 || n > 31) {
        return -1;
    }

    int rc;
    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);

    if (rc != 0) {
        return rc;
    }

    uint32_t mask = 1u << n;

    leds ^= mask;

    iom361_writeReg(base, LEDS_REG, leds, &rc);

    return rc;
}
