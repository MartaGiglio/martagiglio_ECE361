#ifndef STORE_H
#define STORE_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include "sensor.h"

typedef struct store store_t;

typedef struct {
    reading_t reading;
    int       state;
    uint32_t  status;
} record_t;

/* <0 se a viene prima di b, 0 se uguali, >0 se dopo. Confronta solo il timestamp. */
int record_compare(const record_t *a, const record_t *b);

store_t        *store_create(void);
int             store_insert(store_t *s, const record_t *r);  /* 0 = ok, !=0 errore */
const record_t *store_find(const store_t *s, time_t timestamp); /* NULL se assente */
void            store_walk(const store_t *s,
                           void (*callback)(const record_t *, void *),
                           void *ctx);
size_t          store_count(const store_t *s);
int             store_height(const store_t *s);
void            store_destroy(store_t *s);  /* accetta anche NULL */

#endif
