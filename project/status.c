#include "status.h"
#include "bits.h"

#define STATE_POS        0
#define STATE_WIDTH      3
#define FAN_POS          4
#define FAN_WIDTH        1
#define HEATER_POS       5
#define HEATER_WIDTH     1
#define SPRAY_OK_POS     6
#define SPRAY_OK_WIDTH   1
#define COND_POS         7
#define COND_WIDTH       1

uint32_t status_pack(const booth_status_t *s) {
    uint32_t w = 0;
    w = set_field(w, STATE_POS,    STATE_WIDTH,    s->state);
    w = set_field(w, FAN_POS,      FAN_WIDTH,      s->exhaust_fan);
    w = set_field(w, HEATER_POS,   HEATER_WIDTH,   s->heater);
    w = set_field(w, SPRAY_OK_POS, SPRAY_OK_WIDTH, s->spray_ok);
    w = set_field(w, COND_POS,     COND_WIDTH,     s->conditions_met);
    return w;
}

booth_status_t status_unpack(uint32_t word) {
    booth_status_t s;
    s.state          = (uint8_t)get_field(word, STATE_POS,    STATE_WIDTH);
    s.exhaust_fan    = (uint8_t)get_field(word, FAN_POS,      FAN_WIDTH);
    s.heater         = (uint8_t)get_field(word, HEATER_POS,   HEATER_WIDTH);
    s.spray_ok       = (uint8_t)get_field(word, SPRAY_OK_POS, SPRAY_OK_WIDTH);
    s.conditions_met = (uint8_t)get_field(word, COND_POS,     COND_WIDTH);
    return s;
}
