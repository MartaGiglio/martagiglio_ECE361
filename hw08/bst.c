#include "bst.h"
#include <stdlib.h>

/* Represents one node in the binary search tree. */
struct node {
    int data;
    struct node *leftChild;
    struct node *rightChild;
};

/* Represents the binary search tree. */
struct bst {
    struct node *root;
};

/* Creates a new empty tree.
 * Based on the original program's root initialization.
 */
bst_t *bst_create(void) {
    bst_t *t = malloc(sizeof(bst_t));

    if (t == NULL) {
        return NULL;
    }

    t->root = NULL;

    return t;
}

/* Inserts a key into the tree.
 * Based on the original insert() function.
 */
int bst_insert(bst_t *t, int key) {
    if (t == NULL) {
        return -1;
    }

    struct node *tempNode = malloc(sizeof(struct node));

    if (tempNode == NULL) {
        return -1;
    }

    tempNode->data = key;
    tempNode->leftChild = NULL;
    tempNode->rightChild = NULL;

    if (t->root == NULL) {
        t->root = tempNode;
        return 0;
    }

    struct node *current = t->root;
    struct node *parent = NULL;

    while (current != NULL) {
        parent = current;

        if (key < current->data) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }

    if (key < parent->data) {
        parent->leftChild = tempNode;
    } else {
        parent->rightChild = tempNode;
    }

    return 0;
}

/* Frees all nodes in a subtree.
 * Takes a pointer to the subtree root.
 * Returns nothing. Assumes nodes belong to a valid BST.
 */
static void destroy_nodes(struct node *current) {
    if (current == NULL) {
        return;
    }

    destroy_nodes(current->leftChild);
    destroy_nodes(current->rightChild);

    free(current);
}

/* Destroys the tree.
 * Takes a pointer to the tree.
 * Returns nothing. Does nothing if t is NULL.
 */
void bst_destroy(bst_t *t) {
    if (t == NULL) {
        return;
    }

    destroy_nodes(t->root);
    free(t);
}

/* Searches for a key in the tree.
 * Based on the original search() function, with the NULL pointer bug fixed.
 */
bool bst_contains(const bst_t *t, int key, size_t *visits) {
    if (visits != NULL) {
        *visits = 0;
    }

    if (t == NULL) {
        return false;
    }

    const struct node *current = t->root;

    while (current != NULL) {
        if (visits != NULL) {
            (*visits)++;
        }

        if (current->data == key) {
            return true;
        }

        if (key < current->data) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }

    return false;
}

/* Counts the nodes in a subtree.
 * Takes a pointer to the subtree root.
 * Returns the total number of nodes.
 */
static size_t count_nodes(const struct node *current) {
    if (current == NULL) {
        return 0;
    }

    return 1 + count_nodes(current->leftChild)
             + count_nodes(current->rightChild);
}

/* Returns the number of keys in the tree.
 * Takes a pointer to the tree.
 * Returns 0 if t is NULL.
 */
size_t bst_size(const bst_t *t) {
    if (t == NULL) {
        return 0;
    }

    return count_nodes(t->root);
}

/* Calculates the height of a subtree.
 * Takes a pointer to the subtree root.
 * Returns the number of nodes on the longest path.
 */
static size_t tree_height(const struct node *current) {
    if (current == NULL) {
        return 0;
    }

    size_t left_height = tree_height(current->leftChild);
    size_t right_height = tree_height(current->rightChild);

    if (left_height > right_height) {
        return 1 + left_height;
    }

    return 1 + right_height;
}

/* Returns the height of the tree.
 * Takes a pointer to the tree.
 * Returns 0 if t is NULL.
 */
size_t bst_height(const bst_t *t) {
    if (t == NULL) {
        return 0;
    }

    return tree_height(t->root);
}
