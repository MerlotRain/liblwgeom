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

#ifndef SORT_H
#define SORT_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "mathse.h"

/**
 * @brief Use quick sort algorithm to implement array sorting function。
 *
 * @param p the array begin pointer
 * @param ele_count number of elements
 * @param ele_size  size of one element
 * @param compare compare function to compare elements
 * @param user_data user data
 */
void quick_sort(const void *p, size_t ele_count, size_t ele_size,
                int (*compare)(const void *, const void *), void *user_data);

#ifdef __cpluscplus
}
#endif

#endif