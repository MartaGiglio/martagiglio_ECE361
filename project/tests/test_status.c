#include <stdio.h>
#include "../status.h"

static int passed = 0;
static int failed = 0;

#define CHECK(cond, name) do { \
    if (cond) { passed++; } \
    else { failed++; printf("FAIL: %s\n", name); } \
} while (0)

static void test_roundtrip(void)
{
    booth_status_t in = { 3, 1, 0, 1, 1 };
    booth_status_t out = status_unpack(status_pack(&in));

    CHECK(out.state == 3,          "roundtrip state");
    CHECK(out.exhaust_fan == 1,    "roundtrip exhaust_fan");
    CHECK(out.heater == 0,         "roundtrip heater");
    CHECK(out.spray_ok == 1,       "roundtrip spray_ok");
    CHECK(out.conditions_met == 1, "roundtrip conditions_met");
}

static void test_all_states(void)
{
    for (uint8_t st = 0; st <= 4; st++) {
        booth_status_t in = { st, 0, 0, 0, 0 };
        booth_status_t out = status_unpack(status_pack(&in));
        CHECK(out.state == st, "roundtrip every state 0..4");
    }
}

static void test_unused_bits(void)
{
    booth_status_t in = { 7, 1, 1, 1, 1 };   /* tutti i campi al massimo */
    uint32_t w = status_pack(&in);

    CHECK((w & 0x00000008u) == 0, "bit 3 is 0");
    CHECK((w & 0xFFFFFF00u) == 0, "bits 8..31 are 0");
}

static void test_example_from_spec(void)
{
    /* PURGE (stato 1) con ventola accesa -> 0x00000011 */
    booth_status_t in = { 1, 1, 0, 0, 0 };
    CHECK(status_pack(&in) == 0x00000011u, "PURGE + fan = 0x11");
}

int main(void)
{
    test_roundtrip();
    test_all_states();
    test_unused_bits();
    test_example_from_spec();

    printf("%d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
