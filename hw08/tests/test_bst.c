#include <stdio.h>
#include <stdlib.h>
#include "../bst.h"

static int failures = 0;

static void check(int condition, const char *name) {
    if (condition) {
        printf("PASS: %s\n", name);
    } else {
        printf("FAIL: %s\n", name);
        failures++;
    }
}

int main(void) {
    bst_t *tree = bst_create();

    check(tree != NULL, "Create tree");

    if (tree == NULL) {
        return 1;
    }

    check(bst_size(tree) == 0, "Empty tree size");
    check(bst_height(tree) == 0, "Empty tree height");
size_t empty_visits = 0;
check(bst_contains(tree, 10, &empty_visits) == false,
      "Search in empty tree");
check(empty_visits == 0, "Empty tree search visits");

    int keys[] = {27, 14, 35, 10, 19, 31, 42};

    for (size_t i = 0; i < 7; i++) {
        check(bst_insert(tree, keys[i]) == 0, "Insert original key");
    }

    check(bst_size(tree) == 7, "Original tree size");

    check(bst_height(tree) == 3, "Original tree height");

    size_t visits = 0;

    check(bst_contains(tree, 31, &visits) == true,
          "Find 31");

    check(visits == 3, "Find 31 in 3 visits");

    check(bst_contains(tree, 15, &visits) == false,
          "15 not found");

    check(visits == 3, "15 not found in 3 visits");

    bst_destroy(tree);
tree = NULL;
check(tree == NULL, "Destroy full tree");
    /* Duplicate test */
    tree = bst_create();

    bst_insert(tree, 10);
    bst_insert(tree, 10);

    check(bst_size(tree) == 2, "Duplicate is kept");

    bst_destroy(tree);

    /* Increasing order test */
    tree = bst_create();

    for (int i = 1; i <= 10; i++) {
        bst_insert(tree, i);
    }

    check(bst_height(tree) == 10,
          "Increasing order gives height 10");

    bst_destroy(tree);

    printf("\n");

    if (failures == 0) {
        printf("All tests passed.\n");
        return 0;
    }

    printf("%d test(s) failed.\n", failures);
    return 1;
}
