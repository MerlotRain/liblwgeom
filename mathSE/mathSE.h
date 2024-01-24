/* mathSE is an algorithm library for processing two-dimensional geometric and
 * geographical information. It provides basic geometric construction, geometric
 * algorithms, spatial indexing, data storage and data display.
 */

#ifndef __MATHSE_H__
#define __MATHSE_H__

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#if defined(USE_TCMALLOC)

#include <google/tcmalloc.h>

#define malloc(size) tc_malloc(size)
#define calloc(count, size) tc_calloc(count, size)
#define realloc(ptr, size) tc_realloc(ptr, size)
#define free(ptr) tc_free(ptr)

#elif defined(USE_JEMALLOC)

#include <jemalloc/jemalloc.h>

#define malloc(size) je_malloc(size)
#define calloc(count, size) je_calloc(count, size)
#define realloc(ptr, size) je_realloc(ptr, size)
#define free(ptr) je_free(ptr)

#endif

/* ------------------------------ Geometry Type ----------------------------- */

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

/* Dimension value of non-empty geometries (= {P, L, A}) */
#define GEOMETRY_DIMENSION_TRUE -2
/* Dimension value of empty geometry (-1) */
#define GEOMETRY_DIMENSION_FALSE -1
/* Dimension value of point geometry (0) */
#define GEOMETRY_DIMENSION_P 0
/* Dimension value of line geometry (1) */
#define GEOMETRY_DIMENSION_L 1
/* Dimension value of area geometry (2) */
#define GEOMETRY_DIMENSION_A 2

/* --------------------------- Geometry Algorithm --------------------------- */

/* Calculate the length of a geometry */
#define GEOMETRY_PROP_VALUE_LENGTH 0
/* Calculate the width of a geometry */
#define GEOMETRY_PROP_VALUE_WIDTH 1
/* Calculate the height of a geometry */
#define GEOMETRY_PROP_VALUE_HEIGHT 2
/* Calculate the area of a geometry */
#define GEOMETRY_PROP_VALUE_AREA 3

/* Get a clone of the geometry */
#define GEOMETRY_PROP_GEO_CLONE 10
/* Get the marker point of the geometry, which must be inside the geometry */
#define GEOMETRY_PROP_GEO_LABEL 11
/* Get the center point of the geometry */
#define GEOMETRY_PROP_GEO_CENTER 12
/* Get the centroid point of the geometry */
#define GEOMETRY_PROP_GEO_CENTROID 13
/* Get the envelope of the geometry */
#define GEOMETRY_PROP_GEO_ENVELOPE 14
/* Get the oriented minimum envelope of the geometry */
#define GEOMETRY_PROP_GEO_ORIENTED_ENVELOPE 15
/* Get the minimum envelope of the geometry */
#define GEOMETRY_PROP_GEO_ENVELOPE_CIRCLE 16
/* Get the inner minimum rectangle of the geometry */
#define GEOMETRY_PROP_GEO_INNER_RECT 17
/* Get the inner minimum square of the geometry */
#define GEOMETRY_PROP_GEO_INNER_SQUARE 18
/* Get the inner minimum circle of the geometry */
#define GEOMETRY_PROP_GEO_INNER_CIRCLE 19
/* Get the smallest convex polygon that contains all the points int the geometry
 */
#define GEOMETRY_PROP_GEO_CONVEX_HULL 20
/* Get the simplified geometry */
#define GEOMETRY_PROP_GEO_SIMPLIFY 21
/* Geometric simplification that preserves the original geometric topology */
#define GEOMETRY_PROP_GEO_TOPOLOGY_PRESERVING_SIMPLIFY 22
/* Get the boundary of the geometry */
#define GEOMETRY_PROP_GEO_BOUNDARY 23

/* Delete all duplicate points, and pass in the duplicate point judgment
 * threshold (double) in para */
#define GEOMETRY_MODIFY_REMOVE_REPEAT 40
/* Douglas compression algorithm, para is the compression algorithm threshold
 * (double) */
#define GEOMETRY_MODIFY_DOUGLAS 41
/* Reverse the order of the points in the geometry, para is null */
#define GEOMETRY_MODIFY_REVERSE 42

/* Computes the result of union two geometries */
#define GEOMETRY_COMBINE_UNION 50
/* Computes the result of the intersection of two geometries */
#define GEOMETRY_COMBINE_INTERSECT 51
/* Computes the result of the difference of two geometries*/
#define GEOMETRY_COMBINE_DIFFERENCE 52
/* Calculate the result of the difference between two geometric symmetries */
#define GEOMETRY_COMBINE_SYM_DIFFERENCE 53

/* Two geometries do not fit this relationship */
#define GEOMETRY_RELATION_RESULT_UNFIT -1
/* The two geometries do not satisfy this relationship */
#define GEOMETRY_RELATION_RESULT_FALSE 0
/* Two geometries satisfy this relationship */
#define GEOMETRY_RELATION_RESULT_TRUE 1

/* Two geometries is disjoint each other */
#define GEOMETRY_SPR_IS_DISJOINT 60
/* Two geometries is intersect each other */
#define GEOMETRY_SPR_IS_INTERSECT 61
/* Two geometries is contain each other */
#define GEOMETRY_SPR_IS_CONTAIN 62
/* Two geometries is cross each other */
#define GEOMETRY_SPR_IS_CROSS 63
/* Two geometries is equal */
#define GEOMETRY_SPR_IS_EQUAL 64
/* Two geometries is touch each other */
#define GEOMETRY_SPR_IS_TOUCH 65
/* Two geometries is overlap each other */
#define GEOMETRY_SPR_IS_OVERLAP 66
/* Two geometries is within each other */
#define GEOMETRY_SPR_IS_WITHIN 67

/* Check geometry is simple */
#define GEOMETRY_CHECK_1_SIMPLE 80
/* Check geometry is close */
#define GEOMETRY_CHECK_1_CLOSE 81
/* Check geometry is good direction */
#define GEOMETRY_CHECK_1_GOOD_DIRECTION 82
/* Check geometry is self cross */
#define GEOMETRY_CHECK_1_SELF_CROSS 83
/* Check geometry has repeat point */
#define GEOMETRY_CHECK_1_REPEAT 84
/* Check line is lap */
#define GEOMETRY_CHECK_1_LAP 85

/* Check geometry repeat points */
#define GEOMETRY_CHECK_2_REPEAT_POINT 0x01
/* Check geometry isolate points */
#define GEOMETRY_CHECK_2_ISOLATE_POINT 0x02
/* Check geometry pseudo endpoints */
#define GEOMETRY_CHECK_2_PSEUDO_ENDPOINT 0x04

/* ---------------------- Coordinate Reference System ---------------------- */

/* Conversion between WGS84 coordinate system and CGCS2000 coordinate system,
 * the default is from WGS84 to CGCS2000 */
#define TRANS_WGS84_WITH_CGCS2000
/* Conversion between WGS84 coordinate system and XIAN80 coordinate system,
 * the default is from WGS84 to XIAN80 */
#define TRANS_WGS84_WITH_XIAN80
/* Conversion between CGCS2000 coordinate system and XIAN80 coordinate system,
 * the default is from CGCS2000 to XIAN80 */
#define TRANS_CGCS2000_WITH_XIAN80

/* Apply transformation to observation - in forward or inverse direction */
/* Forward */
#define TRANS_FORWARD 1
/* do nothing */
#define TRANS_NONE 0
/* Inverse */
#define TRANS_INVERSE -1

/* --------------------------------- GeoHash -------------------------------- */

/* Direction north */
#define GEOHASH_DIRECTION_NORTH 0
/* Direction east */
#define GEOHASH_DIRECTION_EAST 1
/* Direction west */
#define GEOHASH_DIRECTION_WEST 2
/* Direction south */
#define GEOHASH_DIRECTION_SOUTH 3
/* Direction south west */
#define GEOHASH_DIRECTION_SOUTH_WEST 4
/* Direction south east */
#define GEOHASH_DIRECTION_SOUTH_EAST 5
/* Direction north west */
#define GEOHASH_DIRECTION_NORTH_WEST 6
/* Direction north east */
#define GEOHASH_DIRECTION_NORTH_EAST 7

/* ------------------------------- Read Write ------------------------------- */
/* Byte order big */
#define ENDIAN_BIG 0
/* Byte order little */
#define ENDIAN_LITTLE 1

/* -------------------------------- Graphics -------------------------------- */

typedef unsigned int RGB;
const RGB RGB_MASK = 0x00ffffff;
/* Get the red component of a color */
#define RED(rgb) ((rgb >> 16) & 0xff)
/* Get the green component of a color */
#define GREEN(rgb) ((rgb >> 8) & 0xff)
/* Get the blue component of a color */
#define BLUE(rgb) (rgb & 0xff)
/* Get the alpha component of a color */
#define ALPHA(rgb) (rgb >> 24)
/* Create color with red, green, and blue components */
#define INIT_RGB(r, g, b)                                                      \
  ((0xffu << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu))
/* Create color with red, green, blue, and alpha components */
#define INIT_RGBA(r, g, b, a)                                                  \
  (((a & 0xffu) << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu))

/* https://www.w3schools.com/colors/ A color selection list suitable for HTML.
 * The color value is in standard ARGB format. The specified component of the
 * color can be obtained through RED, GREEN, BLUE, and ALPHA macros. */
enum COLOR {
  AliceBlue = 0xFFF0F8FF,
  AntiqueWhite = 0xFFFAEBD7,
  Aqua = 0xFF00FFFF,
  Aquamarine = 0xFF7FFFD4,
  Azure = 0xFFF0FFFF,
  Beige = 0xFFF5F5DC,
  Bisque = 0xFFFFE4C4,
  Black = 0xFF000000,
  BlanchedAlmond = 0xFFFFEBCD,
  Blue = 0xFF0000FF,
  BlueViolet = 0xFF8A2BE2,
  Brown = 0xFFA52A2A,
  BurlyWood = 0xFFDEB887,
  CadetBlue = 0xFF5F9EA0,
  Chartreuse = 0xFF7FFF00,
  Chocolate = 0xFFD2691E,
  Coral = 0xFFFF7F50,
  CornflowerBlue = 0xFF6495ED,
  Cornsilk = 0xFFFFF8DC,
  Crimson = 0xFFDC143C,
  Cyan = 0xFF00FFFF,
  DarkBlue = 0xFF00008B,
  DarkCyan = 0xFF008B8B,
  DarkGoldenrod = 0xFFB8860B,
  DarkGray = 0xFFA9A9A9,
  DarkGreen = 0xFF006400,
  DarkKhaki = 0xFFBDB76B,
  DarkMagenta = 0xFF8B008B,
  DarkOliveGreen = 0xFF556B2F,
  DarkOrange = 0xFFFF8C00,
  DarkOrchid = 0xFF9932CC,
  DarkRed = 0xFF8B0000,
  DarkSalmon = 0xFFE9967A,
  DarkSeaGreen = 0xFF8FBC8B,
  DarkSlateBlue = 0xFF483D8B,
  DarkSlateGray = 0xFF2F4F4F,
  DarkTurquoise = 0xFF00CED1,
  DarkViolet = 0xFF9400D3,
  DeepPink = 0xFFFF1493,
  DeepSkyBlue = 0xFF00BFFF,
  DimGray = 0xFF696969,
  DodgerBlue = 0xFF1E90FF,
  Feldspar = 0xFFD19275,
  Firebrick = 0xFFB22222,
  FloralWhite = 0xFFFFFAF0,
  ForestGreen = 0xFF228B22,
  Fuchsia = 0xFFFF00FF,
  Gainsboro = 0xFFDCDCDC,
  GhostWhite = 0xFFF8F8FF,
  Gold = 0xFFFFD700,
  Goldenrod = 0xFFDAA520,
  Gray = 0xFF808080,
  Green = 0xFF00FF00,
  GreenYellow = 0xFFADFF2F,
  Honeydew = 0xFFF0FFF0,
  HotPink = 0xFFFF69B4,
  IndianRed = 0xFFCD5C5C,
  Indigo = 0xFF4B0082,
  Ivory = 0xFFFFFFF0,
  Khaki = 0xFFF0E68C,
  Lavender = 0xFFE6E6FA,
  LavenderBlush = 0xFFFFF0F5,
  LawnGreen = 0xFF7CFC00,
  LemonChiffon = 0xFFFFFACD,
  LightBlue = 0xFFADD8E6,
  LightCoral = 0xFFF08080,
  LightCyan = 0xFFE0FFFF,
  LightGoldenrodYellow = 0xFFFAFAD2,
  LightGray = 0xFFD3D3D3,
  LightGrey = 0xFFD3D3D3,
  LightGreen = 0xFF90EE90,
  LightPink = 0xFFFFB6C1,
  LightSalmon = 0xFFFFA07A,
  LightSeaGreen = 0xFF20B2AA,
  LightSkyBlue = 0xFF87CEFA,
  LightSlateBlue = 0xFF8470FF,
  LightSlateGray = 0xFF778899,
  LightSteelBlue = 0xFFB0C4DE,
  LightYellow = 0xFFFFFFE0,
  Lime = 0xFF00FF00,
  LimeGreen = 0xFF32CD32,
  Linen = 0xFFFAF0E6,
  Magenta = 0xFFFF00FF,
  Maroon = 0xFF800000,
  MediumAquamarine = 0xFF66CDAA,
  MediumBlue = 0xFF0000CD,
  MediumOrchid = 0xFFBA55D3,
  MediumPurple = 0xFF9370DB,
  MediumSeaGreen = 0xFF3CB371,
  MediumSlateBlue = 0xFF7B68EE,
  MediumSpringGreen = 0xFF00FA9A,
  MediumTurquoise = 0xFF48D1CC,
  MediumVioletRed = 0xFFC71585,
  MidnightBlue = 0xFF191970,
  MintCream = 0xFFF5FFFA,
  MistyRose = 0xFFFFE4E1,
  Moccasin = 0xFFFFE4B5,
  NavajoWhite = 0xFFFFDEAD,
  Navy = 0xFF000080,
  OldLace = 0xFFFDF5E6,
  Olive = 0xFF808000,
  OliveDrab = 0xFF6B8E23,
  Orange = 0xFFFFA500,
  OrangeRed = 0xFFFF4500,
  Orchid = 0xFFDA70D6,
  PaleGoldenrod = 0xFFEEE8AA,
  PaleGreen = 0xFF98FB98,
  PaleTurquoise = 0xFFAFEEEE,
  PaleVioletRed = 0xFFDB7093,
  PapayaWhip = 0xFFFFEFD5,
  PeachPuff = 0xFFFFDAB9,
  Peru = 0xFFCD853F,
  Pink = 0xFFFFC0CB,
  Plum = 0xFFDDA0DD,
  PowderBlue = 0xFFB0E0E6,
  Purple = 0xFF800080,
  Red = 0xFFFF0000,
  RosyBrown = 0xFFBC8F8F,
  RoyalBlue = 0xFF4169E1,
  SaddleBrown = 0xFF8B4513,
  Salmon = 0xFFFA8072,
  SandyBrown = 0xFFF4A460,
  SeaGreen = 0xFF2E8B57,
  SeaShell = 0xFFFFF5EE,
  Sienna = 0xFFA0522D,
  Silver = 0xFFC0C0C0,
  SkyBlue = 0xFF87CEEB,
  SlateBlue = 0xFF6A5ACD,
  SlateGray = 0xFF708090,
  Snow = 0xFFFFFAFA,
  SpringGreen = 0xFF00FF7F,
  SteelBlue = 0xFF4682B4,
  Tan = 0xFFD2B48C,
  Teal = 0xFF008080,
  Thistle = 0xFFD8BFD8,
  Tomato = 0xFFFF6347,
  Transparent = 0x00FFFFFF,
  Turquoise = 0xFF40E0D0,
  Violet = 0xFFEE82EE,
  VioletRed = 0xFFD02090,
  Wheat = 0xFFF5DEB3,
  White = 0xFFFFFFFF,
  WhiteSmoke = 0xFFF5F5F5,
  Yellow = 0xFFFFFF00,
  YellowGreen = 0xFF9ACD32,
};

/**
 * context_format_t:
 * @FORMAT_INVALID: no such format exist or is supported.
 * @FORMAT_ARGB32: each pixel is a 32-bit quantity, with alpha in the upper 8
 *    bits, red in the next 8 bits, then green, then blue. The 32-bits
 *    quantities are stored native-endian. Pre-multiplied alpha is used.
 * @FORMAT_RGB24: RGB24 each pixel is a 32-bit quantity, with the upper 8 bits
 *    unused. Red, Green, and Blue are stored in the remaining 24 bits int the
 *    order.
 * @FORMAT_A8: A8 each pixel is an 8-bit quantity holding an alpha value.
 *
 */
typedef enum se_context_format {
  FORMAT_INVALID = -1,
  FORMAT_ARGB32 = 0,
  FORMAT_RGB32 = 1,
  FORMAT_A8 = 2
} context_format_t;

/* Circle point */
#define SYMBOL_POINT_CIRCLE 0
/* Square point */
#define SYMBOL_POINT_SQUARE 1

/* A plain line. */
#define PEN_STYLE_SOLID_LINE 100
/* Dashes separated by a few pixels. */
#define PEN_STYLE_DASH_LINE 101
/* Dots separated by a few pixels. */
#define PEN_STYLE_DOT_LINE 102
/* Alternate dots and dashes. */
#define PEN_STYLE_DASH_DOT_LINE 103
/* One dash, two dots, one dash, two dots. */
#define PEN_STYLE_DASH_DOT_DOT_LINE 104

/* a square line end that does not cover the end point of the line. */
#define PEN_CAP_FLAT_STYLE 110
/* a square line end that covers the end point and extends beyond it by half the
 * line width. */
#define PEN_CAP_ROUND_STYLE 111
/* a rounded line end. */
#define PEN_CAP_SQUARE_STYLE 112

/* The outer edges of the lines are extended to meet at an angle, and this area
 * is filled. */
#define PEN_JOIN_MITER_STYLE 120
/* The triangular notch between the two lines is filled. */
#define PEN_JOIN_BEVEL_STYLE 121
/* A circular arc between the two lines is filled. */
#define PEN_JOIN_ROUND_STYLE 122

/* Uniform color. */
#define BRUSH_SOLID_STYLE 130
/* Horizontal lines. */
#define BRUSH_HORIZONTAL_STYLE 131
/* Vertical lines. */
#define BRUSH_VERTICAL_STYLE 132
/* Backward diagonal lines. */
#define BRUSH_FDIAG_STYLE 133
/* Forward diagonal lines.*/
#define BRUSH_BDIAG_STYLE 134
/* Crossing horizontal and vertical lines. */
#define BRUSH_CROSS_STYLE 135
/* Crossing diagonal lines. */
#define BRUSH_DIAGCROSS_STYLE 136

/* --------------------------------- Matrix --------------------------------- */
#define MATRIX_INIT                                                            \
  { 1, 0, 0, 1, 0, 0 }
/* Return matrix m11 element */
#define MATRIX_M11(m) ((m)->m_m11)
/* Return matrix m12 element */
#define MATRIX_M12(m) ((m)->m_m12)
/* Return matrix m21 element */
#define MATRIX_M21(m) ((m)->m_m21)
/* Return matrix m22 element */
#define MATRIX_M22(m) ((m)->m_m22)
/* Return matrix dx element */
#define MATRIX_DX(m) ((m)->m_dx)
/* Return matrix dy element */
#define MATRIX_DY(m) ((m)->m_dy)

/* Return matrix scale int x direction */
#define MATRIX_SCALE_X(m) ((m)->m_m11)
/* Return matrix scale int y direction */
#define MATRIX_SCALE_Y(m) ((m)->m_m22)
/* Return matrix rotate angle */
#define MATRIX_ROTATE_ANGLE(m) (asin((m)->m_m12))
/* Return matrix offset x */
#define MATRIX_OFFSET_X(m) ((m)->m_dx)
/* Return matrix offset y */
#define MATRIX_OFFSET_Y(m) ((m)->m_dy)

/* -------------------------------------------------------------------------- */
/*                                 Coordinate                                 */
/* -------------------------------------------------------------------------- */

/* A binary block of geometric coordinates, used to express the coordinate
 * information of a geometric body. */
typedef struct se_coordinate_blob coordinate_blob_t;

/* A simple two-dimensional point coordinate structure. */
typedef struct se_raw_point {
  double x;
  double y;
} raw_point_t;

/* rectangle */
typedef struct se_raw_box {
  raw_point_t lower_left;
  raw_point_t upper_right;
} raw_box_t;

/* bezier curve */
typedef struct se_bezier {
  raw_point_t p1;
  raw_point_t p2;
  raw_point_t p3;
  raw_point_t p4;
} bezier_t;

/* arc */
typedef struct se_arc {
  raw_point_t p1;
  raw_point_t p2;
  raw_point_t p3;
} arc_t;

/* triangle */
typedef struct se_triangle {
  raw_point_t p1;
  raw_point_t p2;
  raw_point_t p3;
} triangle_t;

/* line */
typedef struct se_line {
  raw_point_t p1;
  raw_point_t p2;
} line_t;

/* Returns whether the coordinates of two points are within the tolerance range
 * Include:
 * 0: Points are equal within tolerance
 * 1: Points are not equal within tolerance
 */
int compare_raw_point(const raw_point_t a, const raw_point_t b);

/* Returns the midpoint of two points. */
raw_point_t mid_point(const raw_point_t a, const raw_point_t b);

/* Returns the centroid of a triangle. */
raw_point_t triangle_in_centre(const triangle_t t);

/* Returns the circum centre of a triangle. */
raw_point_t triangle_circum_centre(const triangle_t t);

/* Returns whether the triangle is acute. */
int triangle_is_acute(const triangle_t t);

/* -------------------------------------------------------------------------- */
/*                             Coordinate factory                             */
/* -------------------------------------------------------------------------- */

/* Create coordinate blob
 * geo: geo type
 * i_n: interpret number
 * i_p: interpret array, it's range is [i_p, i_p + 3 * i_n)
 * c_n: coordinate number
 * c_p: coordinate pointer, it's range is [c_p, c_p + c_n)]
 * flags: Whether to copy coordinate memory. 0: Copy the output coordinate
 * memory block and take over memory management. 1: The output memory is not
 * copied and only referenced, and the memory is managed externally.*/
coordinate_blob_t *create_coordinate(const uint8_t geo, const uint32_t i_n,
                                     const int *i_p, const uint32_t c_n,
                                     const double *c_p, const int flags);
/* Get coordinate information */
void coordinate_info(const coordinate_blob_t *c, uint32_t *i_n, int **i_p,
                     uint32_t *c_n, double **c_p);
/* Destroy coordinate blob */
void coordinate_destroy(coordinate_blob_t *c);

/* Get geometry type
 * c: coordinate blob
 * return: geometry type */
int geometry_type(const coordinate_blob_t *c);
/* Pointer of coordinate interpret
 * c: coordinate blob
 * return: pointer of coordinate interpret */
int *coordinate_interpret(const coordinate_blob_t *c);
/* Number of coordinate interpret */
int coordinate_interpret_n(const coordinate_blob_t *c);
/* Return geometry point pointer */
double *coordinate_coord(const coordinate_blob_t *c);
/* Return geometry point number */
unsigned int coordinate_point_n(const coordinate_blob_t *c);
/* Return geometry dimension type, value range is {TRUE, FALSE, P, L, A} */
int coordinate_dim_g(const coordinate_blob_t *c);
/* Sub number of coordinate
 * c: coordinate blob
 * return: >= 1 */
int coordinate_sub_n(const coordinate_blob_t *c);
/* Sub coordinate blob */
coordinate_blob_t *coordinate_sub_at(coordinate_blob_t *c, int index);

/* create point. p is the first address of a two-dimensional point coordinate */
coordinate_blob_t *create_point(const double *p);
/* create two point line. p is the first address of two two-dimensional point
 * coordinate arrays */
coordinate_blob_t *create_line(const double *p);
/* create line by one point and angle with length */
coordinate_blob_t *create_line2(const double *p, const double angle,
                                const double length);
/* create arc line by three point */
coordinate_blob_t *create_arc(const double *p);
/* create arc by circle and sweep angle */
coordinate_blob_t *create_arc2(const double *c, const double radius,
                               const double angle1, const double angle2);
/* create arc by begin and end point, chord */
coordinate_blob_t *create_arc3(const double *p, const double chord);
/* create single path */
coordinate_blob_t *create_path(const double *p, const int num);
/* create envelope */
coordinate_blob_t *create_envelope(const double *p);

/* -------------------------------------------------------------------------- */
/*                                     i4                                     */
/* -------------------------------------------------------------------------- */

/* se_i4 is an attribute object, which consists of two parts, one is the object
 * pointer, and the second is the object's attribute information.
 * These attribute information are not possessed by the object itself, such as
 * polygon area, polygon enclosing rectangle, etc. Rather, they are some markup
 * attributes, such as the ID of the polygon, and the correspondence between the
 * polygon and a certain field attribute.
 * These properties are passed
 * externally, and we agree that the passed-in value can only be a simple int
 * value, and the meaning of the value is determined externally.
 * After completing a set of object operations, these attribute values
 * still exist in the object, and the object can be obtained by obtaining
 * the attribute values. */
typedef struct se_i4 i4_t;

/* Create i4 */
i4_t *create_i4(const void *p, size_t prop_size);
/* Destroy i4 */
void destroy_i4(i4_t *p);
/* Return prop pointer */
const void *i4_ptr(i4_t *p);
/* Set prop value */
void set_i4_prop(i4_t *p, size_t index, int prop);
/* Get prop value */
int get_i4_prop(i4_t *p, size_t index);
/* Return prop size*/
int i4_prop_size(i4_t *p);

/* -------------------------------------------------------------------------- */
/*                                  Algorithm                                 */
/* -------------------------------------------------------------------------- */

/* Set global tolerance
 * tol: new tolerance
 * return: old tolerance */
double tolerance(double tol);

/* Compute numerical properties of geometry */
double geometry_prop_value(const coordinate_blob_t *c, int mode);

/* Get the geometric properties of a geometry */
coordinate_blob_t *geometry_prop_geo(const coordinate_blob_t *c, int mode);

/* Obtain simple geometric properties of the geometry. These properties usually
 * only require a fixed number of point coordinates to describe, such as center,
 * center of gravity, label point, circumscribed rectangle, inscribed circle,
 * circumscribed circle, inscribed circle*/
int geometry_prop_geo2(const coordinate_blob_t *c, int mode, double **result);

/* Modify geometry properties */
int geometry_modify(coordinate_blob_t *c, int mode, void *para);
/* Computes the result of two geometric combination operations */
coordinate_blob_t *geometry_combine(const coordinate_blob_t *a,
                                    const coordinate_blob_t *b, int mode);

/* Query whether two geometric relationships satisfy the specified spatial
 * relationship */
int geometry_spr_query_is(const coordinate_blob_t *a,
                          const coordinate_blob_t *b, int mode);
/* Determine the spatial relationship between two geometries and obtain the
 * intersection matrix, The matrix is an integer array of length 9 */
int geometry_spr_query(const coordinate_blob_t *a, const coordinate_blob_t *b,
                       int *matrix);

/* Check individual geometric correctness */
int geometry_check_1(const coordinate_blob_t *c, int mode);

/* Checks a set of geometries for correctness and writes error messages to the
 * stream */
int geometry_check_2(const i4_t *r, const int r_n, i4_t **w, int *w_n,
                     int mode);

/* -------------------------------------------------------------------------- */
/*                         Coordinate Reference System                        */
/* -------------------------------------------------------------------------- */

/* Spatial coordinate conversion */
typedef struct se_crs_transform crs_transformation_t;

/* Creates a coordinate transformation based on the EPSG value, or returns NULL
 * if the EPSG value is not found
 * epsg_from: from epsg code epsg_to: to epsg
 * code return: transformation object */
crs_transformation_t *create_trans(int epsg_from, int epsg_to);
/* Create commonly used coordinate transformations */
crs_transformation_t *create_wellknown_trans(int wellknown);
void destroy_trans(crs_transformation_t *trans);

/* trans point */
int trans_coord(const crs_transformation_t *trans, int direction, double *c);
/* trans point array */
int trans_coord_array(const crs_transformation_t *trans, int direction,
                      double *c, int c_n);

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
typedef struct se_data_blob {
  uint64_t bs; /* blob size */
  char *data;  /* blob header pointer */
} data_blob;

/* memory table */
typedef struct se_mem_table mem_table_t;

/* Create memory table
 * fields memory table field
 * size memory table row size
 * flag memory table flag
        0: create OID and geometry field
        1: do not create OID and geometry field
*/
mem_table_t *create_mem_table(table_field_t *fields, uint32_t size, int flag);
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

/*
 * Hashing works like this:
 * Divide the world into 4 buckets.  Label each one as such:
 *  -----------------
 *  |       |       |
 *  |       |       |
 *  | 0,1   | 1,1   |
 *  -----------------
 *  |       |       |
 *  |       |       |
 *  | 0,0   | 1,0   |
 *  -----------------
 */

/* The point structure of the GeoHash algorithm, which saves point coordinates,
 * longitude and latitude, and point attribute information */
typedef struct se_geohash_point geohash_point_t;

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

bool geohash_decode_area_longlat(const geohash_area_t *area, double *xy);
bool geohash_decode_longlat_type(const geohash_area_t *area, double *xy);
bool geohash_decode_longlat_WGS84(const geohash_area_t *area, double *xy);

geohash_neighbors_t *geohash_query_neighbors(const geohash_bits_t *hash);

/* -------------------------------------------------------------------------- */
/*                                Spatial Index                               */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                 Read Write                                 */
/* -------------------------------------------------------------------------- */

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

void rgb2hsv(RGB rgb, double *h, double *s, double *v);
void hsv2rgb(double h, double s, double v, RGB *rgb);
RGB random_color();

/* Map drawing context, which is a memory block in png format and supports
 * multi-threaded drawing.*/
typedef struct se_graphics_context graphics_context_t;

/* Graphics display transform structure */
typedef struct se_graphics_display_transform {
  double reference_scale;  /* reference scale */
  double map_extent[4];    /* map extent */
  double device_extent[2]; /* device extent. device is a size struct, only need
                              to set width and height */
  double resolution;   /* resolution, The geographic length represented by each
                          pixel*/
  double scale;        /* map scale */
  double rotate_angle; /* map rotate angle */
  float dpi;           /* device dpi, default value is 96.*/
} graphics_display_transform_t;

/* Point symbol structure */
typedef struct se_point_symbol {
  RGB color;  /* point color */
  float size; /* Point size. When the symbol is a circular point, it represents
                 the diameter of the circle; when it is a square point, it
                 represents the side length of the square. */
  int style;  /* point style, circle or square */

} point_symbol_t;

/* Line symbol structure */
typedef struct se_line_symbol {
  RGB color;          /* line color */
  float thickness;    /* line thickness */
  int pen_cap;        /* pen cap style */
  int pen_join;       /* pen join style */
  int pen_style;      /* pen style */
  double miter_limit; /* The miter limit of the pen. The miter limit is only
                         relevant when the join style is miter. */
} line_symbol_t;

/* Fill symbol structure */
typedef struct se_fill_symbol {
  RGB fill_color;         /* fill color */
  line_symbol_t out_line; /* fill out line symbol */
  int brush_style;        /* brush style */
} fill_symbol_t;

/* Create graphics context
 * The width and height of the context are in pixels.
 * flags: 0: single thread drawing(default)
 *        1: Multithreaded drawing */
graphics_context_t *create_graphics_context(const int width, const int height,
                                            context_format_t fmt, int flags);
void destroy_graphics_context(graphics_context_t *context);

/* --------------------------------------------------------------------------
 */
/*                                   Matrix */
/* --------------------------------------------------------------------------
 */

/* Affine transformation matrix
 * m11 | m12 | 0
 * m21 | m22 | 0
 * dx  | dy  | 1
 * Typically, after initializing the matrix, the values of each element are
 * as follows [1 0 0] [0 1 0] [0 0 1] The basic formula of affine
 * transformation is nx = m11 * x + m21 * y + dx; ny = m12 * x + m22 * y +
 * dy;
 */
typedef struct se_matrix {
  union {
    float m_m[6];
    struct {
      float m_m11, m_m12, m_m21, m_m22, m_dx, m_dy;
    };
  };
} matrix_t;

/* Matrix left multiplication */
void matrix_left_multiply(matrix_t *m, const matrix_t *mat);
/* Matrix (right) multiplication */
void matrix_multiply(matrix_t *m, const matrix_t *mat);
/* Matrix translation transformation */
void matrix_translate(matrix_t *m, const float x, const float y);
/* Matrix scaling transformation */
void matrix_scale(matrix_t *m, const float x, const float y);
/* Matrix rotation transformation */
void matrix_rotate(matrix_t *m, const float angle);
/* Matrix rotation transformation based on a certain center point */
void matrix_rotate2(matrix_t *m, const float angle, const float x,
                    const float y);
/* matrix shear transformation */
void matrix_shear(matrix_t *m, const float x, const float y);
/* Matrix mirror transformation */
void matrix_mirror(matrix_t *m, const float x, const float y);
/* Matrix inversion matrix */
void matrix_invert(matrix_t *m);
/* Determine whether the matrix can be inverted */
bool matrix_invertible(const matrix_t *m);
/* Determine whether the matrix can be an identity matrix */
bool matrix_identity(const matrix_t *m);

/* Transform point coordinates using a matrix */
void matrix_trans_points(const matrix_t *m, double *points, int count);

#ifdef __cplusplus
}
#endif

#endif //__MATHSE_H__