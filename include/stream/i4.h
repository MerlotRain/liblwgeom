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

#ifndef I4_H
#define I4_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mathse.h>

/// se_i4 is an attribute object, which consists of two parts, one is the
/// object pointer, and the second is the object's attribute information.
/// These attribute information are not possessed by the object itself, such
/// as polygon area, polygon enclosing rectangle, etc. Rather, they are some
/// markup attributes, such as the ID of the polygon, and the correspondence
/// between the polygon and a certain field attribute. These properties are
/// passed externally, and we agree that the passed-in value can only be a
/// simple int value, and the meaning of the value is determined externally.
/// After completing a set of object operations, these attribute values
/// still exist in the object, and the object can be obtained by obtaining
/// the attribute values.
typedef struct SEi4_t i4_t;

/// @brief new i4 object
/// @param p
/// @param prop_size
/// @return
i4_t *i4_new(const void *p, size_t prop_size);

/// @brief destory i4 object
/// @param p
void free_i4(i4_t *p);

/// @brief Return prop pointer
/// @param p
/// @return
const void *i4_ptr(i4_t *p);

/// @brief Set prop value
/// @param p
/// @param index
/// @param prop
void set_i4_prop(i4_t *p, size_t index, int prop);

/// @brief Get prop value
/// @param p
/// @param index
/// @return
int get_i4_prop(i4_t *p, size_t index);
/// @brief Return prop size
/// @param p
/// @return
int i4_prop_size(i4_t *p);

#ifdef __cplusplus
}
#endif

#endif