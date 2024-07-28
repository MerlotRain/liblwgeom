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

#ifndef MG_H
#define MG_H

#include "geom.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tg_line {
    int _; // unused
    bool closed;
    bool clockwise;
    int npoints;
    int nsegs;
    struct tg_envelope rect;
    struct tg_point points[];
};

struct tg_ring {
    int _; // unused
};

struct tg_polygon {
    struct tg_ring *exterior;
    int nholes;
    struct tg_ring **holes;
};

struct tg_multi {
    struct tg_geom **geoms;
    int ngeoms;
    struct tg_envelope rect; // unioned rect child geometries
    struct index *index;     // geometry index, or NULL if not indexed
    int *ixgeoms;            // indexed geometries, or NULL if not indexed
};

struct tg_geom {
    int type;
    int flags;
    union {
        struct tg_point point;
        struct tg_line *line;
        struct tg_polygon *polygon;
        struct tg_multi *multi;
    };
    union {
        struct { // TG_POINT
            double z;
            double m;
        };
        struct {            // !TG_POINT
            double *coords; // extra dimensinal coordinates
            int ncoords;
        };
    };
};

#ifdef __cplusplus
}
#endif

#endif