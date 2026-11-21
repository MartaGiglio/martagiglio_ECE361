#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

int main(void) {
    bst_t *tree = bst_create();

    if (tree == NULL) {
        return 1;
    }

    int key;
    size_t count = 0;
    int result;

    while ((result = scanf("%d", &key)) == 1) {
        if (bst_insert(tree, key) == -1) {
            bst_destroy(tree);
            return 1;
        }

        count++;
    }

    if (result == 0) {
        printf("not a number after %zu values\n", count);
        bst_destroy(tree);
        return 1;
    }

    if (count == 0) {
        printf("empty tree\n");
    } else {
        printf("n %zu, height %zu\n",
               bst_size(tree), bst_height(tree));
    }

    bst_destroy(tree);
    return 0;
}
