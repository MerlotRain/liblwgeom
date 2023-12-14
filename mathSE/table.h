#ifndef TABLE_H
#define TABLE_H

#include "i4.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* memory table one column data type */
typedef struct table_column {
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