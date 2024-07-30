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

#include "mg.h"

void mg_ellipse_prop_value(const struct mg_ellipse ell, int flags,
                           double *values)
{
    int pos = 0;
    assert(values);

    // calc eccentrucity
    if (flags & MG_ELLIPSE_PROP_VALUE_ECCENTRICITY) {
        double dis = sqrt(ell.major * ell.major - ell.minor * ell.minor);
        values[pos] = dis / ell.major;
        pos += 1;
    }

    // calc area
    if (flags & MG_ELLIPSE_PROP_VALUE_AREA) {
        values[pos] = M_PI * ell.major * ell.minor;
        pos += 1;
    }

    // calc perimeter
    if (flags & MG_ELLIPSE_PROP_VALUE_PERIMETER) {
        if (ell.major == ell.minor) {
            values[pos] = M_PI * 2.0 * ell.major;
        }
        else {
            values[pos] =
                M_PI *
                (3 * (ell.major + ell.minor) -
                 sqrt(10 * ell.major * ell.minor +
                      3 * (ell.major * ell.major + ell.minor * ell.minor)));
        }
        pos += 1;
    }

    // calc foci
    if (flags & MG_ELLIPSE_PROP_VALUE_FOCI) {
        double dis = sqrt(ell.major * ell.major - ell.minor * ell.minor);
        struct mg_point p1 = mg_point_project(ell.center, dis, ell.azimuth);
        struct mg_point p2 = mg_point_project(ell.center, -dis, ell.azimuth);
        memcpy(values + pos, &p1, sizeof(struct mg_point));
        memcpy(values + pos + 2, &p2, sizeof(struct mg_point));
        pos += 4;
    }

    // calc focus distance
    if (flags & MG_ELLIPSE_PROP_FOCUS_DISTANCE) {
        double dis = sqrt(ell.major * ell.major - ell.minor * ell.minor);
        values[pos] = dis;
        pos += 1;
    }
}

void mg_construct_circle(const struct mg_point *p, int t,
                         const struct mg_ellipse *rs, int *n)
{
    assert(p);
    assert(rs);
    if (MG_CONSTRUCT_CIRCLE_2P == t) {
    }
    else if (MG_CONSTRUCT_CIRCLE_3P == t) {
    }
    else if (MG_CONSTRUCT_CIRCLE_ICT == t) {
    }
}