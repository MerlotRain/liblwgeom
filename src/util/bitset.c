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
