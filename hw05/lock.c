#include "lock.h"

/* Transition table */
const transition_t lock_table[N_STATES][N_EVENTS] = {

    [LOCKED] = {
        [E_RIGHT]   = {GOT1, A_NONE},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {LOCKED, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },

    [GOT1] = {
        [E_RIGHT]   = {GOT2, A_NONE},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {GOT1, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },

    [GOT2] = {
        [E_RIGHT]   = {OPEN, A_UNLOCK},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {GOT2, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },

    [OPEN] = {
        [E_RIGHT]   = {OPEN, A_NONE},
        [E_WRONG]   = {OPEN, A_NONE},
        [E_CLOSE]   = {LOCKED, A_LOCK},
        [E_TIMEOUT] = {LOCKED, A_LOCK}
    }
};

/* Process an event using the transition table */
state_t lock_step(state_t s, event_t e, action_t *action)
{
    transition_t transition = lock_table[s][e];

    *action = transition.action;

    return transition.next;
}

/* Convert a character into an event */
int lock_event(state_t s, char c)
{
    if (c == 'c') return E_CLOSE;
    if (c == 't') return E_TIMEOUT;

    if (c < '0' || c > '9') return -1;

    switch (s) {
        case LOCKED:
            return (c == '3') ? E_RIGHT : E_WRONG;

        case GOT1:
            return (c == '1') ? E_RIGHT : E_WRONG;

        case GOT2:
            return (c == '4') ? E_RIGHT : E_WRONG;

        case OPEN:
            return E_WRONG;

        default:
            return -1;
    }
}

/* State names */
const char *state_name(state_t s)
{
    static const char *names[] = {
        "LOCKED", "GOT1", "GOT2", "OPEN"
    };

    if (s < 0 || s >= N_STATES) return "?";

    return names[s];
}

/* Event names */
const char *event_name(event_t e)
{
    static const char *names[] = {
        "RIGHT", "WRONG", "CLOSE", "TIMEOUT"
    };

    if (e < 0 || e >= N_EVENTS) return "?";

    return names[e];
}

/* Action names */
const char *action_name(action_t a)
{
    static const char *names[] = {
        "UNDEFINED", "NONE", "UNLOCK", "LOCK", "BEEP"
    };

    if (a < 0 || a >= N_ACTIONS) return "?";

    return names[a];
}
