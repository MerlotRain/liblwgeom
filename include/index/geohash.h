/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#ifndef GEOHASH_H
#define GEOHASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mathse.h>

/* Direction north */
#define GEOHASH_DIRECTION_NORTH      0
/* Direction east */
#define GEOHASH_DIRECTION_EAST       1
/* Direction west */
#define GEOHASH_DIRECTION_WEST       2
/* Direction south */
#define GEOHASH_DIRECTION_SOUTH      3
/* Direction south west */
#define GEOHASH_DIRECTION_SOUTH_WEST 4
/* Direction south east */
#define GEOHASH_DIRECTION_SOUTH_EAST 5
/* Direction north west */
#define GEOHASH_DIRECTION_NORTH_WEST 6
/* Direction north east */
#define GEOHASH_DIRECTION_NORTH_EAST 7

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

typedef struct geohash_bits {
    uint64_t bits;
    uint8_t step;
} geohash_bits_t;

typedef struct geohash_neighbors {
    geohash_bits_t north;
    geohash_bits_t east;
    geohash_bits_t west;
    geohash_bits_t south;
    geohash_bits_t south_west;
    geohash_bits_t south_east;
    geohash_bits_t north_west;
    geohash_bits_t north_east;
} geohash_neighbors_t;

typedef struct geohash_range {
    double max;
    double min;
} geohash_range_t;

typedef struct se_geohash_area {
    geohash_bits_t hash;
    geohash_range_t longitude;
    geohash_range_t latitude;
} geohash_area_t;

EXTERN void get_geohash_range(geohash_range_t *long_range,
                              geohash_range_t *lat_range);
EXTERN geohash_bits_t *geohash_encode(const geohash_range_t *long_range,
                                      const geohash_range_t *lat_range,
                                      double longitude, double latitude,
                                      uint8_t step);
EXTERN geohash_bits_t *geohash_encode_type(double longitude, double latitude,
                                           uint8_t step);
EXTERN geohash_bits_t *geohash_encode_WGS84(double longitude, double latitude,
                                            uint8_t step);
EXTERN geohash_area_t *geohash_decode(const geohash_range_t long_range,
                                      const geohash_range_t lat_range,
                                      const geohash_bits_t hash);
EXTERN geohash_area_t *geohash_decode_type(const geohash_range_t long_range,
                                           const geohash_range_t lat_range,
                                           const geohash_bits_t hash);
EXTERN geohash_area_t *geohash_decode_WGS84(const geohash_range_t long_range,
                                            const geohash_range_t lat_range,
                                            const geohash_bits_t hash);

EXTERN bool geohash_decode_area_longlat(const geohash_area_t *area, double *xy);
EXTERN bool geohash_decode_longlat_type(const geohash_area_t *area, double *xy);
EXTERN bool geohash_decode_longlat_WGS84(const geohash_area_t *area,
                                         double *xy);

EXTERN geohash_neighbors_t *geohash_query_neighbors(const geohash_bits_t *hash);

#ifdef __cplusplus
}
#endif

#endif