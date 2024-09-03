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

struct nv__bitset {
    size_t length;
    size_t capacity;
    char data[];
};

/// @brief Create a new bitset.
/// @param size The size of the bitset.
/// @return Generate an 8-bit aligned bitset
struct nv__bitset *nv__bitset_new(size_t size)
{
    size_t cap = 0;
    if (size == 0)
        cap = 8;
    else {
        cap = (size + 7) & ~7; // round up to multiple of 8
    }

    struct nv__bitset *bs =
        (struct nv__bitset *)nv__malloc(sizeof(struct nv__bitset) + cap / 8);
    if (bs == NULL) {
        return NULL;
    }
    memset(bs, 0, sizeof(struct nv__bitset) + cap / 8);
    bs->length = size;
    bs->capacity = cap;
    return bs;
}

/// @brief Free a bitset.
/// @param bs The bitset to free.
void nv__bitset_free(struct nv__bitset *bs)
{
    nv__free(bs);
}

/// @brief Set the \a index bit to NV_TRUE
/// @param bs The bitset
/// @param index The index of the bit
/// @return
void nv__bitset_set(struct nv__bitset *bs, size_t index)
{
    if (index >= bs->length) {
        return;
    }

    bs->data[index / 8] |= (1 << (index % 8)); // set bit
}

/// @brief Clear the \a index bit to NV_FALSE
/// @param bs The bitset
/// @param index The index of the bit
/// @return
void nv__bitset_clear(struct nv__bitset *bs, size_t index)
{
    if (index >= bs->length) {
        return;
    }

    bs->data[index / 8] &= ~(1 << (index % 8)); // clear bit
}

/// @brief Test if the \a index bit is NV_TRUE or NV_FALSE
/// @param bs The bitset
/// @param index The index of the bit
int nv__bitset_test(struct nv__bitset *bs, size_t index)
{
    if (index >= bs->length) {
        return NV_FALSE;
    }

    return bs->data[index / 8] & (char)(1 << (index % 8)); // test bit
}

/// @brief Flip the \a index bit
/// @param bs The bitset
/// @param index The index of the bit
/// @return
void nv__bitset_flip(struct nv__bitset *bs, size_t index)
{
    if (index >= bs->length) {
        return;
    }

    bs->data[index / 8] ^= (1 << (index % 8));
}

/// @brief Get the state of the bitset
/// @param bs The bitset
/// @return The return value is a BITSET_STATE_* series macro
int nv__bitset_state(struct nv__bitset *bs)
{
    size_t c = nv__bitset_count(bs);
    if (c == 0) {
        return NV__BITSET_STATE_NONE;
    }
    else if (c == bs->length) {
        return NV__BITSET_STATE_ALL;
    }
    else {
        return NV__BITSET_STATE_ANY;
    }
}

/// @brief Count the number of bits set to NV_TRUE
/// @param bs The bitset
/// @return The number of bits set to NV_TRUE
size_t nv__bitset_count(struct nv__bitset *bs)
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

/// @brief Get the size of the bitset
/// @param bs The bitset
/// @return An 8-bit aligned value
size_t nv__bitset_size(struct nv__bitset *bs)
{
    return bs->length;
}
