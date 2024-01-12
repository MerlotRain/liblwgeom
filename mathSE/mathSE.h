/* mathSE is an algorithm library for processing two-dimensional geometric and
 * geographical information. It provides basic geometric construction, geometric
 * algorithms, spatial indexing, data storage and data display.
 */

#ifndef __MATHSE_H__
#define __MATHSE_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Invalid geometry type */
#define GEOMETRY_TYPE_NONE 0
/* single point geometry */
#define GEOMETRY_TYPE_POINT 1
/* Path usually consists of one or a group of line segments connected end to
 * end. It can be a straight line segment, an arc segment or a bezier curve. */
#define GEOMETRY_TYPE_PATH 2
/* Polygon is usually composed of a group of closed Rings, and there is at
 * least one outer ring in these Rings. If there is only one outer ring, then
 * the other rings are inner rings of the outer ring. */
#define GEOMETRY_TYPE_POLYGON 3
/* Collection is the parent type of Polygon and Polyline, but we usually use
 * collection to represent it. The geometric types of elements in collection are
 * inconsistent. If the geometry type is the same, it is usually represented by
 * polygon or polyline. */
#define GEOMETRY_TYPE_COLLECTION 4
/* A collection of points. */
#define GEOMETRY_TYPE_MULTIPOINT 5
/* Multi-segment polyline. It is a geometric collection type, and each
 * sub-element is a continuous polyline。 */
#define GEOMETRY_TYPE_POLYLINE 6
/* Rectangular structure, usually it only needs to be described by two points:
 * the upper left corner and the lower right corner. */
#define GEOMETRY_TYPE_ENVELOPE 7
/* Two point line segment. This is the most basic element that makes up a
 * polyline. */
#define GEOMETRY_TYPE_LINE 8
/* Three points that are not in a straight line must form an arc. The
 * coordinates of a set of consecutive arcs are described as the last point of
 * the first arc is the first point of the next arc. That is, the nodes at the
 * connections are reused. */
#define GEOMETRY_TYPE_CIRCLEARC 9
/* Represents a polyline segment connected end to end. Because they are
 * connected end to end, the line segments divide the space into interior,
 * exterior, and boundary. It is the most basic element that makes up
 * polygons. */
#define GEOMETRY_TYPE_RING 10

/* -------------------------------------------------------------------------- */
/*                                 Coordinate                                 */
/* -------------------------------------------------------------------------- */

typedef struct se_coordinate_blob coordinate_blob_t;

typedef struct se_raw_point {
  double x;
  double y;
} raw_point_t;

typedef struct se_raw_point3d {
  double x;
  double y;
  double z;
} raw_point3d_t;

int compare_raw_point(raw_point_t *a, raw_point_t *b);
int compare_raw_point3d(raw_point3d_t *a, raw_point3d_t *b);

/* -------------------------------------------------------------------------- */
/*                             Coordinate factory                             */
/* -------------------------------------------------------------------------- */


coordinate_blob_t *create_coordinate(uint32_t i_n, const int *i_p, uint32_t c_n,
                                     uint32_t c_dim, const double *c_p,
                                     int flags);
void coordinate_info(coordinate_blob_t *c, uint32_t *i_n, int **i_p,
                     uint32_t *c_n, uint32_t *c_dim, double **c_p);

void coordinate_destroy(coordinate_blob_t *c);

/* create point */
coordinate_blob_t *create_point(double *p);
/* create two point line */
coordinate_blob_t *create_line(double *p);
/* create line by one point and angle with length */
coordinate_blob_t *create_line2(double *p, double angle, double length);
/* create arc line by three point */
coordinate_blob_t *create_arc(double *p);
/* create arc by circle and sweep angle */
coordinate_blob_t *create_arc2(double *c, double radius, double angle1,
                               double angle2);
/* create arc by begin and end point, chord */
coordinate_blob_t *create_arc3(double *p, double chord);
/* create single path */
coordinate_blob_t *create_path(double *p, int num);
/* create envelope */
coordinate_blob_t *create_envelope(double *p);

/* -------------------------------------------------------------------------- */
/*                                     i4                                     */
/* -------------------------------------------------------------------------- */

typedef struct se_i4 i4_t;

i4_t *create_i4(const void *p, size_t prop_size);
void destroy_i4(i4_t *p);
const void *i4_ptr(i4_t *p);
void set_i4_prop(i4_t *p, size_t index, int prop);
int get_i4_prop(i4_t *p, size_t index);
int i4_prop_size(i4_t *p);

double tolerance(double tol);

/* -------------------------------------------------------------------------- */
/*                                  Algorithm                                 */
/* -------------------------------------------------------------------------- */

int geometry_type(coordinate_blob_t *c);
int *coordinate_interpret(coordinate_blob_t *c);
int coordinate_interpret_n(coordinate_blob_t *c);
double *coordinate_coord(coordinate_blob_t *c);
unsigned int coordinate_point_n(coordinate_blob_t *c);
int coordinate_dim_c(coordinate_blob_t *c);
int coordinate_dim_g(coordinate_blob_t *c);
int coordinate_sub_n(coordinate_blob_t *c);
coordinate_blob_t *coordinate_sub_at(coordinate_blob_t *c, int index);

double value_area(coordinate_blob_t *c);
double value_length(coordinate_blob_t *c);
double value_width(coordinate_blob_t *c);
double value_height(coordinate_blob_t *c);

coordinate_blob_t *geometry_clone(coordinate_blob_t *c);
coordinate_blob_t *geometry_label(coordinate_blob_t *c);
coordinate_blob_t *geometry_center(coordinate_blob_t *c);
coordinate_blob_t *geometry_centroid(coordinate_blob_t *c);
coordinate_blob_t *geometry_envelope(coordinate_blob_t *c);

void geometry_label2(coordinate_blob_t *c, double *result);
void geometry_center2(coordinate_blob_t *c, double *result);
void geometry_centroid2(coordinate_blob_t *c, double *result);
void geometry_envelope2(coordinate_blob_t *c, double *result);

/* -------------------------------------------------------------------------- */
/*                         Coordinate Reference System                        */
/* -------------------------------------------------------------------------- */

#define TRANS_WGS84_TO_CGCS2000 0
#define TRANS_WGS84_TO_XIAN80 1
#define TRANS_CGCS2000_TO_WGS84 3
#define TRANS_CGCS2000_TO_XIAN80 4
#define TRANS_XIAN80_TO_CGCS2000 5
#define TRANS_XIAN80_TO_WGS84 6

#define TRANS_FORWARD 1
#define TRANS_INVERSE -1

typedef struct se_crs_transform crs_transformation_t;

crs_transformation_t *create_trans(int epsg_from, int epsg_to);
crs_transformation_t *create_wellknown_trans(int wellknown);
void destroy_trans(crs_transformation_t *trans);

int trans_coord(crs_transformation_t *trans, int direction, double *c);
int trans_coord_array(crs_transformation_t *trans, int direction, double *c,
                      int c_n);

/* -------------------------------------------------------------------------- */
/*                                    Table                                   */
/* -------------------------------------------------------------------------- */

/* memory table one column data type */
typedef struct se_table_field {
  /* column type, t: byte, b: blob */
  char type;
  /* When the type is byte, basic types such as char, short, int, long long, and
   * char* are supported. At this time, the data segment calculates the memory
   * size that needs to be allocated based on size. When the type is blob, there
   * is no need to pass in the size value. A pointer-sized space will be created
   * internally to record the pointer of the data_blob to obtain the real data.
   */
  int size;
  char name[36];
} table_field_t;

/* blob or clob column data */
typedef struct data_blob {
  uint64_t cs;
  char *data;
} data_blob;

typedef struct se_mem_table mem_table_t;

mem_table_t *create_mem_table(table_field_t *fields, uint32_t size);
void destroy_mem_table(mem_table_t *tab);

/* ------------------------------ read function ----------------------------- */
uint32_t find_column_by_name(mem_table_t *tab, const char *name);
table_field_t *table_fields(mem_table_t *tab);
uint64_t table_rows(mem_table_t *tab);
uint32_t table_column(mem_table_t *tab);

void reset_cursor(mem_table_t *tab);
char *table_next(mem_table_t *tab);
char *get_row(mem_table_t *tab, int64_t id);

coordinate_blob_t *read_coordinate(mem_table_t *tab, int64_t id);

int8_t read_cell_i8(mem_table_t *tab, int64_t id, int32_t column);
uint8_t read_cell_u8(mem_table_t *tab, int64_t id, int32_t column);
int16_t read_cell_i16(mem_table_t *tab, int64_t id, int32_t column);
uint16_t read_cell_u16(mem_table_t *tab, int64_t id, int32_t column);
int32_t read_cell_i32(mem_table_t *tab, int64_t id, int32_t column);
uint32_t read_cell_u32(mem_table_t *tab, int64_t id, int32_t column);
int64_t read_cell_i64(mem_table_t *tab, int64_t id, int32_t column);
uint64_t read_cell_u64(mem_table_t *tab, int64_t id, int32_t column);

double read_cell_double(mem_table_t *tab, int64_t id, int32_t column);
float read_cell_float(mem_table_t *tab, int64_t id, int32_t column);

char *read_cell_str(mem_table_t *tab, int64_t id, int32_t column);
data_blob *read_cell_blob(mem_table_t *tab, int64_t id, int32_t column);

/* ----------------------------- write function ----------------------------- */
void create_spr_index(mem_table_t *tab);

char *add_row(mem_table_t *tab);
void set_row_byte_data(mem_table_t *tab, int64_t id, int32_t column,
                       char *data);
void set_row_blob_data(mem_table_t *tab, int64_t id, int32_t column,
                       data_blob *data);
void set_coordinate(mem_table_t *tab, int64_t id);

void remove_row(mem_table_t *tab, int64_t id);
void remove_range(mem_table_t *tab, int64_t *id, uint32_t num);

/* -------------------------------------------------------------------------- */
/*                                   GeoHash                                  */
/* -------------------------------------------------------------------------- */

typedef struct se_geohash_point geohash_point_t;

#define GEOHASH_DIRECTION_NORTH 0
#define GEOHASH_DIRECTION_EAST 1
#define GEOHASH_DIRECTION_WEST 2
#define GEOHASH_DIRECTION_SOUTH 3
#define GEOHASH_DIRECTION_SOUTH_WEST 4
#define GEOHASH_DIRECTION_SOUTH_EAST 5
#define GEOHASH_DIRECTION_NORTH_WEST 6
#define GEOHASH_DIRECTION_NORTH_EAST 7

typedef struct se_geohash_bits {
  uint64_t bits;
  uint8_t step;
} geohash_bits_t;

typedef struct se_geohash_neighbors {
  geohash_bits_t north;
  geohash_bits_t east;
  geohash_bits_t west;
  geohash_bits_t south;
  geohash_bits_t south_west;
  geohash_bits_t south_east;
  geohash_bits_t north_west;
  geohash_bits_t north_east;
} geohash_neighbors_t;

typedef struct se_geohash_range {
  double max;
  double min;
} geohash_range_t;

typedef struct se_geohash_area {
  geohash_bits_t hash;
  geohash_range_t longitude;
  geohash_range_t latitude;
} geohash_area_t;

void get_geohash_range(geohash_range_t *long_range, geohash_range_t *lat_range);
geohash_bits_t *geohash_encode(const geohash_range_t *long_range,
                               const geohash_range_t *lat_range,
                               double longitude, double latitude, uint8_t step);
geohash_bits_t *geohash_encode_type(double longitude, double latitude,
                                    uint8_t step);
geohash_bits_t *geohash_encode_WGS84(double longitude, double latitude,
                                     uint8_t step);
geohash_area_t *geohash_decode(const geohash_range_t long_range,
                               const geohash_range_t lat_range,
                               const geohash_bits_t hash);
geohash_area_t *geohash_decode_type(const geohash_range_t long_range,
                                    const geohash_range_t lat_range,
                                    const geohash_bits_t hash);
geohash_area_t *geohash_decode_WGS84(const geohash_range_t long_range,
                                     const geohash_range_t lat_range,
                                     const geohash_bits_t hash);

int geohash_decode_area_longlat(const geohash_area_t *area, double *xy);
int geohash_decode_longlat_type(const geohash_area_t *area, double *xy);
int geohash_decode_longlat_WGS84(const geohash_area_t *area, double *xy);

geohash_neighbors_t *geohash_query_neighbors(const geohash_bits_t *hash);

/* -------------------------------------------------------------------------- */
/*                                Spatial Index                               */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                 Read Write                                 */
/* -------------------------------------------------------------------------- */

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

coordinate_blob_t *read_from_wkt(const char *wkt);
coordinate_blob_t *read_from_wkb(const char *b, size_t cs, int endian_type);
coordinate_blob_t *read_from_geojson(const char *filename);
coordinate_blob_t *read_from_kml2(const char *kml);
coordinate_blob_t *read_from_kml3(const char *kml);
coordinate_blob_t *read_from_ewkt(const char *wkt);
coordinate_blob_t *read_from_ewkb(const char *b, size_t cs, int endian_type);
coordinate_blob_t *read_from_gml2(const char *kml);
coordinate_blob_t *read_from_gml3(const char *kml);

int write_to_wkt(coordinate_blob_t *c);
int write_to_wkb(coordinate_blob_t *c, char **b, size_t *cs, int endian_type);
int write_to_geojson(coordinate_blob_t *c, const char *filename);
int write_to_kml2(coordinate_blob_t *c, char **kml);
int write_to_kml3(coordinate_blob_t *c, char **kml);
int write_to_ewkt(coordinate_blob_t *c);
int write_to_ewkb(coordinate_blob_t *c, char **b, size_t *cs, int endian_type);
int write_to_gml2(coordinate_blob_t *c, char **kml);
int write_to_gml3(coordinate_blob_t *c, char **kml);

/* -------------------------------------------------------------------------- */
/*                                  Graphics                                  */
/* -------------------------------------------------------------------------- */

typedef unsigned int RGB;
const RGB RGB_MASK = 0x00ffffff;

#define RED(rgb) ((rgb >> 16) & 0xff)
#define GREEN(rgb) ((rgb >> 8) & 0xff)
#define BLUE(rgb) (rgb & 0xff)
#define ALPHA(rgb) (rgb >> 24)
#define INIT_RGB(r, g, b)                                                      \
  ((0xffu << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu))
#define INIT_RGBA(r, g, b, a)                                                  \
  (((a & 0xffu) << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu))

enum {
  SEAliceBlue = 0xFFF0F8FF,
  SEAntiqueWhite = 0xFFFAEBD7,
  SEAqua = 0xFF00FFFF,
  SEAquamarine = 0xFF7FFFD4,
  SEAzure = 0xFFF0FFFF,
  SEBeige = 0xFFF5F5DC,
  SEBisque = 0xFFFFE4C4,
  SEBlack = 0xFF000000,
  SEBlanchedAlmond = 0xFFFFEBCD,
  SEBlue = 0xFF0000FF,
  SEBlueViolet = 0xFF8A2BE2,
  SEBrown = 0xFFA52A2A,
  SEBurlyWood = 0xFFDEB887,
  SECadetBlue = 0xFF5F9EA0,
  SEChartreuse = 0xFF7FFF00,
  SEChocolate = 0xFFD2691E,
  SECoral = 0xFFFF7F50,
  SECornflowerBlue = 0xFF6495ED,
  SECornsilk = 0xFFFFF8DC,
  SECrimson = 0xFFDC143C,
  SECyan = 0xFF00FFFF,
  SEDarkBlue = 0xFF00008B,
  SEDarkCyan = 0xFF008B8B,
  SEDarkGoldenrod = 0xFFB8860B,
  SEDarkGray = 0xFFA9A9A9,
  SEDarkGreen = 0xFF006400,
  SEDarkKhaki = 0xFFBDB76B,
  SEDarkMagenta = 0xFF8B008B,
  SEDarkOliveGreen = 0xFF556B2F,
  SEDarkOrange = 0xFFFF8C00,
  SEDarkOrchid = 0xFF9932CC,
  SEDarkRed = 0xFF8B0000,
  SEDarkSalmon = 0xFFE9967A,
  SEDarkSeaGreen = 0xFF8FBC8B,
  SEDarkSlateBlue = 0xFF483D8B,
  SEDarkSlateGray = 0xFF2F4F4F,
  SEDarkTurquoise = 0xFF00CED1,
  SEDarkViolet = 0xFF9400D3,
  SEDeepPink = 0xFFFF1493,
  SEDeepSkyBlue = 0xFF00BFFF,
  SEDimGray = 0xFF696969,
  SEDodgerBlue = 0xFF1E90FF,
  SEFeldspar = 0xFFD19275,
  SEFirebrick = 0xFFB22222,
  SEFloralWhite = 0xFFFFFAF0,
  SEForestGreen = 0xFF228B22,
  SEFuchsia = 0xFFFF00FF,
  SEGainsboro = 0xFFDCDCDC,
  SEGhostWhite = 0xFFF8F8FF,
  SEGold = 0xFFFFD700,
  SEGoldenrod = 0xFFDAA520,
  SEGray = 0xFF808080,
  SEGreen = 0xFF00FF00,
  SEGreenYellow = 0xFFADFF2F,
  SEHoneydew = 0xFFF0FFF0,
  SEHotPink = 0xFFFF69B4,
  SEIndianRed = 0xFFCD5C5C,
  SEIndigo = 0xFF4B0082,
  SEIvory = 0xFFFFFFF0,
  SEKhaki = 0xFFF0E68C,
  SELavender = 0xFFE6E6FA,
  SELavenderBlush = 0xFFFFF0F5,
  SELawnGreen = 0xFF7CFC00,
  SELemonChiffon = 0xFFFFFACD,
  SELightBlue = 0xFFADD8E6,
  SELightCoral = 0xFFF08080,
  SELightCyan = 0xFFE0FFFF,
  SELightGoldenrodYellow = 0xFFFAFAD2,
  SELightGray = 0xFFD3D3D3,
  SELightGrey = 0xFFD3D3D3,
  SELightGreen = 0xFF90EE90,
  SELightPink = 0xFFFFB6C1,
  SELightSalmon = 0xFFFFA07A,
  SELightSeaGreen = 0xFF20B2AA,
  SELightSkyBlue = 0xFF87CEFA,
  SELightSlateBlue = 0xFF8470FF,
  SELightSlateGray = 0xFF778899,
  SELightSteelBlue = 0xFFB0C4DE,
  SELightYellow = 0xFFFFFFE0,
  SELime = 0xFF00FF00,
  SELimeGreen = 0xFF32CD32,
  SELinen = 0xFFFAF0E6,
  SEMagenta = 0xFFFF00FF,
  SEMaroon = 0xFF800000,
  SEMediumAquamarine = 0xFF66CDAA,
  SEMediumBlue = 0xFF0000CD,
  SEMediumOrchid = 0xFFBA55D3,
  SEMediumPurple = 0xFF9370DB,
  SEMediumSeaGreen = 0xFF3CB371,
  SEMediumSlateBlue = 0xFF7B68EE,
  SEMediumSpringGreen = 0xFF00FA9A,
  SEMediumTurquoise = 0xFF48D1CC,
  SEMediumVioletRed = 0xFFC71585,
  SEMidnightBlue = 0xFF191970,
  SEMintCream = 0xFFF5FFFA,
  SEMistyRose = 0xFFFFE4E1,
  SEMoccasin = 0xFFFFE4B5,
  SENavajoWhite = 0xFFFFDEAD,
  SENavy = 0xFF000080,
  SEOldLace = 0xFFFDF5E6,
  SEOlive = 0xFF808000,
  SEOliveDrab = 0xFF6B8E23,
  SEOrange = 0xFFFFA500,
  SEOrangeRed = 0xFFFF4500,
  SEOrchid = 0xFFDA70D6,
  SEPaleGoldenrod = 0xFFEEE8AA,
  SEPaleGreen = 0xFF98FB98,
  SEPaleTurquoise = 0xFFAFEEEE,
  SEPaleVioletRed = 0xFFDB7093,
  SEPapayaWhip = 0xFFFFEFD5,
  SEPeachPuff = 0xFFFFDAB9,
  SEPeru = 0xFFCD853F,
  SEPink = 0xFFFFC0CB,
  SEPlum = 0xFFDDA0DD,
  SEPowderBlue = 0xFFB0E0E6,
  SEPurple = 0xFF800080,
  SERed = 0xFFFF0000,
  SERosyBrown = 0xFFBC8F8F,
  SERoyalBlue = 0xFF4169E1,
  SESaddleBrown = 0xFF8B4513,
  SESalmon = 0xFFFA8072,
  SESandyBrown = 0xFFF4A460,
  SESeaGreen = 0xFF2E8B57,
  SESeaShell = 0xFFFFF5EE,
  SESienna = 0xFFA0522D,
  SESilver = 0xFFC0C0C0,
  SESkyBlue = 0xFF87CEEB,
  SESlateBlue = 0xFF6A5ACD,
  SESlateGray = 0xFF708090,
  SESnow = 0xFFFFFAFA,
  SESpringGreen = 0xFF00FF7F,
  SESteelBlue = 0xFF4682B4,
  SETan = 0xFFD2B48C,
  SETeal = 0xFF008080,
  SEThistle = 0xFFD8BFD8,
  SETomato = 0xFFFF6347,
  SETransparent = 0x00FFFFFF,
  SETurquoise = 0xFF40E0D0,
  SEViolet = 0xFFEE82EE,
  SEVioletRed = 0xFFD02090,
  SEWheat = 0xFFF5DEB3,
  SEWhite = 0xFFFFFFFF,
  SEWhiteSmoke = 0xFFF5F5F5,
  SEYellow = 0xFFFFFF00,
  SEYellowGreen = 0xFF9ACD32,
};

void rgb2hsv(RGB rgb, double *h, double *s, double *v);
void hsv2rgb(double h, double s, double v, RGB *rgb);

typedef struct se_graphics_context graphics_context_t;

typedef struct se_graphics_display_transform {
  double reference_scale;
  double map_extent[4];
  double device_extent[4];
  double resolution;
  double scale;
  double rotate_angle;
  float dpi;
} graphics_display_transform_t;

#ifdef __cplusplus
}
#endif

#endif //__MATHSE_H__