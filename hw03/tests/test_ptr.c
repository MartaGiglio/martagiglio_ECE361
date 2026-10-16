#include <stdio.h>
#include <stddef.h>
#include "../ptr.h"

int tests_run = 0;
int tests_failed = 0;

void check(int condition, const char *name)
{
    tests_run++;

    if (condition) {
        printf("PASS: %s\n", name);
    } else {
        printf("FAIL: %s\n", name);
        tests_failed++;
    }
}

void test_swap_float(void)
{
    float a = 1.5f;
    float b = 2.5f;

    swap_float(&a, &b);

    check(a == 2.5f && b == 1.5f, "swap_float exchanges values");
}

void test_minmax(void)
{
    float values[] = {4.0f, -2.0f, 7.0f, 1.0f};
    float min = 0.0f;
    float max = 0.0f;

    int result = minmax(values, 4, &min, &max);
    check(result == 0 && min == -2.0f && max == 7.0f,
          "minmax finds minimum and maximum");

    float single[] = {5.0f};
    result = minmax(single, 1, &min, &max);
    check(result == 0 && min == 5.0f && max == 5.0f,
          "minmax handles one element");

    min = 10.0f;
    max = 20.0f;
    result = minmax(values, 0, &min, &max);
    check(result == -1 && min == 10.0f && max == 20.0f,
          "minmax rejects n = 0 without changing outputs");

    result = minmax(NULL, 4, &min, &max);
    check(result == -1 && min == 10.0f && max == 20.0f,
          "minmax rejects NULL array without changing outputs");

    result = minmax(values, 4, NULL, &max);
    check(result == -1 && max == 20.0f,
          "minmax rejects NULL min pointer");

    result = minmax(values, 4, &min, NULL);
    check(result == -1 && min == 10.0f,
          "minmax rejects NULL max pointer");
}

void test_find_above(void)
{
    float values[] = {2.0f, 5.0f, 8.0f, 5.0f};

    const float *found = find_above(values, 4, 4.0f);
    check(found == &values[1],
          "find_above returns first matching element");

    found = find_above(values, 4, 8.0f);
    check(found == NULL,
          "find_above returns NULL when no value is greater");

    found = find_above(values, 4, 5.0f);
    check(found == &values[2],
          "find_above does not match an equal value");

    found = find_above(NULL, 4, 1.0f);
    check(found == NULL,
          "find_above handles NULL array");
}

void test_reverse(void)
{
    float even[] = {1.0f, 2.0f, 3.0f, 4.0f};
    reverse(even, 4);
    check(even[0] == 4.0f && even[1] == 3.0f &&
          even[2] == 2.0f && even[3] == 1.0f,
          "reverse handles even-length array");

    float odd[] = {1.0f, 2.0f, 3.0f};
    reverse(odd, 3);
    check(odd[0] == 3.0f && odd[1] == 2.0f &&
          odd[2] == 1.0f,
          "reverse handles odd-length array");

    float single[] = {9.0f};
    reverse(single, 1);
    check(single[0] == 9.0f,
          "reverse leaves one-element array unchanged");

    reverse(NULL, 3);
    check(1, "reverse safely handles NULL array");
}

int main(void)
{
    test_swap_float();
    test_minmax();
    test_find_above();
    test_reverse();

    printf("\nTests run: %d\n", tests_run);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
