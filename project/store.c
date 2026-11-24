#include "store.h"

#include <stdlib.h>

/* Represents one node in the AVL tree. */
struct store_node {
    record_t record;
    struct store_node *left;
    struct store_node *right;
    int height;
};

/* Represents the record store. */
struct store {
    struct store_node *root;
    size_t count;
};

/* Compares two records using their timestamps. */
int record_compare(const record_t *a, const record_t *b) {
    if (a->reading.timestamp < b->reading.timestamp) {
        return -1;
    }

    if (a->reading.timestamp > b->reading.timestamp) {
        return 1;
    }

    return 0;
}

/* Creates an empty record store. */
store_t *store_create(void) {
    store_t *s = malloc(sizeof(store_t));

    if (s == NULL) {
        return NULL;
    }

    s->root = NULL;
    s->count = 0;

    return s;
}

/* Returns the height of a node. */
static int node_height(const struct store_node *node) {
    if (node == NULL) {
        return 0;
    }

    return node->height;
}

/* Returns the larger of two integers. */
static int max(int a, int b) {
    return (a > b) ? a : b;
}

/* Updates the height of a node. */
static void update_height(struct store_node *node) {
    node->height = 1 + max(node_height(node->left),
                           node_height(node->right));
}

/* Returns the balance factor of a node. */
static int balance_factor(const struct store_node *node) {
    if (node == NULL) {
        return 0;
    }

    return node_height(node->left) - node_height(node->right);
}

/* Performs a right rotation. */
static struct store_node *rotate_right(struct store_node *y) {
    struct store_node *x = y->left;
    struct store_node *temp = x->right;

    x->right = y;
    y->left = temp;

    update_height(y);
    update_height(x);

    return x;
}

/* Performs a left rotation. */
static struct store_node *rotate_left(struct store_node *x) {
    struct store_node *y = x->right;
    struct store_node *temp = y->left;

    y->left = x;
    x->right = temp;

    update_height(x);
    update_height(y);

    return y;
}

/* Inserts a record into an AVL subtree. */
static int insert_node(struct store_node **root,
                       const record_t *record) {
    if (*root == NULL) {
        struct store_node *new_node = malloc(sizeof(struct store_node));

        if (new_node == NULL) {
            return -1;
        }

        new_node->record = *record;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->height = 1;

        *root = new_node;
        return 0;
    }

    int comparison = record_compare(record, &(*root)->record);

    if (comparison < 0) {
        if (insert_node(&(*root)->left, record) != 0) {
            return -1;
        }
    } else if (comparison > 0) {
        if (insert_node(&(*root)->right, record) != 0) {
            return -1;
        }
    } else {
        return -1;
    }

    update_height(*root);

    int balance = balance_factor(*root);

    /* Left-Left case. */
    if (balance > 1 &&
        record_compare(record, &(*root)->left->record) < 0) {
        *root = rotate_right(*root);
    }

    /* Right-Right case. */
    else if (balance < -1 &&
             record_compare(record, &(*root)->right->record) > 0) {
        *root = rotate_left(*root);
    }

    /* Left-Right case. */
    else if (balance > 1 &&
             record_compare(record, &(*root)->left->record) > 0) {
        (*root)->left = rotate_left((*root)->left);
        *root = rotate_right(*root);
    }

    /* Right-Left case. */
    else if (balance < -1 &&
             record_compare(record, &(*root)->right->record) < 0) {
        (*root)->right = rotate_right((*root)->right);
        *root = rotate_left(*root);
    }

    return 0;
}

/* Inserts a record into the store. */
int store_insert(store_t *s, const record_t *r) {
    if (s == NULL || r == NULL) {
        return -1;
    }

    if (insert_node(&s->root, r) != 0) {
        return -1;
    }

    s->count++;

    return 0;
}

/* Finds a record using its timestamp. */
const record_t *store_find(const store_t *s, time_t timestamp) {
    if (s == NULL) {
        return NULL;
    }

    const struct store_node *current = s->root;

    while (current != NULL) {
        if (timestamp == current->record.reading.timestamp) {
            return &current->record;
        }

        if (timestamp < current->record.reading.timestamp) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return NULL;
}

/* Visits all records in chronological order. */
static void walk_nodes(const struct store_node *current,
                       void (*callback)(const record_t *, void *),
                       void *ctx) {
    if (current == NULL) {
        return;
    }

    walk_nodes(current->left, callback, ctx);

    callback(&current->record, ctx);

    walk_nodes(current->right, callback, ctx);
}

/* Walks through all records in the store. */
void store_walk(const store_t *s,
                void (*callback)(const record_t *, void *),
                void *ctx) {
    if (s == NULL || callback == NULL) {
        return;
    }

    walk_nodes(s->root, callback, ctx);
}

/* Returns the number of records in the store. */
size_t store_count(const store_t *s) {
    if (s == NULL) {
        return 0;
    }

    return s->count;
}

/* Returns the height of the store. */
int store_height(const store_t *s) {
    if (s == NULL) {
        return 0;
    }

    return node_height(s->root);
}

/* Frees all nodes in a subtree. */
static void destroy_nodes(struct store_node *current) {
    if (current == NULL) {
        return;
    }

    destroy_nodes(current->left);
    destroy_nodes(current->right);

    free(current);
}

/* Destroys the record store. */
void store_destroy(store_t *s) {
    if (s == NULL) {
        return;
    }

    destroy_nodes(s->root);
    free(s);
}
