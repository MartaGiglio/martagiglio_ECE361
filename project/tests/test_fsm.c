#include <stdio.h>
#include "../fsm.h"

static int passed = 0;
static int failed = 0;

#define CHECK(cond, name) do { \
    if (cond) passed++; \
    else { failed++; printf("FAIL: %s\n", name); } \
} while (0)

static void test_normal_cycle(void)
{
    fsm_transition_t t;

    t = fsm_transition(IDLE, EVENT_START);
    CHECK(t.next_state == PURGE, "IDLE -> PURGE");
    CHECK(t.action == ACTION_START_PURGE, "start purge action");

    t = fsm_transition(PURGE, EVENT_PURGE_READY);
    CHECK(t.next_state == SPRAY, "PURGE -> SPRAY");
    CHECK(t.action == ACTION_START_SPRAY, "start spray action");

    t = fsm_transition(SPRAY, EVENT_SPRAY_DONE);
    CHECK(t.next_state == FLASH_OFF, "SPRAY -> FLASH_OFF");

    t = fsm_transition(FLASH_OFF, EVENT_FLASH_DONE);
    CHECK(t.next_state == CURE, "FLASH_OFF -> CURE");

    t = fsm_transition(CURE, EVENT_CURE_DONE);
    CHECK(t.next_state == IDLE, "CURE -> IDLE");
    CHECK(t.action == ACTION_END_CYCLE, "end cycle action");
}

static void test_ignored_events(void)
{
    CHECK(fsm_next_state(IDLE, EVENT_SPRAY_DONE) == IDLE,
          "ignore SPRAY_DONE in IDLE");

    CHECK(fsm_next_state(PURGE, EVENT_SPRAY_DONE) == PURGE,
          "ignore SPRAY_DONE in PURGE");

    CHECK(fsm_next_state(SPRAY, EVENT_PURGE_READY) == SPRAY,
          "ignore PURGE_READY in SPRAY");

    CHECK(fsm_next_state(FLASH_OFF, EVENT_SPRAY_DONE) == FLASH_OFF,
          "ignore SPRAY_DONE in FLASH_OFF");

    CHECK(fsm_next_state(CURE, EVENT_START) == CURE,
          "ignore START in CURE");
}

static void test_invalid_input(void)
{
    fsm_transition_t t =
        fsm_transition((booth_state_t)99, EVENT_START);

    CHECK(t.next_state == IDLE, "invalid state returns IDLE");
    CHECK(t.action == ACTION_NONE, "invalid state has no action");

    t = fsm_transition(IDLE, (booth_event_t)99);
    CHECK(t.next_state == IDLE, "invalid event returns IDLE");
}

int main(void)
{
    test_normal_cycle();
    test_ignored_events();
    test_invalid_input();

    printf("FSM: %d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
