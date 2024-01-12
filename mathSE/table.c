#include "mathSE.h"
#include "hashtable.h"
#include <string.h>

/* ------------------------------ memory table ------------------------------ */
struct __mem_table {
  uint32_t col_num;
  uint64_t row_num;
  uint32_t row_size;
  struct table_field *fields;
  char **data;
  char *cursor;
  struct hash_table *ht;
  index_t *spa_index;
};

/* Necessary fields for a row of data, including unique id and geometry field
 * pointers under spatial data.*/
#if __WORDSIZE == 64
#define ROW_MASK 16
#elif __WORDSIZE == 32
#define ROW_MASK 12
#endif

uint32_t cala_row_bytes(table_field *fields, size_t size) {
  if (fields == NULL)
    return ROW_MASK;
  uint32_t size = ROW_MASK;
  for (int i = 0; i < size; ++i) {
    table_field *fid = fields + i;
    if (fid->type == 't' && fid->size > 0) {
      size += fid->size;
    } else if (fid->type == 'b') {
      size += sizeof(struct data_blob);
    }
    return size;
  }
}

/* -------------------------- memory table function -------------------------
 */

mem_table *create_mem_table(table_field *fields, uint32_t size) {
  mem_table *table = (mem_table *)malloc(sizeof(mem_table));
  if (table == NULL)
    return NULL;

  memset(table, 0, sizeof(mem_table));
  table->col_num = size;
  table->row_num = 0;
  table->row_size = cala_row_bytes(fields, size);
  table->fields = fields;
  table->data = NULL;
  table->cursor = NULL;
  table->ht = NULL;
  table->spa_index = NULL;
  return table;
}

void destroy_mem_table(mem_table *tab) {
  if (tab == NULL)
    return;

  if (tab->fields != NULL) {
    free(tab->fields);
  }

  tab->cursor = NULL;
  if (tab->data != NULL) {
    for (int i = 0; i < tab->row_num; ++i) {
      free(tab->data[i]);
    }
    free(tab->data);
  }
  if (tab->ht != NULL) {
    hashtab_free(tab->ht);
  }

  // free spatial index
}

uint32_t find_column_by_name(mem_table *tab, const char *name) {
  if (tab == NULL)
    return -1;
  for (int i = 0; i < tab->col_num; ++i) {
    if (strcmp(tab->fields[i].name, name) == 0)
      return i;
  }
  return -1;
}

table_field *table_fields(mem_table *tab) {
  if (tab == NULL)
    return NULL;
  return tab->fields;
}

uint64_t table_rows(mem_table *tab) {
  if (tab == NULL)
    return 0;
  return tab->row_num;
}

uint32_t table_column(mem_table *tab) {
  if (tab == NULL)
    return 0;
  return tab->col_num;
}

void reset_cursor(mem_table *tab) {
  if (tab == NULL) {
    return;
  }
  tab->cursor = NULL;
}

char *table_next(mem_table *tab) {
  if (tab == NULL || tab->data == NULL)
    return NULL;

  if (tab->cursor == NULL) {
    return tab->data[0];
  } else {
  }
  return NULL;
}

char *get_row(mem_table *tab, int64_t id) { return NULL; }

coordinate *read_coordinate(mem_table *tab, int64_t id) { return NULL; }

int8_t read_cell_i8(mem_table *tab, int64_t id, int32_t column) { return 0; }

uint8_t read_cell_u8(mem_table *tab, int64_t id, int32_t column) { return 0; }

int16_t read_cell_i16(mem_table *tab, int64_t id, int32_t column) { return 0; }

uint16_t read_cell_u16(mem_table *tab, int64_t id, int32_t column) { return 0; }

int32_t read_cell_i32(mem_table *tab, int64_t id, int32_t column) { return 0; }

uint32_t read_cell_u32(mem_table *tab, int64_t id, int32_t column) { return 0; }

int64_t read_cell_i64(mem_table *tab, int64_t id, int32_t column) { return 0; }

uint64_t read_cell_u64(mem_table *tab, int64_t id, int32_t column) { return 0; }

double read_cell_double(mem_table *tab, int64_t id, int32_t column) {
  return 0.0;
}

float read_cell_float(mem_table *tab, int64_t id, int32_t column) {
  return 0.0f;
}

char *read_cell_str(mem_table *tab, int64_t id, int32_t column) { return NULL; }

data_blob *read_cell_blob(mem_table *tab, int64_t id, int32_t column) {
  return NULL;
}

void create_spr_index(mem_table *tab) {}

char *add_row(mem_table *tab) { return NULL; }

void set_row_byte_data(mem_table *tab, int64_t id, int32_t column, char *data) {
}

void set_row_blob_data(mem_table *tab, int64_t id, int32_t column,
                       data_blob *data) {}

void set_coordinate(mem_table *tab, int64_t id) {}

void remove_row(mem_table *tab, int64_t id) {}

void remove_range(mem_table *tab, int64_t *id, uint32_t num) {}
