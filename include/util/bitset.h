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

#ifndef BITSET_H
#define BITSET_H

#include <mathse.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BITSET_STATE_NONE 0
#define BITSET_STATE_ALL  1
#define BITSET_STATE_ANY  2

struct bitset;

EXTERN struct bitset *bitset_new(size_t size);

EXTERN void bitset_free(struct bitset *bs);

EXTERN bool bitset_set(struct bitset *bs, size_t index, bool value);

EXTERN bool bitset_test(struct bitset *bs, size_t index);

EXTERN bool bitset_flip(struct bitset *bs, size_t index);

EXTERN int bitset_state(struct bitset *bs);

EXTERN size_t bitset_count(struct bitset *bs);

EXTERN size_t bitset_size(struct bitset *bs);

#ifdef __cplusplus
}
#endif

#endif