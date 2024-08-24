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

#ifndef BITSET_H
#define BITSET_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BITSET_STATE_NONE 0 ///< All bits are false
#define BITSET_STATE_ALL  1 ///< All bits are true
#define BITSET_STATE_ANY  2 ///< Some bits are true

struct bitset;

/// @brief Create a new bitset.
/// @param size The size of the bitset.
/// @return Generate an 8-bit aligned bitset
struct bitset *bitset_new(size_t size);

/// @brief Free a bitset.
/// @param bs The bitset to free.
void bitset_free(struct bitset *bs);

/// @brief Set the \a index bit to true
/// @param bs The bitset
/// @param index The index of the bit
/// @return
void bitset_set(struct bitset *bs, size_t index);

/// @brief Clear the \a index bit to false
/// @param bs The bitset
/// @param index The index of the bit
/// @return
void bitset_clear(struct bitset *bs, size_t index);

/// @brief Test if the \a index bit is true or false
/// @param bs The bitset
/// @param index The index of the bit
bool bitset_test(struct bitset *bs, size_t index);

/// @brief Flip the \a index bit
/// @param bs The bitset
/// @param index The index of the bit
/// @return
void bitset_flip(struct bitset *bs, size_t index);

/// @brief Get the state of the bitset
/// @param bs The bitset
/// @return The return value is a BITSET_STATE_* series macro
int bitset_state(struct bitset *bs);

/// @brief Count the number of bits set to true
/// @param bs The bitset
/// @return The number of bits set to true
size_t bitset_count(struct bitset *bs);

/// @brief Get the size of the bitset
/// @param bs The bitset
/// @return An 8-bit aligned value
size_t bitset_size(struct bitset *bs);

#ifdef __cplusplus
}
#endif

#endif