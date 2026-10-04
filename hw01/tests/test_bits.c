#include <stdint.h>
#include <stdio.h>

#include "../bits.h"
#include "../status.h"

static int tests_run = 0;
static int tests_passed = 0;

static void check(const char *name, int condition)
{
tests_run++;

if (condition) {
    tests_passed++;
    printf("PASS: %s\n", name);
} else {
    printf("FAIL: %s\n", name);
}

}

int main(void)
{
/* get_field */
check("get_field basic", get_field(0x1631, 0, 1) == 1);
check("get_field width 3", get_field(0x1631, 4, 3) == 3);
check("get_field width 8", get_field(0x1631, 8, 8) == 22);

/* get_field boundary cases */
check("get_field width 1", get_field(0x80000000u, 31, 1) == 1);
check("get_field pos 31", get_field(0x80000000u, 31, 1) == 1);
check("get_field width 32", get_field(0xFFFFFFFFu, 0, 32) == 0xFFFFFFFFu);

/* set_field */
check("set_field basic", set_field(0x00, 0, 4, 0xA) == 0xA);
check("set_field shifted", set_field(0x00, 4, 4, 0xA) == 0xA0);

/* set_field value larger than field */
check("set_field truncates large value",
      set_field(0x00, 0, 4, 0x1F) == 0xF);

/* set_field boundary cases */
check("set_field width 1",
      set_field(0x00, 31, 1, 1) == 0x80000000u);

check("set_field width 32",
      set_field(0x00, 0, 32, 0xFFFFFFFFu) == 0xFFFFFFFFu);

/* sign_extend */
check("sign_extend positive", sign_extend(0x7F, 8) == 127);
check("sign_extend negative", sign_extend(0x80, 8) == -128);
check("sign_extend -8", sign_extend(0xF8, 8) == -8);

/* sign_extend boundary cases */
check("sign_extend width 1 positive", sign_extend(0, 1) == 0);
check("sign_extend width 1 negative", sign_extend(1, 1) == -1);
check("sign_extend width 32 minimum",
      sign_extend(0x80000000u, 32) == INT32_MIN);

/* status_unpack */
ThermostatStatus status1 = status_unpack(0x1631);

check("status setpoint", status1.setpoint == 22);
check("status mode", status1.mode == 3);
check("status heat", status1.heat == 1);
check("status cool", status1.cool == 0);
check("status fan", status1.fan == 0);
check("status fault", status1.fault == 0);
check("status reserved", status1.reserved == 0);

/* Additional status words */
ThermostatStatus status2 = status_unpack(0x80F8);
check("status negative setpoint", status2.setpoint == -128);
check("status mode from second word", status2.mode == 7);

ThermostatStatus status3 = status_unpack(0x05F0);
check("status third word setpoint", status3.setpoint == 5);
check("status third word mode", status3.mode == 7);

printf("\nSummary: %d/%d tests passed\n", tests_passed, tests_run);

return tests_passed == tests_run ? 0 : 1;


}
