#ifndef __IO_H__
#define __IO_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ENDIAN_BIG 0
#define ENDIAN_LITTLE 1

uint64_t bswap_uint64(uint64_t src);
uint32_t bswap_uint32(uint32_t src);
uint16_t bswap_uint16(uint16_t src);

int64_t bswap_int64(int64_t src);
int32_t bswap_int32(int32_t src);
int16_t bswap_int16(int16_t src);

float bswap_float(float src);
double bswap_double(double src);

typedef struct coordinate_blob coordinate;

coordinate *read_from_wkt(const char *wkt);
coordinate *read_from_wkb(const char *b, size_t cs, int endian_type);
coordinate *read_from_geojson(const char *filename);
coordinate *read_from_kml2(const char *kml);
coordinate *read_from_kml3(const char *kml);
coordinate *read_from_ewkt(const char *wkt);
coordinate *read_from_ewkb(const char *b, size_t cs, int endian_type);
coordinate *read_from_gml2(const char *kml);
coordinate *read_from_gml3(const char *kml);

int write_to_wkt(coordinate *c);
int write_to_wkb(coordinate *c, char **b, size_t *cs, int endian_type);
int write_to_geojson(coordinate *c, const char *filename);
int write_to_kml2(coordinate *c, char **kml);
int write_to_kml3(coordinate *c, char **kml);
int write_to_ewkt(coordinate *c);
int write_to_ewkb(coordinate *c, char **b, size_t *cs, int endian_type);
int write_to_gml2(coordinate *c, char **kml);
int write_to_gml3(coordinate *c, char **kml);

#ifdef __cplusplus
}
#endif

#endif // __IO_H__
