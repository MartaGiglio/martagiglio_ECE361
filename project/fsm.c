#include "fsm.h"

/*
 * Transition table:
 * Rows represent current states.
 * Columns represent events.
 */
static const fsm_transition_t transitions[FSM_STATE_COUNT][FSM_EVENT_COUNT] = {

    /* IDLE */
    {
        { IDLE, ACTION_NONE },
        { PURGE, ACTION_START_PURGE },
        { IDLE, ACTION_NONE },
        { IDLE, ACTION_NONE },
        { IDLE, ACTION_NONE },
        { IDLE, ACTION_NONE }
    },

    /* PURGE */
    {
        { PURGE, ACTION_NONE },
        { PURGE, ACTION_NONE },
        { SPRAY, ACTION_START_SPRAY },
        { PURGE, ACTION_NONE },
        { PURGE, ACTION_NONE },
        { PURGE, ACTION_NONE }
    },

    /* SPRAY */
    {
        { SPRAY, ACTION_NONE },
        { SPRAY, ACTION_NONE },
        { SPRAY, ACTION_NONE },
        { FLASH_OFF, ACTION_START_FLASH_OFF },
        { SPRAY, ACTION_NONE },
        { SPRAY, ACTION_NONE }
    },

    /* FLASH_OFF */
    {
        { FLASH_OFF, ACTION_NONE },
        { FLASH_OFF, ACTION_NONE },
        { FLASH_OFF, ACTION_NONE },
        { FLASH_OFF, ACTION_NONE },
        { CURE, ACTION_START_CURE },
        { FLASH_OFF, ACTION_NONE }
    },

    /* CURE */
    {
        { CURE, ACTION_NONE },
        { CURE, ACTION_NONE },
        { CURE, ACTION_NONE },
        { CURE, ACTION_NONE },
        { CURE, ACTION_NONE },
        { IDLE, ACTION_END_CYCLE }
    }
};

fsm_transition_t fsm_transition(booth_state_t current,
                                booth_event_t event)
{
    fsm_transition_t result = { IDLE, ACTION_NONE };

    if (current < 0 || current >= FSM_STATE_COUNT ||
        event < 0 || event >= FSM_EVENT_COUNT) {
        return result;
    }

    return transitions[current][event];
}

booth_state_t fsm_next_state(booth_state_t current,
                             booth_event_t event)
{
    return fsm_transition(current, event).next_state;
}
