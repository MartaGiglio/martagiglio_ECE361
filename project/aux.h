#ifndef AUX_H
#define AUX_H

#include <stdbool.h>
#include <stddef.h>
#include "sensor.h"

typedef struct aux aux_t;

aux_t *aux_create(void);
int aux_add(aux_t *a, const reading_t *reading);
const reading_t *aux_get(const aux_t *a, size_t i);
size_t aux_count(const aux_t *a);
bool aux_is_full(const aux_t *a);
void aux_destroy(aux_t *a);

#endif
