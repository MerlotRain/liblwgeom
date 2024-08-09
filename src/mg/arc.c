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

#include "mghelp.h"

static int pri_arc_center(struct mg_point c0, struct mg_point c1,
                          struct mg_point c2, struct mg_point *center)
{
    if(c0.x == c2.x && c0.y == c2.y)
    {
        center->x = (c0.x + c1.x) / 2.0;
        center->y = (c0.y + c1.y) / 2.0;
        return 1;
    }

    double m1, x1, y1;


}

struct mg_object *mg_stroke_arc(struct mg_arc arc,
                                double maxAngleStepSizeDegress)
{
    double startAngle, alongAngle, endAngle;
    double cx, cy, radius;
    return 0;
}