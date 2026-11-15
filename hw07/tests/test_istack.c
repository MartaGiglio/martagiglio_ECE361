#include "../istack.h"
#include <stdio.h>
#include <stdlib.h>

static int passed = 0;
static int failed = 0;

#define CHECK(name, condition) do { \
    if (condition) { \
        printf("PASS: %s\n", name); \
        passed++; \
    } else { \
        printf("FAIL: %s\n", name); \
        failed++; \
    } \
} while (0)


static void test_empty_stack(void)
{
    istack_t *s = istack_create();
    int value = 123;

    CHECK("Create empty stack",
          s != NULL && istack_is_empty(s));

    CHECK("Initial count is zero",
          istack_count(s) == 0);

    CHECK("Initial capacity is eight",
          istack_capacity(s) == 8);

    CHECK("Pop empty leaves value unchanged",
          istack_pop(s, &value) == -1 && value == 123);

    CHECK("Peek empty leaves value unchanged",
          istack_peek(s, &value) == -1 && value == 123);

    istack_destroy(s);
}


static void test_lifo(void)
{
    istack_t *s = istack_create();
    int value = 0;

    istack_push(s, 10);
    istack_push(s, 20);
    istack_push(s, 30);

    int correct = 1;

    correct &= istack_pop(s, &value) == 0 && value == 30;
    correct &= istack_pop(s, &value) == 0 && value == 20;
    correct &= istack_pop(s, &value) == 0 && value == 10;

    CHECK("Last in, first out", correct);

    istack_destroy(s);
}


static void test_growth(void)
{
    istack_t *s = istack_create();

    for (int i = 0; i < 8; i++) {
        istack_push(s, i);
    }

    int full_at_eight = istack_count(s) == 8 &&
                        istack_capacity(s) == 8;

    istack_push(s, 8);

    int grew_to_sixteen = istack_capacity(s) == 16;

    int values_correct = 1;
    int value;

    for (int i = 8; i >= 0; i--) {
        if (istack_pop(s, &value) != 0 || value != i) {
            values_correct = 0;
        }
    }

    CHECK("Full at eight elements", full_at_eight);
    CHECK("Ninth push doubles capacity to sixteen",
          grew_to_sixteen);
    CHECK("Growth preserves all values", values_correct);

    istack_destroy(s);
}


static void test_many_values(void)
{
    istack_t *s = istack_create();
    int value;
    int correct = 1;

    for (int i = 0; i < 1000; i++) {
        if (istack_push(s, i) != 0) {
            correct = 0;
            break;
        }
    }

    for (int i = 999; i >= 0; i--) {
        if (istack_pop(s, &value) != 0 || value != i) {
            correct = 0;
            break;
        }
    }

    CHECK("1000 pushes and pops", correct);

    istack_destroy(s);
}


static void test_to_array(void)
{
    istack_t *s = istack_create();
    size_t n = 0;

    istack_push(s, 10);
    istack_push(s, 20);
    istack_push(s, 30);

    int *array = istack_to_array(s, &n);

    int correct = array != NULL &&
                  n == 3 &&
                  array[0] == 10 &&
                  array[1] == 20 &&
                  array[2] == 30;

    CHECK("Array is bottom first", correct);

    if (array != NULL) {
        array[0] = 999;

        int value = 0;
        istack_peek(s, &value);

        CHECK("Array is an independent copy", value == 30);

        free(array);
    } else {
        CHECK("Array is an independent copy", 0);
    }

    istack_destroy(s);
}


static void test_null_arguments(void)
{
    int value = 123;
    size_t n = 99;

    CHECK("Push NULL", istack_push(NULL, 10) == -1);
    CHECK("Pop NULL", istack_pop(NULL, &value) == -1);
    CHECK("Peek NULL", istack_peek(NULL, &value) == -1);
    CHECK("Count NULL", istack_count(NULL) == 0);
    CHECK("Empty NULL", istack_is_empty(NULL));
    CHECK("Capacity NULL", istack_capacity(NULL) == 0);

    int *array = istack_to_array(NULL, &n);

    CHECK("Array NULL", array == NULL && n == 0);

    istack_t *s = istack_create();

istack_push(s, 42);

CHECK("Pop with NULL output", istack_pop(s, NULL) == 0);

istack_push(s, 42);

CHECK("Peek with NULL output", istack_peek(s, NULL) == 0);

CHECK("To array with NULL size",
      istack_to_array(s, NULL) == NULL);

istack_destroy(s);
}


static void test_independent_stacks(void)
{
    istack_t *a = istack_create();
    istack_t *b = istack_create();

    istack_push(a, 10);
    istack_push(b, 20);

    int va = 0;
    int vb = 0;

    istack_peek(a, &va);
    istack_peek(b, &vb);

    CHECK("Two stacks are independent",
          va == 10 && vb == 20);

    istack_destroy(a);
    istack_destroy(b);
}


static void test_destroy_nonempty(void)
{
    istack_t *s = istack_create();

    istack_push(s, 10);
    istack_push(s, 20);
    istack_push(s, 30);

    istack_destroy(s);

    CHECK("Destroy nonempty stack", 1);
}


int main(void)
{
    test_empty_stack();
    test_lifo();
    test_growth();
    test_many_values();
    test_to_array();
    test_null_arguments();
    test_independent_stacks();
    test_destroy_nonempty();

    printf("\n--------------------------\n");
    printf("Tests passed: %d\n", passed);
    printf("Tests failed: %d\n", failed);
    printf("--------------------------\n");

    return failed == 0 ? 0 : 1;
}
