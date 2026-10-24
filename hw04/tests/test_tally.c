#include "../tally.h"

#include <stdio.h>
#include <string.h>

static int tests_run = 0;
static int tests_failed = 0;

static void check(const char *description, int condition)
{
    tests_run++;

    if (condition) {
        printf("PASS: %s\n", description);
    } else {
        printf("FAIL: %s\n", description);
        tests_failed++;
    }
}

int main(void)
{
    tally_t *t = tally_create();

    check("create tally", t != NULL);

    if (t == NULL) {
        printf("Tests run: %d, failed: %d\n",
               tests_run, tests_failed);
        return 1;
    }

    check("new tally is empty", tally_size(t) == 0);
    check("missing name has count zero",
          tally_count(t, "missing") == 0);
    check("NULL tally has size zero", tally_size(NULL) == 0);
    check("NULL name has count zero",
          tally_count(t, NULL) == 0);

    check("first apple has count one",
          tally_add(t, "apple") == 1);
    check("second apple has count two",
          tally_add(t, "apple") == 2);
    check("apple count is two",
          tally_count(t, "apple") == 2);
    check("one distinct name", tally_size(t) == 1);

    tally_t *full = tally_create();

    check("create tally for full-capacity test", full != NULL);

    if (full != NULL) {
        char name[16];

        for (int i = 0; i < TALLY_NAMES; i++) {
            snprintf(name, sizeof(name), "n%d", i);
            tally_add(full, name);
        }

        check("tally accepts 16 distinct names",
              tally_size(full) == TALLY_NAMES);

        check("tally rejects a new name when full",
              tally_add(full, "newname") == -1);

        check("existing name can be incremented when full",
              tally_add(full, "n0") == 2);

        tally_destroy(full);
    }

    check("reject empty name", tally_add(t, "") == -1);
    check("reject NULL name", tally_add(t, NULL) == -1);
    check("reject NULL tally", tally_add(NULL, "apple") == -1);

    check("accept 15-character name",
          tally_add(t, "abcdefghijklmno") == 1);
    check("reject 16-character name",
          tally_add(t, "abcdefghijklmnop") == -1);

    tally_destroy(t);
    check("destroy NULL is safe", (tally_destroy(NULL), 1));

    t = tally_create();
    check("create after destroy", t != NULL);

    if (t != NULL) {
        check("recreated tally is empty", tally_size(t) == 0);
        tally_destroy(t);
    }

    tally_t *a = tally_create();
    tally_t *b = tally_create();
    tally_t *c = tally_create();
    tally_t *d = tally_create();
    tally_t *e = tally_create();

    check("four tallies can be created",
          a != NULL && b != NULL && c != NULL && d != NULL);
    check("fifth tally is refused", e == NULL);

    if (a != NULL) {
        check("add to first tally", tally_add(a, "first") == 1);
    }

    if (b != NULL) {
        check("tallies are independent", tally_size(b) == 0);
    }

    if (a != NULL) tally_destroy(a);
    if (b != NULL) tally_destroy(b);
    if (c != NULL) tally_destroy(c);
    if (d != NULL) tally_destroy(d);

    t = tally_create();

    if (t != NULL) {
        tally_add(t, "window");
        tally_add(t, "alarm");
        tally_add(t, "door");

        FILE *out = tmpfile();

        if (out != NULL) {
            tally_print(t, out);
            rewind(out);

            char output[128] = "";
            size_t used = 0;
            int ch;

            while ((ch = fgetc(out)) != EOF &&
                   used < sizeof(output) - 1) {
                output[used++] = (char)ch;
            }

            output[used] = '\0';

            check("print names in alphabetical order",
                  strcmp(output,
                         "alarm 1\ndoor 1\nwindow 1\n") == 0);

            fclose(out);
        } else {
            check("create temporary output stream", 0);
        }

        tally_destroy(t);
    } else {
        check("create tally for print test", 0);
    }

    printf("\nTests run: %d, failed: %d\n",
           tests_run, tests_failed);

    return tests_failed == 0 ? 0 : 1;
}

