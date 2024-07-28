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

#include <mathse.h>

#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct geohash_bits {
    uint64_t bits;
    uint8_t step;
} geohash_bits;

typedef struct geohash_neighbors {
    geohash_bits north;
    geohash_bits east;
    geohash_bits west;
    geohash_bits south;
    geohash_bits south_west;
    geohash_bits south_east;
    geohash_bits north_west;
    geohash_bits north_east;
} geohash_neighbors;

typedef struct geohash_range {
    double max;
    double min;
} geohash_range;

typedef struct geohash_area {
    geohash_bits hash;
    geohash_range longitude;
    geohash_range latitude;
} geohash_area;

EXTERN void get_geohash_range(geohash_range *long_range,
                              geohash_range *lat_range);
EXTERN int geohash_encode(const geohash_range *long_range,
                          const geohash_range *lat_range, double longitude,
                          double latitude, uint8_t step, geohash_bits *hash);
EXTERN int geohash_encode_type(double longitude, double latitude, uint8_t step,
                               geohash_bits *hash);
EXTERN int geohash_encode_WGS84(double longitude, double latitude, uint8_t step,
                                geohash_bits *hash);

EXTERN int geohash_decode(const geohash_range long_range,
                          const geohash_range lat_range,
                          const geohash_bits hash, geohash_area *area);
EXTERN int geohash_decode_type(const geohash_bits hash, geohash_area *area);
EXTERN int geohash_decode_WGS84(const geohash_bits hash, geohash_area *area);
EXTERN int geohash_decode_area_longlat(const geohash_area *area, double *xy);
EXTERN int geohash_decode_longlat_type(const geohash_bits hash, double *xy);
EXTERN int geohash_decode_longlat_WGS84(const geohash_bits hash, double *xy);

EXTERN void geohash_query_neighbors(const geohash_bits *hash,
                                    geohash_neighbors *neighbors);

#ifdef __cplusplus
}
#endif

#endif