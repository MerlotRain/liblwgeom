#ifndef RW_H
#define RW_H

#ifdef __cplusplus
extern "C" {
#endif


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

#endif // RW_H
