/**
 * Copyright (c) 2023-present Merlot.Rain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <nv-common.h>

/**
 * The GeoHash module defaults to using the WGS84 coordinate system and defines
 * the ellipsoidal parameters related to WGS84. So, all input coordinates
 * should use the geodetic coordinates in the WGS84 coordinate system. In the
 * CRS module, common coordinate conversion methods are provided. If you need
 * to convert relevant coordinates into projection coordinates, please complete
 * them in the CRS module.
 * https://epsg.io/4326
 */

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

#define GEO_LAT_MIN  -85.05112878
#define GEO_LAT_MAX  85.05112878
#define GEO_LONG_MIN -180
#define GEO_LONG_MAX 180

#define D_R          (M_PI / 180.0)
#define R_MAJOR      6378137.0
#define R_MINOR      6356752.3142
#define RATIO        (R_MINOR / R_MAJOR)
#define ECCENT       (sqrt(1.0 - (RATIO * RATIO)))
#define COM          (0.5 * ECCENT)

/// @brief The usual PI/180 constant
const double DEG_TO_RAD = 0.017453292519943295769236907684886;
/// @brief Earth's quatratic mean radius for WGS-84
const double EARTH_RADIUS_IN_METERS = 6372797.560856;

const double MERCATOR_MAX = 20037726.37;
const double MERCATOR_MIN = -20037726.37;

#define HASHISZERO(r)   (!(r).bits && !(r).step)
#define RANGEISZERO(r)  (!(r).max && !(r).min)
#define RANGEPISZERO(r) ((r) == NULL || RANGEISZERO(*(r)))

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

static void geohash_move_x(geohash_bits *hash, int8_t d);
static void geohash_move_y(geohash_bits *hash, int8_t d);

struct geo_array *g_geo_array = NULL;

void init_geo_point_buckets()
{
    if (g_geo_array == NULL) {
        g_geo_array = lwmalloc(sizeof(struct geo_array));
        if (g_geo_array == NULL)
            return;
    }
}
/* http://graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN */
static inline uint64_t interleave64(uint32_t xlo, uint32_t ylo)
{
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

static inline uint64_t deinterleave64(uint64_t interleaved)
{
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

void get_geohash_range(geohash_range *long_range, geohash_range *lat_range)
{
    long_range->max = GEO_LONG_MAX;
    long_range->min = GEO_LONG_MIN;
    lat_range->max = GEO_LAT_MAX;
    lat_range->min = GEO_LAT_MIN;
}

int geohash_encode(const geohash_range *long_range,
                   const geohash_range *lat_range, double longitude,
                   double latitude, uint8_t step, geohash_bits *hash)
{
    /* Check basic arguments sanity. */
    if (hash == NULL || step > 32 || step == 0 || RANGEPISZERO(lat_range) ||
        RANGEPISZERO(long_range))
        return 0;

    /* Return an error when trying to index outside the supported
     * constraints. */
    if (longitude > GEO_LONG_MAX || longitude < GEO_LONG_MIN ||
        latitude > GEO_LAT_MAX || latitude < GEO_LAT_MIN)
        return 0;

    hash->bits = 0;
    hash->step = step;

    if (latitude < lat_range->min || latitude > lat_range->max ||
        longitude < long_range->min || longitude > long_range->max) {
        return 0;
    }

    double lat_offset =
        (latitude - lat_range->min) / (lat_range->max - lat_range->min);
    double long_offset =
        (longitude - long_range->min) / (long_range->max - long_range->min);

    /* convert to fixed point based on the step size */
    lat_offset *= (1ULL << step);
    long_offset *= (1ULL << step);
    hash->bits = interleave64(lat_offset, long_offset);
    return 1;
}

int geohash_encode_type(double longitude, double latitude, uint8_t step,
                        geohash_bits *hash)
{
    geohash_range r[2] = {{0}};
    get_geohash_range(&r[0], &r[1]);
    return geohash_encode(&r[0], &r[1], longitude, latitude, step, hash);
}

int geohash_encode_WGS84(double longitude, double latitude, uint8_t step,
                         geohash_bits *hash)
{
    return geohash_encode_type(longitude, latitude, step, hash);
}

int geohash_decode(const geohash_range long_range,
                   const geohash_range lat_range, const geohash_bits hash,
                   geohash_area *area)
{
    if (HASHISZERO(hash) || NULL == area || RANGEISZERO(lat_range) ||
        RANGEISZERO(long_range)) {
        return 0;
    }

    area->hash = hash;
    uint8_t step = hash.step;
    uint64_t hash_sep = deinterleave64(hash.bits); /* hash = [LAT][LONG] */

    double lat_scale = lat_range.max - lat_range.min;
    double long_scale = long_range.max - long_range.min;

    uint32_t ilato = hash_sep;       /* get lat part of deinterleaved hash */
    uint32_t ilono = hash_sep >> 32; /* shift over to get long part of hash */

    /* divide by 2**step.
     * Then, for 0-1 coordinate, multiply times scale and add
       to the min to get the absolute coordinate. */
    area->latitude.min =
        lat_range.min + (ilato * 1.0 / (1ull << step)) * lat_scale;
    area->latitude.max =
        lat_range.min + ((ilato + 1) * 1.0 / (1ull << step)) * lat_scale;
    area->longitude.min =
        long_range.min + (ilono * 1.0 / (1ull << step)) * long_scale;
    area->longitude.max =
        long_range.min + ((ilono + 1) * 1.0 / (1ull << step)) * long_scale;

    return 1;
}

int geohash_decode_type(const geohash_bits hash, geohash_area *area)
{
    geohash_range r[2] = {{0}};
    get_geohash_range(&r[0], &r[1]);
    return geohash_decode(r[0], r[1], hash, area);
}

int geohash_decode_WGS84(const geohash_bits hash, geohash_area *area)
{
    return geohash_decode_type(hash, area);
}

int geohash_decode_area_longlat(const geohash_area *area, double *xy)
{
    if (!xy)
        return 0;
    xy[0] = (area->longitude.min + area->longitude.max) / 2;
    if (xy[0] > GEO_LONG_MAX)
        xy[0] = GEO_LONG_MAX;
    if (xy[0] < GEO_LONG_MIN)
        xy[0] = GEO_LONG_MIN;
    xy[1] = (area->latitude.min + area->latitude.max) / 2;
    if (xy[1] > GEO_LAT_MAX)
        xy[1] = GEO_LAT_MAX;
    if (xy[1] < GEO_LAT_MIN)
        xy[1] = GEO_LAT_MIN;
    return 1;
}

int geohash_decode_longlat_type(const geohash_bits hash, double *xy)
{
    geohash_area area = {{0}};
    if (!xy || !geohash_decode_type(hash, &area))
        return 0;
    return geohash_decode_area_longlat(&area, xy);
}

int geohash_decode_longlat_WGS84(const geohash_bits hash, double *xy)
{
    return geohash_decode_longlat_type(hash, xy);
}

void geohash_query_neighbors(const geohash_bits *hash,
                             geohash_neighbors *neighbors)
{
    neighbors->east = *hash;
    neighbors->west = *hash;
    neighbors->north = *hash;
    neighbors->south = *hash;
    neighbors->south_east = *hash;
    neighbors->south_west = *hash;
    neighbors->north_east = *hash;
    neighbors->north_west = *hash;

    geohash_move_x(&neighbors->east, 1);
    geohash_move_y(&neighbors->east, 0);

    geohash_move_x(&neighbors->west, -1);
    geohash_move_y(&neighbors->west, 0);

    geohash_move_x(&neighbors->south, 0);
    geohash_move_y(&neighbors->south, -1);

    geohash_move_x(&neighbors->north, 0);
    geohash_move_y(&neighbors->north, 1);

    geohash_move_x(&neighbors->north_west, -1);
    geohash_move_y(&neighbors->north_west, 1);

    geohash_move_x(&neighbors->north_east, 1);
    geohash_move_y(&neighbors->north_east, 1);

    geohash_move_x(&neighbors->south_east, 1);
    geohash_move_y(&neighbors->south_east, -1);

    geohash_move_x(&neighbors->south_west, -1);
    geohash_move_y(&neighbors->south_west, -1);
}

static void geohash_move_x(geohash_bits *hash, int8_t d)
{
    if (d == 0)
        return;

    uint64_t x = hash->bits & 0xaaaaaaaaaaaaaaaaULL;
    uint64_t y = hash->bits & 0x5555555555555555ULL;

    uint64_t zz = 0x5555555555555555ULL >> (64 - hash->step * 2);

    if (d > 0) {
        x = x + (zz + 1);
    }
    else {
        x = x | zz;
        x = x - (zz + 1);
    }

    x &= (0xaaaaaaaaaaaaaaaaULL >> (64 - hash->step * 2));
    hash->bits = (x | y);
}

static void geohash_move_y(geohash_bits *hash, int8_t d)
{
    if (d == 0)
        return;

    uint64_t x = hash->bits & 0xaaaaaaaaaaaaaaaaULL;
    uint64_t y = hash->bits & 0x5555555555555555ULL;

    uint64_t zz = 0xaaaaaaaaaaaaaaaaULL >> (64 - hash->step * 2);
    if (d > 0) {
        y = y + (zz + 1);
    }
    else {
        y = y | zz;
        y = y - (zz + 1);
    }
    y &= (0x5555555555555555ULL >> (64 - hash->step * 2));
    hash->bits = (x | y);
}