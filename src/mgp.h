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

#ifndef MGP_H
#define MGP_H

#include "mg.h"

#ifdef __cplusplus
extern "C" {
#endif

union mg_upoint {
    struct {
        double x;
        double y;
        double z;
        double m;
    };
    struct mg_point pt;
    double extend[2];
};

struct mg_path {
    bool clockwise;
    int npoints;
    struct mg_envelope rect;
    union mg_upoint points[];
};

struct mg_ring {
    bool clockwise;
    int npoints;
    struct mg_envelope rect;
    union mg_upoint points[];
};

struct mg_polygon {
    struct mg_envelope rect;
    struct mg_ring *exterior;
    int nholes;
    struct mg_ring **holes;
};

struct mg_multi {
    struct mg_geom **geoms;
    int ngeoms;
    struct mg_envelope rect; // unioned rect child geometries
};

struct mg_geom {
    int geomt;
    int flags;
    union {
        union mg_upoint point;
        struct mg_path *path;
        struct mg_polygon *polygon;
        struct mg_multi *multi;
    };
};

#ifdef __cplusplus
}
#endif

#endif