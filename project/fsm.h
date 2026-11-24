#ifndef FSM_H
#define FSM_H

typedef enum {
    IDLE = 0,
    PURGE,
    SPRAY,
    FLASH_OFF,
    CURE,
    FSM_STATE_COUNT
} booth_state_t;

typedef enum {
    EVENT_NONE = 0,
    EVENT_START,
    EVENT_PURGE_READY,
    EVENT_SPRAY_DONE,
    EVENT_FLASH_DONE,
    EVENT_CURE_DONE,
    FSM_EVENT_COUNT
} booth_event_t;

typedef enum {
    ACTION_NONE = 0,
    ACTION_START_PURGE,
    ACTION_START_SPRAY,
    ACTION_START_FLASH_OFF,
    ACTION_START_CURE,
    ACTION_END_CYCLE
} fsm_action_t;

typedef struct {
    booth_state_t next_state;
    fsm_action_t action;
} fsm_transition_t;

fsm_transition_t fsm_transition(booth_state_t current,
                                booth_event_t event);

booth_state_t fsm_next_state(booth_state_t current,
                             booth_event_t event);

#endif
