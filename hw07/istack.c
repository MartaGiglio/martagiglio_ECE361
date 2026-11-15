#include "istack.h"
#include <stdlib.h>

struct istack {
    int *data;
    size_t count;
    size_t capacity;
};

istack_t *istack_create(void)
{
    istack_t *s = malloc(sizeof(istack_t));

    if (s == NULL) {
        return NULL;
    }

    s->data = malloc(ISTACK_START * sizeof(int));

    if (s->data == NULL) {
        free(s);
        return NULL;
    }

    s->count = 0;
    s->capacity = ISTACK_START;

    return s;
}

void istack_destroy(istack_t *s)
{
    if (s == NULL) {
        return;
    }

    free(s->data);
    free(s);
}

int istack_push(istack_t *s, int v)
{
    if (s == NULL) {
        return -1;
    }

    if (s->count == s->capacity) {
        size_t new_capacity = s->capacity * 2;

        int *temp = realloc(s->data, new_capacity * sizeof(int));

        if (temp == NULL) {
            return -1;
        }

        s->data = temp;
        s->capacity = new_capacity;
    }

    s->data[s->count] = v;
    s->count++;

    return 0;
}

int istack_pop(istack_t *s, int *v)
{
    if (s == NULL || s->count == 0) {
        return -1;
    }

    s->count--;

    if (v != NULL) {
        *v = s->data[s->count];
    }

    return 0;
}

int istack_peek(const istack_t *s, int *v)
{
    if (s == NULL || s->count == 0) {
        return -1;
    }

    if (v != NULL) {
        *v = s->data[s->count - 1];
    }

    return 0;
}

size_t istack_count(const istack_t *s)
{
    if (s == NULL) {
        return 0;
    }

    return s->count;
}

bool istack_is_empty(const istack_t *s)
{
    return s == NULL || s->count == 0;
}

size_t istack_capacity(const istack_t *s)
{
    if (s == NULL) {
        return 0;
    }

    return s->capacity;
}

int *istack_to_array(const istack_t *s, size_t *n)
{
    if (n != NULL) {
        *n = 0;
    }

    if (s == NULL || s->count == 0 || n == NULL) {
        return NULL;
    }

    int *array = malloc(s->count * sizeof(int));

    if (array == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < s->count; i++) {
        array[i] = s->data[i];
    }

    *n = s->count;

    return array;
}
