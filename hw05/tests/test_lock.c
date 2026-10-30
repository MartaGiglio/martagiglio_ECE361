#include <stdio.h>
#include "lock.h"

static int passed = 0;
static int failed = 0;

/* Check a transition */
void test_transition(state_t s, event_t e,
                    state_t expected_state,
                    action_t expected_action)
{
    action_t action;

    state_t result = lock_step(s, e, &action);

    if (result == expected_state && action == expected_action) {
        printf("PASS: %s + %s\n", state_name(s), event_name(e));
        passed++;
    } else {
        printf("FAIL: %s + %s\n", state_name(s), event_name(e));
        failed++;
    }
}

/* Check an input character */
void test_event(state_t s, char c, int expected)
{
    int result = lock_event(s, c);

    if (result == expected) {
        printf("PASS: lock_event(%s, '%c')\n", state_name(s), c);
        passed++;
    } else {
        printf("FAIL: lock_event(%s, '%c')\n", state_name(s), c);
        failed++;
    }
}

/* Test a complete input sequence */
state_t run_sequence(const char *sequence)
{
    state_t state = LOCKED;
    action_t action;

    while (*sequence != '\0') {
        int event = lock_event(state, *sequence);

        if (event >= 0) {
            state = lock_step(state, (event_t)event, &action);
        }

        sequence++;
    }

    return state;
}

void test_sequence(const char *sequence, state_t expected)
{
    state_t result = run_sequence(sequence);

    if (result == expected) {
        printf("PASS: sequence %s\n", sequence);
        passed++;
    } else {
        printf("FAIL: sequence %s\n", sequence);
        failed++;
    }
}

int main(void)
{
    printf("\n--- TRANSITION TESTS ---\n");

    test_transition(LOCKED, E_RIGHT, GOT1, A_NONE);
    test_transition(LOCKED, E_WRONG, LOCKED, A_BEEP);
    test_transition(LOCKED, E_CLOSE, LOCKED, A_NONE);
    test_transition(LOCKED, E_TIMEOUT, LOCKED, A_NONE);

    test_transition(GOT1, E_RIGHT, GOT2, A_NONE);
    test_transition(GOT1, E_WRONG, LOCKED, A_BEEP);
    test_transition(GOT1, E_CLOSE, GOT1, A_NONE);
    test_transition(GOT1, E_TIMEOUT, LOCKED, A_NONE);

    test_transition(GOT2, E_RIGHT, OPEN, A_UNLOCK);
    test_transition(GOT2, E_WRONG, LOCKED, A_BEEP);
    test_transition(GOT2, E_CLOSE, GOT2, A_NONE);
    test_transition(GOT2, E_TIMEOUT, LOCKED, A_NONE);

    test_transition(OPEN, E_RIGHT, OPEN, A_NONE);
    test_transition(OPEN, E_WRONG, OPEN, A_NONE);
    test_transition(OPEN, E_CLOSE, LOCKED, A_LOCK);
    test_transition(OPEN, E_TIMEOUT, LOCKED, A_LOCK);

    printf("\n--- EVENT TESTS ---\n");

test_event(LOCKED, '3', E_RIGHT);
test_event(LOCKED, '5', E_WRONG);

test_event(GOT1, '1', E_RIGHT);
test_event(GOT1, '5', E_WRONG);

test_event(GOT2, '4', E_RIGHT);
test_event(GOT2, '5', E_WRONG);

test_event(OPEN, '3', E_WRONG);
test_event(OPEN, '5', E_WRONG);

test_event(LOCKED, 'c', E_CLOSE);
test_event(GOT1, 'c', E_CLOSE);
test_event(GOT2, 'c', E_CLOSE);
test_event(OPEN, 'c', E_CLOSE);

test_event(LOCKED, 't', E_TIMEOUT);
test_event(GOT1, 't', E_TIMEOUT);
test_event(GOT2, 't', E_TIMEOUT);
test_event(OPEN, 't', E_TIMEOUT);

test_event(LOCKED, 'x', -1);
test_event(GOT1, 'x', -1);
test_event(GOT2, 'x', -1);
test_event(OPEN, 'x', -1);

    printf("\n--- SEQUENCE TESTS ---\n");

    test_sequence("314", OPEN);
    test_sequence("3314", LOCKED);
    test_sequence("31t4", LOCKED);
    test_sequence("314c", LOCKED);

    printf("\n--- SUMMARY ---\n");

    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);

    return (failed == 0) ? 0 : 1;
}
