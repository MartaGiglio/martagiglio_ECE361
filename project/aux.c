#include "aux.h"

#include <stdlib.h>

#define AUX_CAPACITY 32

struct aux {
    reading_t readings[AUX_CAPACITY];
    size_t head;
    size_t count;
};

/* Creates an empty circular buffer. */
aux_t *aux_create(void) {
    aux_t *a = malloc(sizeof(aux_t));

    if (a == NULL) {
        return NULL;
    }

    a->head = 0;
    a->count = 0;

    return a;
}

/* Adds a reading, overwriting the oldest if full. */
int aux_add(aux_t *a, const reading_t *reading) {
    if (a == NULL || reading == NULL) {
        return -1;
    }

    if (a->count == 0) {
        a->head = 0;
    } else {
        a->head = (a->head + 1) % AUX_CAPACITY;
    }

    a->readings[a->head] = *reading;

    if (a->count < AUX_CAPACITY) {
        a->count++;
    }

    return 0;
}

/* Returns the i-th newest reading. */
const reading_t *aux_get(const aux_t *a, size_t i) {
    if (a == NULL || i >= a->count) {
        return NULL;
    }

    size_t index = (a->head + AUX_CAPACITY - i) % AUX_CAPACITY;

    return &a->readings[index];
}

/* Returns the number of readings in the buffer. */
size_t aux_count(const aux_t *a) {
    if (a == NULL) {
        return 0;
    }

    return a->count;
}

/* Checks whether the buffer is full. */
bool aux_is_full(const aux_t *a) {
    return a != NULL && a->count == AUX_CAPACITY;
}

/* Destroys the circular buffer. */
void aux_destroy(aux_t *a) {
    free(a);
}
