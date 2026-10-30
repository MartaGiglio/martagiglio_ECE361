#ifndef LOCK_H
#define LOCK_H

typedef enum {
    LOCKED,
    GOT1,
    GOT2,
    OPEN,
    N_STATES
} state_t;

typedef enum {
    E_RIGHT,
    E_WRONG,
    E_CLOSE,
    E_TIMEOUT,
    N_EVENTS
} event_t;

typedef enum {
    A_UNDEFINED,
    A_NONE,
    A_UNLOCK,
    A_LOCK,
    A_BEEP,
    N_ACTIONS
} action_t;

typedef struct {
    state_t next;
    action_t action;
} transition_t;

/* Transition table */
extern const transition_t lock_table[N_STATES][N_EVENTS];

/* Process an event and return the next state */
state_t lock_step(state_t s, event_t e, action_t *action);

/* Convert an input character into an event */
int lock_event(state_t s, char c);

/* Return the name of a state */
const char *state_name(state_t s);

/* Return the name of an event */
const char *event_name(event_t e);

/* Return the name of an action */
const char *action_name(action_t a);

#endif
