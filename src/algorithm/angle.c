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

double mg_angle(const struct mg_point p0)
{
    return atan2(p0.y, p0.x);
}

double mg_angle2(const struct mg_point p0, const struct mg_point p1)
{
    double dx = p1.x - p0.x;
    double dy = p1.y - p0.y;    
    return atan2(dy, dx);
}

bool mg_acute(const struct mg_point p0, const struct mg_point p1,
    const struct mg_point p2)
{
    double dx0 = p0.x - p1.x;
    double dy0 = p0.y - p1.y;
    double dx1 = p2.x - p1.x;
    double dy1 = p2.y - p1.y;
    double dotprod = dx0 * dx1 + dy0 * dy1;
    return dotprod > 0;
}

bool mg_obtuse(const struct mg_point p0, const struct mg_point p1,
    const struct mg_point p2)
{
    double dx0 = p0.x - p1.x;
    double dy0 = p0.y - p1.y;
    double dx1 = p2.x - p1.x;
    double dy1 = p2.y - p1.y;
    double dotprod = dx0 * dx1 + dy0 * dy1;
    return dotprod < 0; 
}

double mg_angle_between(const struct mg_point tip1, const struct mg_point tail,
    const struct mg_point tip2)
{
    double a1 = angle(tail, tip1);
    double a2 = angle(tail, tip2);

    return diff(a1, a2);
}

bool mg_interior_angle(const struct mg_point p0, const struct mg_point p1,
    const struct mg_point p2)
{
    return false;
}