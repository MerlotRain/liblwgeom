#include "../mathse.h"
#include <stddef.h>

/**
 * The GeoHash module defaults to using the WGS84 coordinate system and defines
 * the ellipsoidal parameters related to WGS84. So, all input coordinates should
 * use the geodetic coordinates in the WGS84 coordinate system. In the CRS
 * module, common coordinate conversion methods are provided. If you need to
 * convert relevant coordinates into projection coordinates, please complete
 * them in the CRS module.
 * https://epsg.io/4326
 */

#define GEO_LAT_MIN -85.05112878
#define GEO_LAT_MAX 85.05112878
#define GEO_LONG_MIN -180
#define GEO_LONG_MAX 180

#define D_R (M_PI / 180.0)
#define R_MAJOR 6378137.0
#define R_MINOR 6356752.3142
#define RATIO (R_MINOR / R_MAJOR)
#define ECCENT (sqrt(1.0 - (RATIO * RATIO)))
#define COM (0.5 * ECCENT)

/// @brief The usual PI/180 constant
const double DEG_TO_RAD = 0.017453292519943295769236907684886;
/// @brief Earth's quatratic mean radius for WGS-84
const double EARTH_RADIUS_IN_METERS = 6372797.560856;

const double MERCATOR_MAX = 20037726.37;
const double MERCATOR_MIN = -20037726.37;

struct geo_point {
  double longitude;
  double latitude;
  double code;
  char *name;
};

struct geo_array {
  struct geo_point *array;
  size_t buckets;
  size_t used;
};

struct geo_array *g_geo_array = NULL;

void init_geo_point_buckets() {
  if (g_geo_array == NULL) {
    g_geo_array = malloc(sizeof(struct geo_array));
    if (g_geo_array == NULL)
      return;
  }
}
/* http://graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN */
static inline uint64_t interleave64(uint32_t xlo, uint32_t ylo) {
  static const uint64_t B[] = {0x5555555555555555ULL, 0x3333333333333333ULL,
                               0x0F0F0F0F0F0F0F0FULL, 0x00FF00FF00FF00FFULL,
                               0x0000FFFF0000FFFFULL};
  static const int S[] = {1, 2, 4, 8, 16};
  uint64_t x = xlo;
  uint64_t y = ylo;

  x = (x | (x << S[4])) & B[4];
  y = (y | (y << S[4])) & B[4];

  x = (x | (x << S[3])) & B[3];
  y = (y | (y << S[3])) & B[3];

  x = (x | (x << S[2])) & B[2];
  y = (y | (y << S[2])) & B[2];

  x = (x | (x << S[1])) & B[0];
  y = (y | (y << S[1])) & B[0];

  x = (x | (x << S[0])) & B[0];
  y = (y | (y << S[0])) & B[0];

  return x | (y << 1);
}

static inline uint64_t deinterleave64(uint64_t interleaved) {
  static const uint64_t B[] = {0x5555555555555555ULL, 0x3333333333333333ULL,
                               0x0F0F0F0F0F0F0F0FULL, 0x00FF00FF00FF00FFULL,
                               0x0000FFFF0000FFFFULL};
  static const int S[] = {1, 2, 4, 8, 16};
  uint64_t x = interleaved;
  uint64_t y = interleaved >> 1;

  x = (x | (x >> S[4])) & B[4];
  y = (y | (y >> S[4])) & B[4];

  x = (x | (x >> S[3])) & B[3];
  y = (y | (y >> S[3])) & B[3];

  x = (x | (x >> S[2])) & B[2];
  y = (y | (y >> S[2])) & B[2];

  x = (x | (x >> S[1])) & B[0];
  y = (y | (y >> S[1])) & B[0];

  x = (x | (x >> S[0])) & B[0];
  y = (y | (y >> S[0])) & B[0];

  return x | (y << 32);
}

void get_geohash_range(geohash_range_t *long_range,
                       geohash_range_t *lat_range) {
  long_range->max = GEO_LONG_MAX;
  long_range->min = GEO_LONG_MIN;
  lat_range->max = GEO_LAT_MAX;
  lat_range->min = GEO_LAT_MIN;
}

geohash_bits_t *geohash_encode(const geohash_range_t *long_range,
                               const geohash_range_t *lat_range,
                               double longitude, double latitude,
                               uint8_t step) {
  return NULL;
}

geohash_bits_t *geohash_encode_type(double longitude, double latitude,
                                    uint8_t step) {
  return NULL;
}

geohash_bits_t *geohash_encode_WGS84(double longitude, double latitude,
                                     uint8_t step) {
  return NULL;
}

geohash_area_t *geohash_decode(const geohash_range_t long_range,
                               const geohash_range_t lat_range,
                               const geohash_bits_t hash) {
  return NULL;
}

geohash_area_t *geohash_decode_type(const geohash_range_t long_range,
                                    const geohash_range_t lat_range,
                                    const geohash_bits_t hash) {
  return NULL;
}

geohash_area_t *geohash_decode_WGS84(const geohash_range_t long_range,
                                     const geohash_range_t lat_range,
                                     const geohash_bits_t hash) {
  return NULL;
}

bool geohash_decode_area_longlat(const geohash_area_t *area, double *xy) {
  return false;
}

bool geohash_decode_longlat_type(const geohash_area_t *area, double *xy) {
  return false;
}

bool geohash_decode_longlat_WGS84(const geohash_area_t *area, double *xy) {
  return false;
}

geohash_neighbors_t *geohash_query_neighbors(const geohash_bits_t *hash) {
  return NULL;
}
