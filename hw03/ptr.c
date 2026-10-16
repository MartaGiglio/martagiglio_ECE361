#include "ptr.h"
#include <stddef.h>

void swap_float(float *a, float *b)
{
    if (a == NULL || b == NULL) {
        return;
    }

    float t = *a;
    *a = *b;
    *b = t;
}

int minmax(const float *a, int n, float *min, float *max)
{
    if (a == NULL || min == NULL || max == NULL || n < 1) {
        return -1;
    }

    float smallest = *a;
    float largest = *a;

    for (const float *p = a + 1; p < a + n; p++) {
        if (*p < smallest) {
            smallest = *p;
        }

        if (*p > largest) {
            largest = *p;
        }
    }

    *min = smallest;
    *max = largest;

    return 0;
}

const float *find_above(const float *a, int n, float threshold)
{
    if (a == NULL || n < 1) {
        return NULL;
    }

    const float *p = a;

    while (p < a + n) {
        if (*p > threshold) {
            return p;
        }

        p++;
    }

    return NULL;
}

void reverse(float *a, int n)
{
    if (a == NULL || n < 2) {
        return;
    }

    for (float *lo = a, *hi = a + n - 1; lo < hi; lo++, hi--) {
         swap_float(lo, hi);
    }
}
