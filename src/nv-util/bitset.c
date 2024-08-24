/**
 * Copyright (c) 2023-present Merlot.Rain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "bitset.h"
#include <string.h>
#include <nv-common.h>

struct bitset {
    size_t length;
    size_t capacity;
    char data[];
};

struct bitset *bitset_new(size_t size)
{
    size_t cap = 0;
    if (size == 0)
        cap = 8;
    else {
        cap = (size + 7) & ~7; // round up to multiple of 8
    }

    struct bitset *bs =
        (struct bitset *)nv__malloc(sizeof(struct bitset) + cap / 8);
    if (bs == NULL) {
        return NULL;
    }
    memset(bs, 0, sizeof(struct bitset) + cap / 8);
    bs->length = size;
    bs->capacity = cap;
    return bs;
}

void bitset_free(struct bitset *bs)
{
    nv__free(bs);
}

void bitset_set(struct bitset *bs, size_t index)
{
    if (index >= bs->length) {
        return;
    }

    bs->data[index / 8] |= (1 << (index % 8)); // set bit
}

void bitset_clear(struct bitset *bs, size_t index)
{
    if (index >= bs->length) {
        return;
    }

    bs->data[index / 8] &= ~(1 << (index % 8)); // clear bit
}

bool bitset_test(struct bitset *bs, size_t index)
{
    if (index >= bs->length) {
        return false;
    }

    return bs->data[index / 8] & (char)(1 << (index % 8)); // test bit
}

void bitset_flip(struct bitset *bs, size_t index)
{
    if (index >= bs->length) {
        return;
    }

    bs->data[index / 8] ^= (1 << (index % 8));
}

int bitset_state(struct bitset *bs)
{
    size_t c = bitset_count(bs);
    if (c == 0) {
        return BITSET_STATE_NONE;
    }
    else if (c == bs->length) {
        return BITSET_STATE_ALL;
    }
    else {
        return BITSET_STATE_ANY;
    }
}

size_t bitset_count(struct bitset *bs)
{
    int count = 0;
    for (int i = 0; i < (bs->length / 8) + (bs->length % 8 ? 1 : 0); i++) {
        unsigned char byte = bs->data[i];
        while (byte) {
            byte &= (byte - 1);
            count++;
        }
    }
    return count;
}

size_t bitset_size(struct bitset *bs)
{
    return bs->length;
}
