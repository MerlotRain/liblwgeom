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

#include <mathse.h>

#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct SEGeom_t se_geom;

struct buf;

/* memory table one column data type */
typedef struct se_table_field_t {
    /* column type, t: byte, b: blob */
    char type;
    /* When the type is byte, basic types such as char, short, int, long long,
     * and char* are supported. At this time, the data segment calculates the
     * memory size that needs to be allocated based on size. When the type is
     * blob, there is no need to pass in the size value. A pointer-sized space
     * will be created internally to record the pointer of the se_datablob_t to
     * obtain the real data.
     */
    int size;
    char name[36];
} se_table_field_t;

/* memory table */
typedef struct SETable_t se_table;

/* Create memory table
 * fields memory table field
 * size memory table row size
 * flag memory table flag
        0: create OID and geometry field
        1: do not create OID and geometry field
*/
EXTERN se_table *create_mem_table(se_table_field_t *fields, uint32_t size,
                                  int flag);
EXTERN void destroy_mem_table(se_table *tab);

/* ------------------------------ read function -----------------------------
 */
EXTERN uint32_t find_column_by_name(se_table *tab, const char *name);
EXTERN se_table_field_t *table_fields(se_table *tab);
EXTERN uint64_t table_rows(se_table *tab);
EXTERN uint32_t table_column(se_table *tab);

EXTERN void reset_cursor(se_table *tab);
EXTERN char *table_next(se_table *tab);
EXTERN char *get_row(se_table *tab, int64_t id);

EXTERN se_geom *read_geom(se_table *tab, int64_t id);

EXTERN int8_t read_cell_i8(se_table *tab, int64_t id, int32_t column);
EXTERN uint8_t read_cell_u8(se_table *tab, int64_t id, int32_t column);
EXTERN int16_t read_cell_i16(se_table *tab, int64_t id, int32_t column);
EXTERN uint16_t read_cell_u16(se_table *tab, int64_t id, int32_t column);
EXTERN int32_t read_cell_i32(se_table *tab, int64_t id, int32_t column);
EXTERN uint32_t read_cell_u32(se_table *tab, int64_t id, int32_t column);
EXTERN int64_t read_cell_i64(se_table *tab, int64_t id, int32_t column);
EXTERN uint64_t read_cell_u64(se_table *tab, int64_t id, int32_t column);

EXTERN double read_cell_double(se_table *tab, int64_t id, int32_t column);
EXTERN float read_cell_float(se_table *tab, int64_t id, int32_t column);

EXTERN char *read_cell_str(se_table *tab, int64_t id, int32_t column);
EXTERN struct buf *read_cell_blob(se_table *tab, int64_t id, int32_t column);

/* ----------------------------- write function -----------------------------
 */
EXTERN void create_spr_index(se_table *tab);

EXTERN char *add_row(se_table *tab);
EXTERN void set_row_byte_data(se_table *tab, int64_t id, int32_t column,
                              char *data);
EXTERN void set_row_blob_data(se_table *tab, int64_t id, int32_t column,
                              struct buf *data);
EXTERN void set_coordinate(se_table *tab, int64_t id);

EXTERN void remove_row(se_table *tab, int64_t id);
EXTERN void remove_range(se_table *tab, int64_t *id, uint32_t num);

#ifdef __cplusplus
}
#endif

#endif