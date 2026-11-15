#ifndef ISTACK_H
#define ISTACK_H

#include <stdbool.h>
#include <stddef.h>

#define ISTACK_START 8

typedef struct istack istack_t;

/*
 * Creates an empty stack with initial capacity ISTACK_START.
 * Returns a pointer to the stack, or NULL if allocation fails.
 */
istack_t *istack_create(void);

/*
 * Frees the stack and all memory it owns.
 * Assumes s is a valid stack pointer or NULL.
 */
void istack_destroy(istack_t *s);

/*
 * Pushes value v onto the stack.
 * Returns 0 on success, -1 if s is NULL or allocation fails.
 */
int istack_push(istack_t *s, int v);

/*
 * Removes the top value and stores it in *v if v is not NULL.
 * Returns 0 on success, -1 if s is NULL or empty.
 */
int istack_pop(istack_t *s, int *v);

/*
 * Reads the top value into *v if v is not NULL.
 * Does not remove the value.
 * Returns 0 on success, -1 if s is NULL or empty.
 */
int istack_peek(const istack_t *s, int *v);

/*
 * Returns the number of values in the stack.
 * Returns 0 if s is NULL.
 */
size_t istack_count(const istack_t *s);

/*
 * Returns true if the stack is empty or s is NULL.
 */
bool istack_is_empty(const istack_t *s);

/*
 * Returns the current capacity of the stack.
 * Returns 0 if s is NULL.
 */
size_t istack_capacity(const istack_t *s);

/*
 * Creates a copy of the stack values in bottom-to-top order.
 * Stores the number of elements in *n.
 * Returns NULL if s is NULL, empty, or allocation fails.
 * The caller owns the returned array and must free it.
 */
int *istack_to_array(const istack_t *s, size_t *n);

#endif
