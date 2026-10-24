#ifndef TALLY_H
#define TALLY_H

#include <stdio.h>

#define TALLY_MAX 4
#define TALLY_NAMES 16
#define TALLY_LEN 15

typedef struct tally tally_t;

tally_t *tally_create(void);

void tally_destroy(tally_t *t);

int tally_add(tally_t *t, const char *name);

int tally_count(const tally_t *t, const char *name);

int tally_size(const tally_t *t);

void tally_print(const tally_t *t, FILE *out);

#endif


