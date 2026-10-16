#ifndef PTR_H
#define PTR_H

/*
 * Swaps the values pointed to by a and b.
 * Assumes a and b point to valid float objects.
 */
void swap_float(float *a, float *b);

/*
 * Finds the minimum and maximum values in an array.
 * Stores the results in *min and *max.
 * Returns 0 on success, or -1 if n < 1 or any pointer is NULL.
 * Assumes a points to at least n float elements when n >= 1.
 */
int minmax(const float *a, int n, float *min, float *max);

/*
 * Returns a pointer to the first element greater than threshold.
 * Returns NULL if no element matches.
 * Assumes a points to at least n float elements when n > 0.
 */
const float *find_above(const float *a, int n, float threshold);

/*
 * Reverses the array in place.
 * Does nothing if a is NULL or n < 2.
 * Assumes a points to at least n float elements when n >= 2.
 */
void reverse(float *a, int n);

#endif
