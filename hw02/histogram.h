#ifndef HISTOGRAM_H
#define HISTOGRAM_H

/*
 * Print a histogram of the temperatures in 5-degree Celsius bins.
 * Values below 0 go in the first bin; values at or above 100 go
 * in the last bin. Assumes n >= 0.
 */
void histogram_print(const float temps[], int n);

#endif
