#ifndef TABLE_H
#define TABLE_H

#include "i4.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct table_column {
  char *name;
  char type;
  int byte_size;
} table_column;

typedef struct data_blob {
  uint64_t cs;
  char *data;
} data_blob;

typedef struct __mem_table mem_table_t;

#ifdef __cplusplus
}
#endif

#endif // TABLE_H