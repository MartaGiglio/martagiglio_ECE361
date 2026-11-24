#include <stdio.h>
#include "../aux.h"

static int passed = 0;
static int failed = 0;

#define CHECK(cond, name) do { \
    if (cond) passed++; \
    else { failed++; printf("FAIL: %s\n", name); } \
} while (0)

static reading_t make_reading(int timestamp)
{
    reading_t r = {0};
    r.timestamp = timestamp;
    r.temp_c = 20.0f + timestamp;
    r.rh_pct = 50.0f;
    return r;
}

static void test_basic_operations(void)
{
    aux_t *a = aux_create();
    CHECK(a != NULL, "create aux");

    if (a == NULL) return;

    CHECK(aux_count(a) == 0, "initial count is zero");
    CHECK(!aux_is_full(a), "initial buffer not full");

    reading_t r = make_reading(1);
    CHECK(aux_add(a, &r) == 0, "add first reading");
    CHECK(aux_count(a) == 1, "count after first add");

    const reading_t *got = aux_get(a, 0);
    CHECK(got != NULL, "get newest reading");

    if (got != NULL)
        CHECK(got->timestamp == 1, "newest timestamp correct");

    CHECK(aux_get(a, 1) == NULL, "invalid index returns NULL");

    aux_destroy(a);
}

static void test_wraparound(void)
{
    aux_t *a = aux_create();
    if (a == NULL) {
        CHECK(0, "create aux for wraparound");
        return;
    }

    for (int i = 1; i <= 40; i++) {
        reading_t r = make_reading(i);
        CHECK(aux_add(a, &r) == 0, "add reading");
    }

    CHECK(aux_count(a) == 32, "capacity remains 32");
    CHECK(aux_is_full(a), "buffer is full");

    const reading_t *newest = aux_get(a, 0);
    const reading_t *oldest = aux_get(a, 31);

    CHECK(newest != NULL && newest->timestamp == 40,
          "newest reading is 40");

    CHECK(oldest != NULL && oldest->timestamp == 9,
          "oldest reading is 9");

    for (int i = 0; i < 32; i++) {
        const reading_t *r = aux_get(a, (size_t)i);

        CHECK(r != NULL, "reading exists after wraparound");

        if (r != NULL) {
            CHECK(r->timestamp == 40 - i,
                  "readings maintain correct order after wraparound");
        }
    }

    aux_destroy(a);
}

static void test_null_arguments(void)
{
    reading_t r = make_reading(1);

    CHECK(aux_add(NULL, &r) != 0, "reject NULL buffer");
    CHECK(aux_add(NULL, NULL) != 0, "reject NULL arguments");
    CHECK(aux_get(NULL, 0) == NULL, "NULL buffer get");

    aux_destroy(NULL);
}

int main(void)
{
    test_basic_operations();
    test_wraparound();
    test_null_arguments();

    printf("AUX: %d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
