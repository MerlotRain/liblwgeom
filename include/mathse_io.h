#ifndef __MATHSE_IO_H__
#define __MATHSE_IO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------- Read Write ------------------------------- */
/* Byte order big */
#define ENDIAN_BIG 0
/* Byte order little */
#define ENDIAN_LITTLE 1

uint64_t bswap_uint64(uint64_t src);
uint32_t bswap_uint32(uint32_t src);
uint16_t bswap_uint16(uint16_t src);

int64_t bswap_int64(int64_t src);
int32_t bswap_int32(int32_t src);
int16_t bswap_int16(int16_t src);

float  bswap_float(float src);
double bswap_double(double src);

/* read wkt string */
SE_API geom_coordinate_t *se_wkt_reader(const char *wkt);

/* read wkb buffer */
SE_API geom_coordinate_t *se_wkb_reader(const char *buffer, int length);

/* read ewkt string */
SE_API geom_coordinate_t *se_ewkt_reader(const char *ewkt);

/* read ewkb buffer */
SE_API geom_coordinate_t *se_ewkb_reader(const char *buffer, int length);

/* read kml string */
SE_API geom_coordinate_t *se_kml_reader(const char *kml);

/* read gml string */
SE_API geom_coordinate_t *se_gml_reader(const char *gml, int version);

/* read geojson string */
SE_API geom_coordinate_t *se_geojson_reader(const char *filename);

/* write wkt string */
SE_API int se_wkt_writer(const geom_coordinate_t *c, char **wkt);

/* write wkb buffer */
SE_API int se_wkb_writer(const geom_coordinate_t *c,
                         char                   **buffer,
                         size_t                  *size,
                         int                      endian_type);

/* write ewkt string */
SE_API int se_ewkt_writer(const geom_coordinate_t *c, char **ewkt);

/* write ewkb buffer */
SE_API int se_ewkb_writer(const geom_coordinate_t *c,
                          char                   **buffer,
                          size_t                  *size,
                          int                      endian_type);

/* write kml string */
SE_API int se_kml_writer(const geom_coordinate_t *c, char **kml);

/* write gml string */
SE_API int se_gml_writer(const geom_coordinate_t *c, char **gml);

/* write geojson string */
SE_API int se_gml2_writer(const geom_coordinate_t *c, char **gml);

/* write geojson string */
SE_API int se_geojson_writer(const geom_coordinate_t *c, const char *filename);

#ifdef __cplusplus
}
#endif

#endif//__MATHSE_IO_H__