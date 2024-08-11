/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include "bitset.h"

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

    struct bitset *bs = malloc(sizeof(struct bitset) + cap / 8);
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
    free(bs);
}

bool bitset_set(struct bitset *bs, size_t index, bool value)
{
    if (index >= bs->length) {
        return false;
    }

    bs->data[index / 8] |= (char)(value ? 1 << (index % 8) : 0); // set bit
    return value;
}

bool bitset_test(struct bitset *bs, size_t index)
{
    if (index >= bs->length) {
        return false;
    }

    return bs->data[index / 8] & (char)(1 << (index % 8)); // test bit
}

bool bitset_flip(struct bitset *bs, size_t index)
{
    if (index >= bs->length) {
        return false;
    }

    bs->data[index / 8] ^= (char)(1 << (index % 8)); // flip bit
    return false;
}

int bitset_state(struct bitset *bs)
{
    if (bs->length == 0) {
        return 0;
    }
    return 0;
}

size_t bitset_count(struct bitset *bs)
{
    return 0;
}

size_t bitset_size(struct bitset *bs)
{
    return 0;
}
