#include "bits.h"
#include <stdio.h>
#include <stdint.h>

void print_binary(uint32_t x, int width) {
    for (int i = width - 1; i >= 0; i--) {
        printf("%d", (x >> i) & 1);

        if (i > 0 && i % 4 == 0) {
            printf(" ");
        }
    }

    printf("\n");
}

uint32_t get_field(uint32_t word, int pos, int width) {
    if (pos < 0 || width <= 0 || width > 32 || pos + width > 32) {
        return 0;
    }

    uint32_t mask;

    if (width == 32) {
        mask = UINT32_MAX;
    } else {
        mask = (1u << width) - 1u;
    }

    return (word >> pos) & mask;
}

uint32_t set_field(uint32_t word, int pos, int width, uint32_t value) {
    if (pos < 0 || width <= 0 || width > 32 || pos + width > 32) {
        return word;
    }

    uint32_t field_mask;

    if (width == 32) {
        field_mask = UINT32_MAX;
    } else {
        field_mask = (1u << width) - 1u;
    }

    uint32_t mask = field_mask << pos;

    word &= ~mask;
    word |= (value & field_mask) << pos;

    return word;
}

int32_t sign_extend(uint32_t value, int width) {
    if (width <= 0 || width > 32) {
        return 0;
    }

    if (width == 32) {
        return (int32_t)value;
    }

    uint32_t mask = (1u << width) - 1u;
    uint32_t result = value & mask;
    uint32_t sign_bit = 1u << (width - 1);

    if (result & sign_bit) {
        result |= ~mask;
    }

    return (int32_t)result;
}