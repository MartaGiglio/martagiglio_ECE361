#include <stdio.h>
#include "../store.h"

static int passed = 0;
static int failed = 0;

#define CHECK(cond, name) do { \
    if (cond) passed++; \
    else { failed++; printf("FAIL: %s\n", name); } \
} while (0)

static record_t make_record(time_t timestamp)
{
    record_t r = {0};
    r.reading.timestamp = timestamp;
    r.reading.temp_c = 23.4f;
    r.reading.rh_pct = 55.6f;
    return r;
}

struct walk_context {
    time_t previous;
    int count;
    int ordered;
};

static void check_order(const record_t *r, void *ctx)
{
    struct walk_context *w = ctx;

    if (w->count > 0 && r->reading.timestamp <= w->previous)
        w->ordered = 0;

    w->previous = r->reading.timestamp;
    w->count++;
}

static void test_compare(void)
{
    record_t a = make_record(10);
    record_t b = make_record(20);
    record_t c = make_record(10);

    CHECK(record_compare(&a, &b) < 0, "compare earlier timestamp");
    CHECK(record_compare(&b, &a) > 0, "compare later timestamp");
    CHECK(record_compare(&a, &c) == 0, "compare equal timestamps");
}

static void test_insert_find_walk(void)
{
    store_t *s = store_create();
    CHECK(s != NULL, "create store");

    if (s == NULL) return;

    for (int i = 1; i <= 100; i++) {
        record_t r = make_record(i);
        CHECK(store_insert(s, &r) == 0, "insert record");
    }

    CHECK(store_count(s) == 100, "count 100 records");

    const record_t *found = store_find(s, 50);
    CHECK(found != NULL, "find existing record");

    if (found != NULL)
        CHECK(found->reading.timestamp == 50, "found timestamp is correct");

    CHECK(store_find(s, 200) == NULL, "missing timestamp returns NULL");

    struct walk_context w = {0, 0, 1};
    store_walk(s, check_order, &w);

    CHECK(w.count == 100, "walk visits every record");
    CHECK(w.ordered, "walk is in timestamp order");

    CHECK(store_height(s) <= 8, "AVL remains balanced");

    store_destroy(s);
}

static void test_duplicate_and_null(void)
{
    store_t *s = store_create();
    record_t r = make_record(10);

    CHECK(store_insert(s, &r) == 0, "first insert succeeds");
    CHECK(store_insert(s, &r) != 0, "duplicate timestamp rejected");
    CHECK(store_insert(NULL, &r) != 0, "NULL store rejected");
    CHECK(store_insert(s, NULL) != 0, "NULL record rejected");

    store_destroy(s);
    store_destroy(NULL);
}

int main(void)
{
    test_compare();
    test_insert_find_walk();
    test_duplicate_and_null();

    printf("STORE: %d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
