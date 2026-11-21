#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stddef.h>

/* Opaque type: the internal structure is defined in bst.c */
typedef struct bst bst_t;

/* Creates an empty tree.
 * Takes no arguments.
 * Returns a pointer to the new tree, or NULL if allocation fails.
 */
bst_t *bst_create(void);

/* Destroys the tree and frees all allocated memory.
 * Takes a pointer to the tree.
 * Returns nothing. Does nothing if t is NULL.
 */
void bst_destroy(bst_t *t);

/* Inserts a key into the tree.
 * Takes a pointer to the tree and an integer key.
 * Returns 0 on success, or -1 if t is NULL or allocation fails.
 */
int bst_insert(bst_t *t, int key);

/* Searches for a key in the tree.
 * Takes a pointer to a constant tree, a key, and an optional visits pointer.
 * Returns true if found, false otherwise.
 * If visits is not NULL, stores the number of nodes compared.
 */
bool bst_contains(const bst_t *t, int key, size_t *visits);

/* Returns the number of keys in the tree.
 * Takes a pointer to the tree.
 * Returns 0 if t is NULL.
 */
size_t bst_size(const bst_t *t);

/* Returns the height of the tree.
 * Takes a pointer to the tree.
 * Returns 0 if t is NULL or the tree is empty.
 */
size_t bst_height(const bst_t *t);

#endif
