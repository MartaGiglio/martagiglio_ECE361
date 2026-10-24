#include "tally.h"
#include <string.h>

struct entry {
    char name[TALLY_LEN + 1];
    int count;
};

struct tally {
    struct entry entries[TALLY_NAMES];
    int size;
};

static tally_t tallies[TALLY_MAX];
static int tally_in_use[TALLY_MAX];

tally_t *tally_create(void)
{
    for (int i = 0; i < TALLY_MAX; i++) {
        if (!tally_in_use[i]) {
            tally_in_use[i] = 1;
            tallies[i].size = 0;
            return &tallies[i];
        }
    }

    return NULL;
}

void tally_destroy(tally_t *t)
{
    if (t == NULL) {
        return;
    }

    for (int i = 0; i < TALLY_MAX; i++) {
        if (t == &tallies[i]) {
            tally_in_use[i] = 0;
            tallies[i].size = 0;
            return;
        }
    }
}

int tally_add(tally_t *t, const char *name)
{
    if (t == NULL || name == NULL || name[0] == '\0') {
        return -1;
    }

    size_t len = strlen(name);

    if (len > TALLY_LEN) {
        return -1;
    }

    for (int i = 0; i < t->size; i++) {
        if (strcmp(t->entries[i].name, name) == 0) {
            t->entries[i].count++;
            return t->entries[i].count;
        }
    }
    if (t->size >= TALLY_NAMES) {
        return -1;
    }

    int pos = 0;

    while (pos < t->size &&
           strcmp(t->entries[pos].name, name) < 0) {
        pos++;
    }
    for (int i = t->size; i > pos; i--) {
        t->entries[i] = t->entries[i - 1];
    }
    memcpy(t->entries[pos].name, name, len + 1);
    t->entries[pos].count = 1;
    t->size++;

    return 1;
}

int tally_count(const tally_t *t, const char *name)
{
    if (t == NULL || name == NULL) {
        return 0;
    }

    for (int i = 0; i < t->size; i++) {
        if (strcmp(t->entries[i].name, name) == 0) {
            return t->entries[i].count;
        }
    }

    return 0;
}

int tally_size(const tally_t *t)
{
    if (t == NULL) {
        return 0;
    }

    return t->size;
}

void tally_print(const tally_t *t, FILE *out)
{
    if (t == NULL || out == NULL) {
        return;
    }

    for (int i = 0; i < t->size; i++) {
        fprintf(out, "%s %d\n",
                t->entries[i].name,
                t->entries[i].count);
    }
}
