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

#define BITSET_STATE_NONE 0 ///< All bits are false
#define BITSET_STATE_ALL  1 ///< All bits are true
#define BITSET_STATE_ANY  2 ///< Some bits are true

struct bitset;

/// @brief Create a new bitset.
/// @param size The size of the bitset.
/// @return Generate an 8-bit aligned bitset
EXTERN struct bitset *bitset_new(size_t size);

/// @brief Free a bitset.
/// @param bs The bitset to free.
EXTERN void bitset_free(struct bitset *bs);

/// @brief Set the \a index bit to true
/// @param bs The bitset
/// @param index The index of the bit
/// @return
EXTERN void bitset_set(struct bitset *bs, size_t index);

/// @brief Clear the \a index bit to false
/// @param bs The bitset
/// @param index The index of the bit
/// @return
EXTERN void bitset_clear(struct bitset *bs, size_t index);

/// @brief Test if the \a index bit is true or false
/// @param bs The bitset
/// @param index The index of the bit
EXTERN bool bitset_test(struct bitset *bs, size_t index);

/// @brief Flip the \a index bit
/// @param bs The bitset
/// @param index The index of the bit
/// @return
EXTERN void bitset_flip(struct bitset *bs, size_t index);

/// @brief Get the state of the bitset
/// @param bs The bitset
/// @return The return value is a BITSET_STATE_* series macro
EXTERN int bitset_state(struct bitset *bs);

/// @brief Count the number of bits set to true
/// @param bs The bitset
/// @return The number of bits set to true
EXTERN size_t bitset_count(struct bitset *bs);

/// @brief Get the size of the bitset
/// @param bs The bitset
/// @return An 8-bit aligned value
EXTERN size_t bitset_size(struct bitset *bs);

#ifdef __cplusplus
}
#endif

#endif