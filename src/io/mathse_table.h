#ifndef __MATHSE_TABLE_H__
#define __MATHSE_TABLE_H__

/* memory table one column data type */
typedef struct se_table_field_t
{
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

/* blob or clob column data */
typedef struct se_datablob_t
{
  uint64_t bs; /* blob size */
  char *data;  /* blob header pointer */
} se_datablob_t;

/* memory table */
typedef struct se_mem_table_t se_mem_table_t;

/* Create memory table
 * fields memory table field
 * size memory table row size
 * flag memory table flag
        0: create OID and geometry field
        1: do not create OID and geometry field
*/
SE_API se_mem_table_t *create_mem_table (se_table_field_t *fields,
                                         uint32_t size, int flag);
SE_API void destroy_mem_table (se_mem_table_t *tab);

/* ------------------------------ read function -----------------------------
 */
SE_API uint32_t find_column_by_name (se_mem_table_t *tab, const char *name);
SE_API se_table_field_t *table_fields (se_mem_table_t *tab);
SE_API uint64_t table_rows (se_mem_table_t *tab);
SE_API uint32_t table_column (se_mem_table_t *tab);

SE_API void reset_cursor (se_mem_table_t *tab);
SE_API char *table_next (se_mem_table_t *tab);
SE_API char *get_row (se_mem_table_t *tab, int64_t id);

SE_API geom_coordinate_t *read_coordinate (se_mem_table_t *tab, int64_t id);

SE_API int8_t read_cell_i8 (se_mem_table_t *tab, int64_t id, int32_t column);
SE_API uint8_t read_cell_u8 (se_mem_table_t *tab, int64_t id, int32_t column);
SE_API int16_t read_cell_i16 (se_mem_table_t *tab, int64_t id, int32_t column);
SE_API uint16_t read_cell_u16 (se_mem_table_t *tab, int64_t id,
                               int32_t column);
SE_API int32_t read_cell_i32 (se_mem_table_t *tab, int64_t id, int32_t column);
SE_API uint32_t read_cell_u32 (se_mem_table_t *tab, int64_t id,
                               int32_t column);
SE_API int64_t read_cell_i64 (se_mem_table_t *tab, int64_t id, int32_t column);
SE_API uint64_t read_cell_u64 (se_mem_table_t *tab, int64_t id,
                               int32_t column);

SE_API double read_cell_double (se_mem_table_t *tab, int64_t id,
                                int32_t column);
SE_API float read_cell_float (se_mem_table_t *tab, int64_t id, int32_t column);

SE_API char *read_cell_str (se_mem_table_t *tab, int64_t id, int32_t column);
SE_API se_datablob_t *read_cell_blob (se_mem_table_t *tab, int64_t id,
                                      int32_t column);

/* ----------------------------- write function -----------------------------
 */
SE_API void create_spr_index (se_mem_table_t *tab);

SE_API char *add_row (se_mem_table_t *tab);
SE_API void set_row_byte_data (se_mem_table_t *tab, int64_t id, int32_t column,
                               char *data);
SE_API void set_row_blob_data (se_mem_table_t *tab, int64_t id, int32_t column,
                               se_datablob_t *data);
SE_API void set_coordinate (se_mem_table_t *tab, int64_t id);

SE_API void remove_row (se_mem_table_t *tab, int64_t id);
SE_API void remove_range (se_mem_table_t *tab, int64_t *id, uint32_t num);

#endif