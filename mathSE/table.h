#ifndef __TABLE_H__
#define __TABLE_H__

#include "i4.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct coordinate_blob coordinate;

/* memory table one column data type */
typedef struct table_field {
  char *name;
  /* column type, t: byte, b: blob */
  char type;
  /* When the type is byte, basic types such as char, short, int, long long, and
   * char* are supported. At this time, the data segment calculates the memory
   * size that needs to be allocated based on size. When the type is blob, there
   * is no need to pass in the size value. A pointer-sized space will be created
   * internally to record the pointer of the data_blob to obtain the real data.
   */
  int size;
} table_field;

/* blob or clob column data */
typedef struct data_blob {
  uint64_t cs;
  char *data;
} data_blob;

typedef struct __mem_table mem_table;

mem_table *create_mem_table(table_field *fields, uint32_t size);
void destroy_mem_table(mem_table *tab);

/* ------------------------------ read function ----------------------------- */
uint32_t find_column_by_name(mem_table *tab, const char *name);
table_field *table_fields(mem_table *tab);
uint64_t table_rows(mem_table *tab);
uint32_t table_column(mem_table *tab);

void reset_cursor(mem_table *tab);
char *table_next(mem_table *tab);
char *get_row(mem_table *tab, int64_t id);

coordinate *read_coordinate(mem_table *tab, int64_t id);

int8_t read_cell_i8(mem_table *tab, int64_t id, int32_t column);
uint8_t read_cell_u8(mem_table *tab, int64_t id, int32_t column);
int16_t read_cell_i16(mem_table *tab, int64_t id, int32_t column);
uint16_t read_cell_u16(mem_table *tab, int64_t id, int32_t column);
int32_t read_cell_i32(mem_table *tab, int64_t id, int32_t column);
uint32_t read_cell_u32(mem_table *tab, int64_t id, int32_t column);
int64_t read_cell_i64(mem_table *tab, int64_t id, int32_t column);
uint64_t read_cell_u64(mem_table *tab, int64_t id, int32_t column);

double read_cell_double(mem_table *tab, int64_t id, int32_t column);
float read_cell_float(mem_table *tab, int64_t id, int32_t column);

char *read_cell_str(mem_table *tab, int64_t id, int32_t column);
data_blob *read_cell_blob(mem_table *tab, int64_t id, int32_t column);

/* ----------------------------- write function ----------------------------- */
void create_spr_index(mem_table *tab);

char *add_row(mem_table *tab);
void set_row_byte_data(mem_table *tab, int64_t id, int32_t column, char *data);
void set_row_blob_data(mem_table *tab, int64_t id, int32_t column,
                       data_blob *data);
void set_coordinate(mem_table *tab, int64_t id);

void remove_row(mem_table *tab, int64_t id);
void remove_range(mem_table *tab, int64_t *id, uint32_t num);

#ifdef __cplusplus
}
#endif

#endif // __TABLE_H__